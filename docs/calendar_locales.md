# Gregorian calendar locales

Include `roo_locale/calendar_locale.h` and link the optional Bazel
`//:calendar_locale` target. The existing `roo_locale.h` and `//:roo_locale`
remain language-only. Arduino and PlatformIO discover the `.cpp` files normally.

```cpp
#include "roo_locale/calendar_locale.h"

const roo_locale::CalendarLocale& locale = roo_locale::PolishPoland();
const char* november = locale.month_standalone_wide[10];  // listopad
const char* date_month = locale.month_date_wide[10];       // listopada
```

Built-ins have program lifetime. Consumers borrow a reference, which can differ
between widgets. Custom aggregates and every referenced string/array must remain
immutable and outlive their consumers. Validate custom UTF-8, cardinalities,
enums, and patterns at installation. No allocation, mutable selection, timezone,
formatter, calendar arithmetic, operating-system locale, or UI dependency exists.

## Patterns and text

Month arrays are January-first; weekday arrays are Sunday-first, regardless of
`first_weekday`. Narrow weekdays are calendar headers and need not be unique.
Date patterns use date-context names; month/year captions use standalone names.
Each locale supplies all fifteen `CalendarPatternId` slots, including both clock
cycles and milliseconds. Numeric dates always use four-digit years.

Patterns contain UTF-8 literals and braced tokens: `yyyy`, `M`, `MM`, `d`, `dd`,
`MMM`, `MMMM`, `LLL`, `LLLL`, `EEE`, `H`, `HH`, `h`, `mm`, `ss`, `SSS`, `a`,
and `decimal`. `MMM`/`MMMM` select date-context months; `LLL`/`LLLL` select
standalone months. `EEE` selects abbreviated weekdays. `H`/`HH` use 0–23;
`h` uses 1–12. Escape literal braces as `{{`/`}}`. Unknown tokens, unmatched
braces, embedded NULs, more than 255 UTF-8 bytes (excluding NUL), or more than
32 substitutions are errors. Substituted strings are never parsed recursively.
The consumer implements pattern evaluation, bounds checking, and shaping.

| Locale | Full date (10 November 2026) | Numeric date | Week start | Clock |
| --- | --- | --- | --- | --- |
| pl-PL | 10 listopada 2026 | 10.11.2026 | Monday | H23 |
| en-US | November 10, 2026 | 11/10/2026 | Sunday | H12 |
| en-GB | 10 November 2026 | 10/11/2026 | Monday | H23 |
| fr-FR | 10 novembre 2026 | 10/11/2026 | Monday | H23 |
| es-ES | 10 de noviembre de 2026 | 10/11/2026 | Monday | H23 |
| it-IT | 10 novembre 2026 | 10/11/2026 | Monday | H23 |
| de-DE | 10. November 2026 | 10.11.2026 | Monday | H23 |
| ru-RU | 10 ноября 2026 г. | 10.11.2026 | Monday | H23 |
| zh-Hans-CN | 2026年11月10日 | 2026/11/10 | Monday | H23 |
| hi-IN | 10 नवंबर 2026 | 10/11/2026 | Sunday | H12 |
| ar-EG | ١٠ نوفمبر ٢٠٢٦ | ١٠‏/١١‏/٢٠٢٦ | Saturday | H12 |

## Source and attribution

