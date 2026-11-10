# Shared calendar locale data

Status: **Implemented, L1–L7 complete (2026-11-10).** Originally written against
`roo_locale` commit `9dd1072`. See the [locale guide](../../calendar_locales.md)
for usage, pinned CLDR revision, and measured host/ESP32 resource acceptance.

Implementation note: CLDR 48 uses UK `Sept` and US `Sep`, so regional English
abbreviation arrays remain distinct while equal wide-month and weekday arrays
share storage. Native-script rendering acceptance remains with consumers.

## Objective

Provide compact, explicit calendar locale data shared by date pickers, chart
axes, and other date/time presentation code.

## Motivation

A chart needs weekday and month labels, while a date picker already owns
separate calendar strings. Copying those strings into each widget creates
inconsistent abbreviations and regional conventions. One month-name array also
cannot express Polish standalone `listopad` and a date such as `10 listopada`.

## Background

[roo_locale.h](../../../src/roo_locale.h) currently supplies the compile-time
`ROO_LANG` default using [language identifiers](../../../src/roo_locale/languages.h).
The library has no locale tables or runtime dependency on time or UI libraries.

The existing
[DatePickerStrings](../../../../roo_windows/src/roo_windows/material3/date_picker/date_picker_types.h)
contains widget text, month names, narrow weekdays, and first-weekday selection.
Its implementation has English and Polish build-language tables. The English
numeric date convention is currently US-style.

A language identifies text vocabulary; a regional locale also selects
conventions. English alone does not determine Sunday versus Monday week start
or month/day versus day/month ordering.

Related proposals:

- [Calendar intervals](../../../../roo_time/docs/design/proposed/calendar_intervals_design.md)
  owns arithmetic, timezone spans, and boundary enumeration.
- [Chart time axis and execution coordination](../../../../../Projects/smarthome_cwucontrol/frontend/docs/design/proposed/time_axis_design.md)
  owns localized formatting recipes, label measurement, and app selection.

This design follows the
[shared design-authoring instructions](../../../../roo-registry/template/push/.github/instructions/general-design-authoring-instructions.md),
since this repository has no local design-authoring instruction file.

## Requirements

1. Share immutable calendar names and regional conventions without depending
   on `roo_time`, `roo_windows`, an operating-system locale, or a global mutable
   locale selection.
2. Support Polish/Poland, US and UK English, French/France, Spanish/Spain,
   Italian/Italy, German/Germany, Russian/Russia, Simplified Chinese/China,
   Hindi/India, and Arabic/Egypt, including grammatical forms, date patterns,
   numbering systems, text direction, week starts, and clock-cycle defaults.
3. Let callers select different locales in one process and provide custom
   tables with explicit lifetimes.
4. Preserve existing `ROO_LANG` behavior and the lightweight language-only
   include path. Unreferenced locale data must not be forced into firmware.
5. Keep strings in shared constant storage; avoid per-widget string copies,
   dynamic initialization, allocation, and runtime locale-data parsing.
6. Supply enough semantic formatting data for context-dependent date labels,
   including standalone months, dates containing a month, period captions, and
   localized clock readings. Keep formatting and text shaping outside this
   dependency-free data library.

## Design Overview

A **CalendarLocale** is an immutable aggregate of pointers to calendar-name
tables, finite date/time patterns, digit symbols, and regional-convention values.
It is data, not a formatter or a timezone. Built-in objects have program lifetime;
custom objects and their referenced strings must outlive every consumer.

A **month-name context** selects standalone names for a month alone or with a
year, versus names used with a day of the month. Each context has abbreviated
and wide forms. Separate pointers allow equal tables to share storage.

**Calendar patterns** are shared strings describing field order, grammatical
name context, padding, and literal text for a fixed set of common date/time
forms. A **digit set** contains ten UTF-8 digit strings; it is independent of
the script used for weekday/month names.

The consumer asks a context-dependent formatter for a semantic presentation
such as "month and year". The formatter selects the appropriate locale pattern
and name context. The planner chooses which information is needed; the
formatter expresses it in the selected locale. This keeps chart-specific
layout policy and `roo_time::DateTime` dependencies outside the data library.

For example, a consumer displaying November uses the standalone wide form
`listopad`; one displaying the tenth of November uses the date-context wide
form `listopada`. Switching the same consumer to `en-US` changes the textual
order to month then day without changing the represented date or timezone.

The shared tables satisfy consistency and grammar requirements; explicit
references satisfy per-consumer selection; separate translation units and
opt-in headers keep storage proportional to the linked locales.

