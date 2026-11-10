#!/usr/bin/env python3
"""Verify selective archive linking and report the thirteen calendar probe sizes.

Requires a GNU-compatible C++ compiler and binutils. For ESP32, pass the Xtensa
compiler and the installed SDK's esp32/ld directory. The cross probes use the
SDK's real memory/section scripts, with main as the entry and no startup runtime;
they measure this library's placement, not a complete bootable application.
"""

import argparse
import json
from pathlib import Path
import re
import shutil
import subprocess


LOCALES = {
    "pl_PL": ("pl-PL", "PolishPoland", "styczeń"),
    "en_US": ("en-US", "EnglishUnitedStates", "January"),
    "en_GB": ("en-GB", "EnglishUnitedKingdom", "January"),
    "fr_FR": ("fr-FR", "FrenchFrance", "janvier"),
    "es_ES": ("es-ES", "SpanishSpain", "enero"),
    "it_IT": ("it-IT", "ItalianItaly", "gennaio"),
    "de_DE": ("de-DE", "GermanGermany", "Januar"),
    "ru_RU": ("ru-RU", "RussianRussia", "январь"),
    "zh_Hans_CN": ("zh-Hans-CN", "ChineseSimplifiedChina", "一月"),
    "hi_IN": ("hi-IN", "HindiIndia", "जनवरी"),
    "ar_EG": ("ar-EG", "ArabicEgypt", "يناير"),
}


def run(command):
    return subprocess.check_output([str(arg) for arg in command], text=True)


def require(condition, message):
    if not condition:
        raise RuntimeError(message)


def sections(objdump, path):
    """Read section sizes, addresses, and permissions from GNU objdump."""
    lines = run([objdump, "-h", path]).splitlines()
    result = {}
    for index, line in enumerate(lines):
        match = re.match(r"\s*\d+\s+(\S+)\s+([0-9a-f]+)\s+([0-9a-f]+)\s", line)
        if match is None:
            continue
        name, size, address = match.groups()
        result[name] = {
            "size": int(size, 16),
            "address": int(address, 16),
            "flags": lines[index + 1],
        }
    return result


def is_esp32_flash(section):
    address = section["address"]
    return (0x3f400000 <= address < 0x3f800000 or
            0x400d0000 <= address < 0x40400000)


def writable_size(table, esp32=False):
    return sum(section["size"] for section in table.values()
               if "ALLOC" in section["flags"]
               and "READONLY" not in section["flags"]
               and not (esp32 and is_esp32_flash(section)))


def readonly_size(table, esp32=False):
    return sum(section["size"] for section in table.values()
               if "ALLOC" in section["flags"]
               and (is_esp32_flash(section) if esp32
                    else "READONLY" in section["flags"]))


def verify_objects(objects, nm, objdump):
    """Reject writable locale storage, dynamic initialization, and runtime deps."""
    for path in objects:
        table = sections(objdump, path)
        require(writable_size(table) == 0, f"Writable locale data in {path}")
        require(not any(section["size"] != 0 and
                        ("init_array" in name or "ctors" in name)
                        for name, section in table.items()),
                f"Dynamic initialization in {path}")
        symbols = run([nm, "-C", path])
        require(re.search(r"_GLOBAL__sub_I|__cxa|operator new|\b(malloc|calloc|"
                          r"realloc|free)\b|roo_time::|roo_windows::", symbols) is None,
                f"Unexpected runtime dependency in {path}")


