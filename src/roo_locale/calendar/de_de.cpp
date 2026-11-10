// Calendar data derived from Unicode CLDR 48. See docs/calendar_locales.md
// and LICENSE-UNICODE for provenance and the Unicode-3.0 license.

#include "roo_locale/calendar_locale.h"
#include "roo_locale/internal/latin_digits.h"

namespace roo_locale {
namespace {

constexpr const char* kMonthsStandaloneAbbreviated[] = {
    "Jan", "Feb", "Mär", "Apr", "Mai", "Jun",
    "Jul", "Aug", "Sep", "Okt", "Nov", "Dez",
};
static_assert(sizeof(kMonthsStandaloneAbbreviated) /
                      sizeof(kMonthsStandaloneAbbreviated[0]) ==
                  12,
              "Calendar table cardinality");

constexpr const char* kMonthsDateAbbreviated[] = {
    "Jan.", "Feb.", "März",  "Apr.", "Mai",  "Juni",
    "Juli", "Aug.", "Sept.", "Okt.", "Nov.", "Dez.",
};
static_assert(sizeof(kMonthsDateAbbreviated) /
                      sizeof(kMonthsDateAbbreviated[0]) ==
                  12,
              "Calendar table cardinality");

constexpr const char* kMonthsStandaloneWide[] = {
    "Januar", "Februar", "März",      "April",   "Mai",      "Juni",
    "Juli",   "August",  "September", "Oktober", "November", "Dezember",
};
static_assert(sizeof(kMonthsStandaloneWide) /
                      sizeof(kMonthsStandaloneWide[0]) ==
                  12,
              "Calendar table cardinality");

constexpr const char* kWeekdaysAbbreviated[] = {
    "So.", "Mo.", "Di.", "Mi.", "Do.", "Fr.", "Sa.",
};
static_assert(sizeof(kWeekdaysAbbreviated) / sizeof(kWeekdaysAbbreviated[0]) ==
                  7,
              "Calendar table cardinality");

constexpr const char* kWeekdaysNarrow[] = {
    "S", "M", "D", "M", "D", "F", "S",
};
static_assert(sizeof(kWeekdaysNarrow) / sizeof(kWeekdaysNarrow[0]) == 7,
              "Calendar table cardinality");

constexpr CalendarPatterns kPatterns = {{
    "{dd}.{MM}.{yyyy}",                      // kNumericDate
    "{d}.{M}.",                              // kNumericDayMonth
    "{d}. {MMM}",                            // kDayMonthAbbreviated
    "{d}. {MMMM}",                           // kDayMonthWide
    "{EEE}, {d}.",                           // kWeekdayDay
    "{EEE}, {d}. {MMM} {yyyy}",              // kWeekdayDate
    "{LLL} {yyyy}",                          // kMonthYearAbbreviated
    "{LLLL} {yyyy}",                         // kMonthYearWide
    "{d}. {MMMM} {yyyy}",                    // kFullDate
    "{HH}:{mm}",                             // kH23Minutes
    "{HH}:{mm}:{ss}",                        // kH23Seconds
    "{HH}:{mm}:{ss}{decimal}{SSS}",          // kH23Millis
    "{h}:{mm}\u202f{a}",                     // kH12Minutes
    "{h}:{mm}:{ss}\u202f{a}",                // kH12Seconds
    "{h}:{mm}:{ss}{decimal}{SSS}\u202f{a}",  // kH12Millis
}};

constexpr CalendarLocale kLocale = {
    "de-DE",
    kMonthsStandaloneAbbreviated,
    kMonthsDateAbbreviated,
    kMonthsStandaloneWide,
    kMonthsStandaloneWide,
    kWeekdaysAbbreviated,
    kWeekdaysNarrow,
    "AM",
    "PM",
    ",",
    &kPatterns,
    &internal::kLatinDigits,
    HourCycle::kH23,
    Weekday::kMonday,
    TextDirection::kLtr,
    Script::kLatin,
};

}  // namespace

const CalendarLocale& GermanGermany() { return kLocale; }

}  // namespace roo_locale
