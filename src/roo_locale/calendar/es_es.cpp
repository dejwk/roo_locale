// Calendar data derived from Unicode CLDR 48. See docs/calendar_locales.md
// and LICENSE-UNICODE for provenance and the Unicode-3.0 license.

#include "roo_locale/calendar_locale.h"
#include "roo_locale/internal/latin_digits.h"

namespace roo_locale {
namespace {

constexpr const char* kMonthsStandaloneAbbreviated[] = {
    "ene", "feb", "mar",  "abr", "may", "jun",
    "jul", "ago", "sept", "oct", "nov", "dic",
};
static_assert(sizeof(kMonthsStandaloneAbbreviated) /
                      sizeof(kMonthsStandaloneAbbreviated[0]) ==
                  12,
              "Calendar table cardinality");

constexpr const char* kMonthsStandaloneWide[] = {
    "enero", "febrero", "marzo",      "abril",   "mayo",      "junio",
    "julio", "agosto",  "septiembre", "octubre", "noviembre", "diciembre",
};
static_assert(sizeof(kMonthsStandaloneWide) /
                      sizeof(kMonthsStandaloneWide[0]) ==
                  12,
              "Calendar table cardinality");

constexpr const char* kWeekdaysAbbreviated[] = {
    "dom", "lun", "mar", "mié", "jue", "vie", "sáb",
};
static_assert(sizeof(kWeekdaysAbbreviated) / sizeof(kWeekdaysAbbreviated[0]) ==
                  7,
              "Calendar table cardinality");

constexpr const char* kWeekdaysNarrow[] = {
    "D", "L", "M", "X", "J", "V", "S",
};
static_assert(sizeof(kWeekdaysNarrow) / sizeof(kWeekdaysNarrow[0]) == 7,
              "Calendar table cardinality");

constexpr CalendarPatterns kPatterns = {{
    "{d}/{M}/{yyyy}",                        // kNumericDate
    "{d}/{M}",                               // kNumericDayMonth
    "{d} {MMM}",                             // kDayMonthAbbreviated
    "{d} de {MMMM}",                         // kDayMonthWide
    "{EEE} {d}",                             // kWeekdayDay
    "{EEE}, {d} {MMM} {yyyy}",               // kWeekdayDate
    "{LLL} {yyyy}",                          // kMonthYearAbbreviated
    "{LLLL} de {yyyy}",                      // kMonthYearWide
    "{d} de {MMMM} de {yyyy}",               // kFullDate
    "{H}:{mm}",                              // kH23Minutes
    "{H}:{mm}:{ss}",                         // kH23Seconds
    "{H}:{mm}:{ss}{decimal}{SSS}",           // kH23Millis
    "{h}:{mm}\u202f{a}",                     // kH12Minutes
    "{h}:{mm}:{ss}\u202f{a}",                // kH12Seconds
    "{h}:{mm}:{ss}{decimal}{SSS}\u202f{a}",  // kH12Millis
}};

constexpr CalendarLocale kLocale = {
    "es-ES",
    kMonthsStandaloneAbbreviated,
    kMonthsStandaloneAbbreviated,
    kMonthsStandaloneWide,
    kMonthsStandaloneWide,
    kWeekdaysAbbreviated,
    kWeekdaysNarrow,
    "a.\u202fm.",
    "p.\u202fm.",
    ",",
    &kPatterns,
    &internal::kLatinDigits,
    HourCycle::kH23,
    Weekday::kMonday,
    TextDirection::kLtr,
    Script::kLatin,
};

}  // namespace

const CalendarLocale& SpanishSpain() { return kLocale; }

}  // namespace roo_locale