## Design Details

### Locale scope and conventions

All eleven initial locales format Gregorian dates. Chinese does not switch to
a lunisolar calendar, Hindi to an Indian national calendar, or Arabic to a Hijri
calendar. Calendar selection is separate from language, digits, and direction.

| Tag | Default region/script | Numeric date order | Week starts | Default clock | Digits / direction |
| --- | --- | --- | --- | --- | --- |
| `pl-PL` | Poland / Latin | Day, month, year | Monday | 00–23 | `latn` / LTR |
| `en-US` | US / Latin | Month, day, year | Sunday | 1–12 with day period | `latn` / LTR |
| `en-GB` | UK / Latin | Day, month, year | Monday | 00–23 | `latn` / LTR |
| `fr-FR` | France / Latin | Day, month, year | Monday | 00–23 | `latn` / LTR |
| `es-ES` | Spain / Latin | Day, month, year | Monday | 00–23 | `latn` / LTR |
| `it-IT` | Italy / Latin | Day, month, year | Monday | 00–23 | `latn` / LTR |
| `de-DE` | Germany / Latin | Day, month, year | Monday | 00–23 | `latn` / LTR |
| `ru-RU` | Russia / Cyrillic | Day, month, year | Monday | 00–23 | `latn` / LTR |
| `zh-Hans-CN` | China / Simplified Han | Year, month, day | Monday | 00–23 | `latn` / LTR |
| `hi-IN` | India / Devanagari | Day, month, year | Sunday | 1–12 with day period | `latn` / LTR |
| `ar-EG` | Egypt / Arabic | Day, month, year | Saturday | 1–12 with day period | `arab` / RTL |

These are explicit defaults for the requested languages, not substitutes for
every regional variant. Chinese uses the written Simplified Chinese locale;
the spoken-language label "Mandarin" does not define a separate date format.
Hindi's CLDR default numbering is `latn` despite its Devanagari text. Arabic/Egypt
uses Arabic-Indic digits (`arab`), not Extended Arabic-Indic (`arabext`).

Date ordering and punctuation live in patterns rather than one date-order enum
and a separator. Examples of full textual dates are `10 listopada 2026`,
`10 de noviembre de 2026`, `10. November 2026`, `10 ноября 2026 г.`, and
`2026年11月10日`. This also handles a day-period marker before a clock reading
and nonbreaking spaces. Numeric patterns use a four-digit year; their padding
and separators are fixed per locale from the pinned CLDR Gregorian patterns.
Use full-year numeric patterns rather than two-digit-year short dates.

Timezone identifiers, offsets, chart messages, and tick-spacing decisions remain
outside the locale table. A locale reference never selects a timezone.

Month arrays have twelve entries indexed by `month - 1`. Weekday arrays have
seven entries in Sunday-first order, matching the documented local enum below.
Do not use locale first-weekday settings to reorder the stored arrays.

### Context-dependent formatter contract

Locale names alone cannot produce a correct date label. The consumer-side
formatter takes a fixed-offset date/time snapshot, a semantic label request,
and a locale reference. It returns a complete bounded UTF-8 string in logical
order with its text-direction metadata. It does not read a clock or infer
fields from available pixels.

The chart planner chooses the request using zoom and surrounding calendar
context: a clock label when a context row already identifies the date, a
weekday/day within a labelled month, a standalone month within a labelled year,
or a complete date when no surrounding label supplies it. It also requests an
offset qualifier for an ambiguous local reading. The formatter selects the
appropriate pattern, grammatical month form, clock cycle, digits, and literals.
Measurement then decides whether that complete representation fits. A shorter
request is selected explicitly; the formatter never silently drops fields.

For example, a Polish month tick uses `listopad`, while a full date uses
`10 listopada 2026`; Russian similarly distinguishes `ноябрь` and `ноября`.
Chinese full-date and month/year forms place the year first. Spanish date
patterns contain the required literal words. These are different contexts,
not merely different translations of one concatenated template.

