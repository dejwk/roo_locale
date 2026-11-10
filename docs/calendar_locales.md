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

## Source and attribution

Data is a reviewed static subset of Unicode CLDR **48**, tag `release-48`, commit
[`acd6d88ae493633240e19a87a721076a8a75c310`](https://github.com/unicode-org/cldr/tree/acd6d88ae493633240e19a87a721076a8a75c310).
The data is covered by the [Unicode License V3](../LICENSE-UNICODE), separately
from this library's MIT code license. Copyright © 2004–2025 Unicode, Inc.

Source files under that commit: `common/main/{root,pl}.xml`,
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
