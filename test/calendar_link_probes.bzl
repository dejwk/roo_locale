load("@rules_cc//cc:cc_binary.bzl", "cc_binary")

def calendar_link_probes():
    """Build language-only, single-locale, and all-locale archive link probes."""
    cc_binary(
        name = "calendar_probe_language_only",
        srcs = ["test/language_only.cpp"],
        linkopts = ["-Wl,--gc-sections"],
        linkstatic = True,
        deps = [":roo_locale"],
    )
    locales = [
        "pl_PL",
        "en_US",
        "en_GB",
        "fr_FR",
        "es_ES",
        "it_IT",
        "de_DE",
        "ru_RU",
        "zh_Hans_CN",
        "hi_IN",
        "ar_EG",
    ]
    for name in locales + ["all"]:
        cc_binary(
            name = "calendar_probe_" + name,
            srcs = ["test/calendar_link_probe.cpp"],
            local_defines = ["ROO_TEST_ALL_CALENDARS"] if name == "all" else [
                "ROO_CALENDAR_LOCALE=" + name,
                "ROO_TEST_CALENDAR_ACCESSOR=DefaultCalendarLocale",
            ],
            linkopts = ["-Wl,--gc-sections"],
            linkstatic = True,
            deps = [":calendar_locale"],
        )

    native.genrule(
        name = "calendar_link_acceptance",
        tags = ["manual"],
        srcs = [":calendar_probe_" + name for name in ["language_only"] + locales + ["all"]],
        tools = ["test/verify_calendar_linking.py"],
        toolchains = ["@bazel_tools//tools/cpp:current_cc_toolchain"],
        outs = ["calendar_link_acceptance/report.json"],
        cmd = "python3 $(location test/verify_calendar_linking.py) --cxx $(CC) --bazel-bin $(RULEDIR) --output-dir $(@D)",
    )
