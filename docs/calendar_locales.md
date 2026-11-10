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