The first formatter implementation lives in the
[app's presentation adapter](../../../../../Projects/smarthome_cwucontrol/frontend/docs/design/proposed/time_axis_design.md#formatting-recipes-and-context),
using `roo_time` values and this data. Extraction into `roo_windows` is later
work. `roo_locale` implements no pattern evaluator, calendar conversion,
collision detection, or shaping engine.

### Finite pattern language

Use the fifteen semantic forms declared in `CalendarPatternId` below. Patterns
contain UTF-8 literals and only these braced tokens:

| Tokens | Meaning |
| --- | --- |
| `{yyyy}` | Four-digit year |
| `{M}`, `{MM}` | Unpadded/padded numeric month |
| `{d}`, `{dd}` | Unpadded/padded day |
| `{MMM}`, `{MMMM}` | Abbreviated/wide month in date context |
| `{LLL}`, `{LLLL}` | Abbreviated/wide standalone month |
| `{EEE}` | Abbreviated weekday |
| `{H}`, `{HH}`, `{h}` | Unpadded/padded 0–23 hour; unpadded 1–12 hour |
| `{mm}`, `{ss}`, `{SSS}` | Minute, second, three fractional digits |
| `{a}`, `{decimal}` | Locale day period and fractional separator |

Literal braces are escaped as `{{` and `}}`. Unknown tokens, unmatched braces,
and embedded NUL bytes are errors. Tokens are not recursive; inserted locale
strings are never reinterpreted as patterns. The vocabulary has no conditions,
plural rules, executable callbacks, or runtime CLDR skeleton matching.
Limit a pattern to 255 UTF-8 bytes excluding NUL and 32 substitutions; validate
these bounds when accepting a custom locale and in built-in data tests.

For example, Spanish `kFullDate` is
`{d} de {MMMM} de {yyyy}`, and Chinese is `{yyyy}年{M}月{d}日`.
Gregorian CLDR patterns are reviewed and translated to this finite vocabulary
when preparing the static data. Keep `{a}` in both H12 clock precision variants
and the millisecond variant; its position comes from the locale pattern.

### Source data and text policy

Use a checked-in, reviewed subset from CLDR **48**, with the upstream revision
and Unicode license recorded in `docs/calendar_locales.md` and accompanying
attribution. Pin data to the `release-48` source when preparing the implementation;
record its exact commit in the implementation's source note. This is a selected
stable input, not a promise to follow the latest release automatically.

Sources: [CLDR 48 date semantics](https://www.unicode.org/reports/tr35/tr35-76/tr35-dates.html),
[Polish data](https://github.com/unicode-org/cldr/blob/release-48/common/main/pl.xml),
[English data](https://github.com/unicode-org/cldr/blob/release-48/common/main/en.xml),
[French](https://github.com/unicode-org/cldr/blob/release-48/common/main/fr.xml),
[Spanish](https://github.com/unicode-org/cldr/blob/release-48/common/main/es.xml),
[Italian](https://github.com/unicode-org/cldr/blob/release-48/common/main/it.xml),
[German](https://github.com/unicode-org/cldr/blob/release-48/common/main/de.xml),
[Russian](https://github.com/unicode-org/cldr/blob/release-48/common/main/ru.xml),
[Chinese](https://github.com/unicode-org/cldr/blob/release-48/common/main/zh.xml),
[Hindi](https://github.com/unicode-org/cldr/blob/release-48/common/main/hi.xml),
[Arabic](https://github.com/unicode-org/cldr/blob/release-48/common/main/ar.xml),
[Arabic/Egypt overrides](https://github.com/unicode-org/cldr/blob/release-48/common/main/ar_EG.xml),
and the same release's regional inheritance and
[supplemental week/time data](https://github.com/unicode-org/cldr/blob/release-48/common/supplemental/supplementalData.xml).
Resolve aliases and inherited entries when preparing the tables. Do not copy
inheritance markers into C++ strings.

Store UTF-8 strings in their supplied casing. Keep standalone/date-context
forms distinct even when their pointers currently reference the same table.
Abbreviations are intentional locale entries, not a byte prefix of a full
word. Narrow weekdays are supplied for calendar headers; axis text uses the
abbreviated forms because narrow letters are not necessarily unique.

Keep Arabic and Devanagari in normal Unicode logical order, with the required
combining characters; do not store presentation-form glyphs or reverse strings.
Preserve meaningful nonbreaking spaces and bidi marks from the selected
patterns, spelling invisible characters with explicit escapes in C++ sources.
Digit strings encode individual digits, not the entire number reversed.

The data library makes no font/shaping guarantee. Native Russian and Chinese
text needs the appropriate glyph repertoire. Native Hindi needs Devanagari
shaping and mark positioning; Arabic needs contextual shaping and bidi layout.
Consumers measure the same shaped glyph runs that they paint. Isolate an ASCII
UTC offset such as `+02:00` as an LTR run within an RTL label, following
[Unicode bidi rules](https://www.unicode.org/reports/tr9/). Label direction does
not reverse the chart's chronological axis.

A renderer without the required capability selects an explicit ASCII numeric
fallback with LTR direction; it must not display unshaped letters as a successful
native label. Report fallback distinctly in tests/status. Locale-data completion
and native-script rendering acceptance are separate gates. No process-wide
`setlocale` call or shaping-engine dependency is introduced here.

### Selection and compatibility

Add the opt-in `roo_locale/calendar_locale.h` header. Leave `roo_locale.h` as
the small language-selection header. Built-in accessors return const references
without constructing runtime objects or registering locale identifiers.

`DefaultCalendarLocale()` is an inline selection helper. An explicit build
macro `ROO_CALENDAR_LOCALE` selects one of the eleven underscore identifiers:
`pl_PL`, `en_US`, `en_GB`, `fr_FR`, `es_ES`, `it_IT`, `de_DE`, `ru_RU`,
`zh_Hans_CN`, `hi_IN`, or `ar_EG`. Public locale tags use the hyphenated spellings
in the table; the underscores are C/C++ configuration identifiers.

Without an explicit calendar override, use the following exact language mapping:

| Existing language identifier | Default calendar locale |
| --- | --- |
| `ROO_LANG_pl` | `pl-PL` |
| `ROO_LANG_en`, `ROO_LANG_en_US` | `en-US` |
| `ROO_LANG_en_GB` | `en-GB` |
| `ROO_LANG_fr` | `fr-FR` |
| `ROO_LANG_es`, `ROO_LANG_es_ES` | `es-ES` |
| `ROO_LANG_it` | `it-IT` |
| `ROO_LANG_de` | `de-DE` |
| `ROO_LANG_ru` | `ru-RU` |
| `ROO_LANG_zh`, `ROO_LANG_zh_Hans` | `zh-Hans-CN` |
| `ROO_LANG_hi` | `hi-IN` |
| `ROO_LANG_ar` | `ar-EG` |
| Other identifiers | `en-US` compatibility fallback |

Do not alias `ROO_LANG_en_GB` to US conventions or `ROO_LANG_zh_Hant` to
Simplified Chinese. Unlisted regions/scripts require an explicit supported
override or a caller-owned locale. Unsupported explicit macro values are
compile errors. Build macros must be consistent across translation units.

Explicit constructor/setter references override this default per consumer.
Selecting Polish text never selects a timezone. Runtime locale settings in the
application only switch a borrowed reference and request presentation updates.

The existing date picker is a future consumer, not an implementation dependency.
Do not change the public `DatePickerStrings` layout in this work. A subsequent
roo_windows change can migrate its calendar data while preserving the custom
string-table API. That work must keep picker-specific actions and validation
messages in the picker rather than adding them here.

### Storage and build layout

Use one source file per built-in locale, plus one shared English name-table
source. US and UK objects share English name pointers. Put initialized data in
constant storage and use static initialization only. Add no vtable, registry,
heap ownership, or `std::function` to a locale value.

On a 32-bit ABI the proposed locale object has twelve pointers and four one-byte
values, approximately 52 bytes with padding. Its separately shared pattern table
has fifteen pointers (60 bytes); a digit set adds a descriptor and ten pointers
(48 bytes before digit payload). A complete set of four month-name
pointer arrays and two weekday arrays uses `4 * 12 * 4 + 2 * 7 * 4 = 248`
bytes before sharing. Budget roughly 1–3 KiB of names/pattern text per language,
including multibyte scripts; measure actual deduplicated payloads rather than
assuming that character counts equal bytes. Digit sets are shared across locales.

One widget stores a four-byte locale pointer. Name tables and strings are
shared, normally in flash on the ESP32 target. Pointer relocation and platform
placement must be confirmed by the linker map; `const` alone is not evidence
of zero RAM use on every platform.

Add an optional Bazel `//:calendar_locale` target containing the locale sources
and public header; preserve the existing `//:roo_locale` language-only target.
Separate object files ensure a caller of only `PolishPoland()` does not pull
English, Arabic, Hindi, or Chinese strings through an all-locales registry.
Arabic-Indic digit data is linked only by consumers referencing it. Arduino/PlatformIO builds
must include the new `.cpp` files through their normal library source discovery.

## Proposed API

```cpp
namespace roo_locale {

enum class Weekday : uint8_t {
  kSunday, kMonday, kTuesday, kWednesday, kThursday, kFriday, kSaturday
};
enum class HourCycle : uint8_t { kH23, kH12 };
enum class TextDirection : uint8_t { kLtr, kRtl };
enum class Script : uint8_t { kLatin, kCyrillic, kHan, kDevanagari, kArabic };

enum class CalendarPatternId : uint8_t {
  kNumericDate, kNumericDayMonth, kDayMonthAbbreviated, kDayMonthWide,
  kWeekdayDay, kWeekdayDate, kMonthYearAbbreviated, kMonthYearWide, kFullDate,
  kH23Minutes, kH23Seconds, kH23Millis,
  kH12Minutes, kH12Seconds, kH12Millis, kCount
};

struct CalendarPatterns {
  const char* values[static_cast<size_t>(CalendarPatternId::kCount)];
};

struct DigitSet {
  const char* id;                 // "latn" or "arab" for initial built-ins.
  const char* const* digits;      // Ten immutable UTF-8 entries, zero to nine.
};

struct CalendarLocale {
  const char* tag;
  const char* const* month_standalone_abbreviated;  // 12 entries each.
  const char* const* month_date_abbreviated;
  const char* const* month_standalone_wide;
  const char* const* month_date_wide;
  const char* const* weekday_abbreviated;           // 7 entries each.
  const char* const* weekday_narrow;
  const char* am;
  const char* pm;
  const char* fraction_separator;
  const CalendarPatterns* patterns;
  const DigitSet* digits;
  HourCycle hour_cycle;
  Weekday first_weekday;
  TextDirection direction;
  Script script;
};

const CalendarLocale& PolishPoland();
const CalendarLocale& EnglishUnitedKingdom();
const CalendarLocale& EnglishUnitedStates();
const CalendarLocale& FrenchFrance();
const CalendarLocale& SpanishSpain();
const CalendarLocale& ItalianItaly();
const CalendarLocale& GermanGermany();
const CalendarLocale& RussianRussia();
const CalendarLocale& ChineseSimplifiedChina();
const CalendarLocale& HindiIndia();
const CalendarLocale& ArabicEgypt();
inline const CalendarLocale& DefaultCalendarLocale();  // Build-selected body.

}  // namespace roo_locale
```

All pointers are nonnull and all string entries reference NUL-terminated valid UTF-8.
Every pattern slot is present, including both clock cycles regardless of the
locale's preferred cycle. Latin digits are U+0030–U+0039; `arab` digits are
U+0660–U+0669. Each digit entry represents one digit without a sign or separator.
Strings and arrays are immutable for the entire borrowing lifetime. Built-in
tables satisfy these conditions by construction; validate custom tables when
installing them in a consumer. Enum values are explicit and do not depend on
the representation of `roo_time` enums. Consumers convert weekdays by a
documented switch or checked numeric mapping.

Consumer example: `const auto& locale = roo_locale::PolishPoland();`, then
borrow `locale` in the axis options and map `locale.first_weekday` into its
calendar interval. The formatter receives a `roo_time::DateTime` separately.

There are no placeholder data accessors: each accessor is introduced together
with its complete validated table. The selection helper lands only after its
eleven possible built-ins exist.

## Implementation Plan

Authoring reference: use the registry's
[shared C++ instructions](../../../../roo-registry/template/push/.github/instructions/general-cpp-code-authoring-instructions.md),
since this repository has no local copy. Follow the
[shared design instructions](../../../../roo-registry/template/push/.github/instructions/general-design-authoring-instructions.md)
for updates to this proposal.

### L1 — Calendar data model and Polish locale

Commit: `feat(locale): add explicit calendar data and Polish locale`.

Add the header, enums, pattern/digit data model, Polish table, optional build target, and host test setup
using the repository's normal rules_cc conventions. Include the pinned-source
and Unicode attribution note plus a short consumer example. Validate every
table entry as UTF-8, pattern token vocabulary/required fields, digit cardinality,
semantic month pairs, weekday indexing, and Polish conventions.
Gate: language-only callers compile unchanged and the
new table tests pass without a dependency on time or UI libraries.

### L2 — English regional locales

Commit: `feat(locale): add US and UK calendar locale data`.

Add shared English names and separate UK/US pattern/convention objects. Test
numeric ordering, week starts, H12/H23 patterns, and the corresponding accessors.
Document both locales with examples. Gate: the two English regions retain
distinct conventions while sharing equal name data.

### L3 — French, Spanish, Italian, and German

Commit: `feat(locale): add French Spanish Italian and German calendars`.

Add the four complete locale objects and patterns, with reviewed CLDR provenance
and examples. Cover accents, abbreviations, full textual literals, numeric
conventions, and clock patterns. Gate: fixtures validate each complete locale,
including Spanish connecting words and German day punctuation.

### L4 — Russian calendar data

Commit: `feat(locale): add Russian calendar forms and patterns`.

Add Cyrillic names, standalone/date month forms, date patterns, and metadata.
Document complete example dates. Gate: nominative/genitive fixtures differ
where required, UTF-8 is valid, and year literals are retained correctly.

### L5 — Chinese, Hindi, and Arabic data

Commit: `feat(locale): add Chinese Hindi and Arabic calendar defaults`.

Add the three complete objects, native-script names, pattern tables, metadata,
and shared Arabic-Indic digits. Include source notes and examples distinguishing
Gregorian locale data from calendar-system selection. Gate: Chinese year-first
patterns and day-period placement, Hindi Devanagari text with default Latin
digits/Sunday week start, and Arabic logical order, RTL metadata, Arabic-Indic
digits/Saturday week start pass exact fixtures. Numeric fallback behavior belongs
to consumer tests; it is not used to replace missing native locale data.

### L6 — Default selection and explicit overrides

Commit: `feat(locale): select calendar defaults for supported languages`.

Add the inline selection helper only after all eleven built-ins exist. Compile
tests for every explicit selector, every language mapping above, unsupported
explicit identifiers, and representative compatibility fallbacks. Include an
example selecting multiple locales in one program. Gate: `ROO_LANG_en_GB` maps
to UK conventions, explicit overrides win, and selection has no mutable global
state or timezone effect.

### L7 — Linking and resource acceptance

Commit: `test(locale): verify locale data placement and selective linking`.

Add minimal consumers for language-only, each single locale, and all eleven
locales (thirteen probes). Measure linked sections and symbol maps on host and
ESP32. Gate: no dynamic initializers or allocation, no time/UI dependency, no
unused language/digit/pattern payload in a single-locale link, and a 32-bit CalendarLocale at most 64
bytes. Report flash/RAM deltas in the locale guide; fix object grouping when
unused tables are retained. This validates the selected source organization
rather than deferring it to later optimization.

## Testing Plan

Add `//:calendar_locale_test`, build-configuration compile tests, and the thirteen
link probes. Test all eleven locales, pattern grammar, grammatical contexts,
digit sets, script/direction metadata, defaults, independence of consumer
selections, and resource placement. Include byte-exact fixtures for nonbreaking
spaces and bidi marks rather than relying on source-editor appearance.
Consumer formatter and rendering tests validate field substitution, shaping,
bidi isolation, and explicit fallback separately. Locale-data tests do not
claim that a device font/rendering pipeline supports those scripts.

Serialize Bazel build/test/run commands and preserve the global cache/resource
configuration. All Bazel roots/caches/sandboxes stay on persistent storage under
`/home/dawidk`, never `/tmp`.

## Caveats

### Rejected Alternatives

#### Keep all names in individual widgets

This permits isolated customization but duplicates calendar grammar and
regional conventions. Borrowed shared data preserves customization with one
authoritative table.

#### Use operating-system locales or a complete CLDR runtime

They offer much broader formatting coverage, but introduce platform availability,
state, code-size, and data-size costs beyond these embedded consumers. A pinned
subset meets the stated initial locales; it is not advertised as full CLDR.

#### Make roo_time depend on locale data

Arithmetic consumers would inherit presentation concerns. The app's formatting
adapter consumes both libraries while each remains independent.

#### Infer a regional locale or timezone from language alone

Language alone is insufficient. The legacy default mapping is a documented
compatibility fallback; explicit regional references are the normal new API.

#### Keep only date-order enums and separators

This works for a few numeric dates but cannot express Chinese year-first
textual dates, Spanish literals, or Russian year suffixes. Finite semantic
patterns cover the selected forms while leaving evaluation with the consumer.

Public table growth changes aggregate layout. Introduce fields only for the
listed consumers and rebuild dependents after API updates. Keep custom tables
immutable; mutating shared strings would invalidate measurement caches without
notification.

## Future Work

Migrate the date picker to consume the shared calendar data with an explicit
compatibility design. Add further regions/scripts such as Traditional Chinese,
additional numbering systems such as optional Devanagari digits, and other
calendars with corresponding formatter/font support. The eleven listed defaults,
their patterns, and RTL/script metadata are required initial scope.
Timezone display names, general message translation, and non-Gregorian calendars
are outside this data model's initial scope.