Data is a reviewed static subset of Unicode CLDR **48**, tag `release-48`, commit
[`acd6d88ae493633240e19a87a721076a8a75c310`](https://github.com/unicode-org/cldr/tree/acd6d88ae493633240e19a87a721076a8a75c310).
The data is covered by the [Unicode License V3](../LICENSE-UNICODE), separately
from this library's MIT code license. Copyright © 2004–2025 Unicode, Inc.

Source files under that commit: `common/main/{root,pl,en,en_US,en_001,en_GB,fr,es,it,de,ru,zh,hi,ar,ar_EG}.xml`,
`common/supplemental/{supplementalData,numberingSystems}.xml`.
Gregorian calendar aliases and inheritance markers are resolved into complete
tables; no source inheritance markers are retained. Week starts and preferred
clock cycles come from supplemental territory data. Names retain source casing;
narrow weekdays use standalone forms for calendar headers. Day periods use
format/abbreviated forms; fractions use the numbering system's decimal symbol.

Date patterns translate the short date and `Md`, `MMMd`, `MMMMd`, `Ed`,
`yMMMEd`, `yMMM`, `yMMMM` skeletons, plus the long textual date. Clock patterns
translate `Hm`, `Hms`, `hm`, `hms`; millisecond forms insert
`{decimal}{SSS}` immediately after seconds. Years widen to `{yyyy}` and
month/year names select standalone context. Apostrophe quoting is resolved
into literal UTF-8. Meaningful nonbreaking spaces and bidi marks use explicit
Unicode escapes in C++ sources. There is no runtime CLDR parsing or dependency.

See the [design](design/proposed/calendar_locale_design.md) for consumer formatter
and rendering responsibilities and the implementation stages.

English regions share wide months and weekday arrays from one source file.
CLDR's UK September abbreviation is `Sept`, versus US `Sep`, so abbreviated
arrays remain distinct; day periods also retain UK `am`/`pm` versus US `AM`/`PM`.
The UK inherits through `en_001` before `en`; both inherit from `root`.

All locales describe Gregorian dates. Chinese uses Simplified Han text and
Latin digits; Hindi uses Devanagari names and Latin digits. Arabic/Egypt uses
logical-order Arabic, RTL metadata, Arabic-Indic digits U+0660–U+0669, and U+066B
as the fractional separator. Arabic numeric dates retain U+200F RIGHT-TO-LEFT MARK;
Russian year suffixes and many H12 patterns retain U+202F NARROW NO-BREAK SPACE.
The examples above preserve those bytes. The library does not reverse text.

Native rendering requires suitable glyphs, Devanagari shaping/mark positioning,
and Arabic contextual shaping and bidi layout. Consumers measure and paint the
same shaped runs, isolate ASCII offsets as LTR runs, and choose an explicit
ASCII numeric LTR fallback when native rendering is unavailable. Native-script
rendering and fallback acceptance belong to consumer tests, not this data library.

## Build defaults and explicit selection

`DefaultCalendarLocale()` is an inline build selection helper. Define
`ROO_CALENDAR_LOCALE` as one of `pl_PL`, `en_US`, `en_GB`, `fr_FR`, `es_ES`,
`it_IT`, `de_DE`, `ru_RU`, `zh_Hans_CN`, `hi_IN`, `ar_EG`. Unsupported explicit
identifiers fail compilation. Keep build macros consistent across translation
units. The override wins over `ROO_LANG`; a locale never selects a timezone.

Without an override, exact language IDs map as follows:

| ROO_LANG suffix | Default |
| --- | --- |
| pl | pl-PL |
| en, en_US | en-US |
| en_GB | en-GB |
| fr | fr-FR |
| es, es_ES | es-ES |
| it | it-IT |
| de | de-DE |
| ru | ru-RU |
| zh, zh_Hans | zh-Hans-CN |
| hi | hi-IN |
| ar | ar-EG |
| All other IDs | en-US |

Unlisted regions/scripts such as `en_AU` and `zh_Hant` use the compatibility
fallback; select a supported regional override or a custom locale explicitly.
Per-consumer references take precedence over these build defaults:

```cpp
const roo_locale::CalendarLocale& polish = roo_locale::PolishPoland();
const roo_locale::CalendarLocale& british = roo_locale::EnglishUnitedKingdom();
// Pass either reference to each consumer; both coexist without global state.
```

Run `bazel test //...` for locale fixtures, every supported language mapping,
every explicit override with a conflicting language, compatibility fallbacks,
and rejection of unsupported explicit identifiers. Locale fixtures check data
contracts; consumer formatter, shaping, bidi, and fallback tests remain separate.

## Linking and resource acceptance

The optional calendar target compiles each locale into its own object. Latin
and Arabic-Indic digits have separate objects; equal English name tables share
one object. Data/function sections and linker garbage collection also remove
unreferenced regional English arrays. The language-only target contains no
calendar header or source. Each consumer borrows one pointer (four bytes on
ESP32); the shared locale descriptor is 52 bytes on ESP32 and 104 on x86-64.

`//:calendar_probe_language_only`, `//:calendar_probe_<underscore locale>`, and
`//:calendar_probe_all` provide thirteen minimal consumers. The normal
`//:calendar_locale_test` build also checks their final ELF symbols and payloads
through `//:calendar_link_acceptance`. It rejects extra locale accessors, tags,
native names, digit data, regional English arrays, and pattern tables, as well as
added constructors or mutable data. Host PIE builds use relocatable read-only
tables in `.data.rel.ro`; the checker allows those relocation sections.
ASAN data tests run separately from the placement gate because ASAN adds global
registration constructors and deliberately retains instrumented globals.

The standalone checker compiles the same thirteen probes as a static archive,
checks every object's constant initialization and lack of allocation/time/UI
symbols, executes host probes, and records linked sections, maps, and symbols:

```sh
python3 test/verify_calendar_linking.py \
  --output-dir .calendar_locale_validation/host
python3 test/verify_calendar_linking.py \
  --cxx "$HOME/.platformio/packages/toolchain-xtensa-esp-elf/bin/xtensa-esp32-elf-g++" \
  --esp32-ld-dir "$HOME/.platformio/packages/framework-arduinoespressif32-libs/esp32/ld" \
  --output-dir .calendar_locale_validation/esp32
```

Measured 2026-11-10 with GCC 14.2.0 on x86-64 and Espressif GCC 14.2.0
(`esp-14.2.0_20251107`) for ESP32, using the installed ESP-IDF 5.5.2 memory and
section linker scripts. Both use C++11, `-Os`, no exceptions/RTTI/PIC/PIE,
data/function sections, and section garbage collection. Figures are byte deltas
against the language-only program, including descriptor pointers, strings,
patterns, digits, accessor/probe code, and alignment. Shared strings are counted
as actually linked, so the all-locales figure is smaller than the sum of singles.

| Probe | Host read-only bytes added | ESP32 flash bytes added | ESP32 RAM bytes added |
| --- | ---: | ---: | ---: |
| language_only | 0 | 0 | 0 |
| pl_PL | 1446 | 972 | 0 |
| en_US | 1222 | 792 | 0 |
| en_GB | 1222 | 800 | 0 |
| fr_FR | 1222 | 804 | 0 |
| es_ES | 1222 | 816 | 0 |
| it_IT | 1222 | 796 | 0 |
| de_DE | 1350 | 884 | 0 |
| ru_RU | 1702 | 1188 | 0 |
| zh_Hans_CN | 1190 | 760 | 0 |
| hi_IN | 1446 | 1032 | 0 |
| ar_EG | 1222 | 840 | 0 |
| all | 10722 | 6773 | 0 |

All selected locale symbols lie in ESP32 flash address ranges: `.flash.rodata`
for data and `.flash.text` for accessors. Every non-flash allocated section stays
the same size as the language-only baseline. The SDK scripts reserve 1,024 bytes
for vectors and four BSS bytes even in the empty probe; those are baseline costs,
not locale storage. No locale object has writable data or dynamic initializers.
No single-locale link retains another locale's patterns, native names, or digits;
US and UK retain only their own abbreviation array plus equal shared names.

ESP32 probes use `main` as the entry without the startup/runtime libraries. They
validate actual SDK section placement and the library's flash/RAM delta, and are
not bootable firmware or native-script rendering acceptance. Application linker
maps remain authoritative for other SDK configurations and targets. Reports,
ELFs, symbol dumps, and maps are retained in the ignored output directories.
Bazel continues to use its stable persistent output root and global disk cache
under `/home/dawidk`, with the configured resource limits unchanged.