def verify_image(image, expected, nm, objdump, esp32, symbol_output):
    """Check the final ELF, not discarded sections appearing in a linker map."""
    symbols = run([nm, "-S", "-C", "--defined-only", image])
    symbol_output.write_text(symbols)
    require(re.search(r"_GLOBAL__sub_I|__cxa_guard|operator new", symbols) is None,
            f"Dynamic initialization or allocation in {image}")
    payload = image.read_bytes()
    for name, (tag, accessor, anchor) in LOCALES.items():
        require((f"roo_locale::{accessor}()" in symbols) == (name in expected),
                f"Unexpected accessor retention in {image}: {accessor}")
        require((tag.encode() + b"\0" in payload) == (name in expected),
                f"Unexpected locale tag retention in {image}: {tag}")
        # US and UK share names; other anchors must not cross locale boundaries.
        shares_names = name in expected or (
            name in ("en_US", "en_GB") and
            any(region in expected for region in ("en_US", "en_GB")))
        require((anchor.encode() + b"\0" in payload) == shares_names,
                f"Unexpected native name retention in {image}: {tag}")
    require(("kArabicDigits" in symbols) == ("ar_EG" in expected),
            f"Unexpected Arabic digit retention in {image}")
    require(("kLatinDigits" in symbols) == any(name != "ar_EG" for name in expected),
            f"Unexpected Latin digit retention in {image}")
    for region in ("Us", "Gb"):
        require(("kEnglishMonthsAbbreviated" + region in symbols) ==
                ("en_" + ("US" if region == "Us" else "GB") in expected),
                f"Unexpected regional English array retention in {image}")
    require(symbols.count("roo_locale::(anonymous namespace)::kPatterns\n") == len(expected),
            f"Unexpected pattern table retention in {image}")
    descriptor_sizes = []
    for line in symbols.splitlines():
        if line.endswith("roo_locale::(anonymous namespace)::kLocale"):
            descriptor_sizes.append(int(line.split()[1], 16))
    require(len(descriptor_sizes) == len(expected), f"Missing locale descriptors in {image}")
    if esp32:
        require(all(size <= 64 for size in descriptor_sizes), "32-bit descriptor budget")
    table = sections(objdump, image)
    if esp32:
        for name, section in table.items():
            if "ALLOC" not in section["flags"] or section["size"] == 0:
                continue
            if name.startswith(".flash."):
                require(is_esp32_flash(section),
                        f"Unexpected flash placement of {name} in {image}")
        for line in symbols.splitlines():
            if "roo_locale::" in line:
                address = int(line.split()[0], 16)
                require(is_esp32_flash({"address": address}),
                        f"Locale symbol outside flash: {line}")
    return {
        "readonly_bytes": readonly_size(table, esp32),
        "writable_bytes": writable_size(table, esp32),
        "descriptor_bytes": sorted(set(descriptor_sizes)),
        "sections": table,
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cxx", default="g++")
    parser.add_argument("--bazel-bin", type=Path,
                        help="Check existing Bazel probes instead of compiling")
    parser.add_argument("--esp32-ld-dir", type=Path)
    parser.add_argument("--output-dir", type=Path, required=True)
    args = parser.parse_args()
    repo = Path(__file__).resolve().parents[1]
    output = args.output_dir.resolve()
    output.mkdir(parents=True, exist_ok=True)
    cxx = shutil.which(args.cxx)
    require(cxx is not None, "Compiler not found")
    prefix = cxx[:-3]
    # GCC cross toolchains colocate prefixed binutils. Native Clang and wrapper
    # compilers can use the host binutils available on PATH.
    nm = shutil.which(prefix + "nm") or shutil.which("nm")
    objdump = shutil.which(prefix + "objdump") or shutil.which("objdump")
    ar = shutil.which(prefix + "ar") or shutil.which("ar")
    require(all(tool is not None for tool in (nm, objdump, ar)), "Binutils not found")
    esp32 = args.esp32_ld_dir is not None
    cases = [("language_only", [])] + [(name, [name]) for name in LOCALES]
    cases.append(("all", list(LOCALES)))
    if args.bazel_bin is not None:
        require(not esp32, "Bazel mode checks host probe images")
        results = {}
        for name, expected in cases:
            image = args.bazel_bin.resolve() / ("calendar_probe_" + name)
            results[name] = verify_image(image, expected, nm, objdump, False,
                                         output / (name + ".symbols"))
        baseline = results["language_only"]["sections"]
        for result in results.values():
            for name in (".init_array", ".ctors", ".bss", ".data"):
                require(result["sections"].get(name, {}).get("size", 0) ==
                        baseline.get(name, {}).get("size", 0),
                        f"Bazel probe added initialization or mutable data: {name}")
        (output / "report.json").write_text(json.dumps(results, indent=2) + "\n")
        print("Verified selective linking and constant initialization of 13 Bazel probes")
        return
    common = [cxx, "-std=c++11", "-Os", "-fno-exceptions", "-fno-rtti",
              "-fno-pic", "-fno-pie", "-ffunction-sections", "-fdata-sections",
              "-Wall", "-Wextra", "-Werror", "-I" + str(repo / "src")]
    objects = []
    for source in sorted((repo / "src/roo_locale").rglob("*.cpp")):
        obj = output / (source.stem + ".o")
        run(common + ["-c", source, "-o", obj])
        objects.append(obj)
    verify_objects(objects, nm, objdump)
    archive = output / "libcalendar_locale.a"
    archive.unlink(missing_ok=True)
    run([ar, "rcs", archive] + objects)
    results = {}
    for name, expected in cases:
        source = repo / "test" / ("language_only.cpp" if not expected else "calendar_link_probe.cpp")
        image = output / (name + ".elf")
        defines = []
        if name == "all":
            defines = ["-DROO_TEST_ALL_CALENDARS"]
        elif expected:
            defines = ["-DROO_TEST_CALENDAR_ACCESSOR=" + LOCALES[name][1]]
        link = ["-Wl,--gc-sections", "-Wl,-Map=" + str(image.with_suffix(".map"))]
        if esp32:
            link += ["-nostdlib", "-Wl,-e,main", "-L" + str(args.esp32_ld_dir.resolve()),
                     "-Tmemory.ld", "-Tsections.ld"]
        else:
            link += ["-no-pie"]
        run(common + defines + [source, archive] + link + ["-o", image])
        results[name] = verify_image(image, expected, nm, objdump, esp32,
                                     output / (name + ".symbols"))
        if not esp32:
            subprocess.run([str(image)], check=True)
    baseline = results["language_only"]
    for result in results.values():
        result["readonly_delta"] = result["readonly_bytes"] - baseline["readonly_bytes"]
        result["writable_delta"] = result["writable_bytes"] - baseline["writable_bytes"]
        require(result["writable_delta"] == 0, "Locale storage increased writable sections")
        if esp32:
            for section_name, section in result["sections"].items():
                if "ALLOC" in section["flags"] and not is_esp32_flash(section):
                    require(section["size"] == baseline["sections"].get(section_name, {}).get("size", 0),
                            f"Locale storage changed non-flash section {section_name}")
    report = {
        "compiler": run([cxx, "--version"]).splitlines()[0],
        "esp32_ld_dir": str(args.esp32_ld_dir) if esp32 else None,
        "probes": results,
    }
    (output / "report.json").write_text(json.dumps(report, indent=2) + "\n")
    print(report["compiler"])
    print("probe,readonly_delta_bytes,writable_delta_bytes,descriptor_bytes")
    for name, result in results.items():
        print(f"{name},{result['readonly_delta']},{result['writable_delta']},"
              f"{result['descriptor_bytes']}")
    print(f"Verified all 13 probes; maps, symbols, and report: {output}")


if __name__ == "__main__":
    main()
