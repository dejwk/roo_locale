// Calendar data derived from Unicode CLDR 48. See docs/calendar_locales.md
// and LICENSE-UNICODE for provenance and the Unicode-3.0 license.

#include "roo_locale/calendar_locale.h"
#include "roo_locale/internal/english_names.h"
#include "roo_locale/internal/latin_digits.h"

namespace roo_locale {
namespace {

constexpr CalendarPatterns kPatterns = {{
    "{M}/{d}/{yyyy}",                        // kNumericDate
    "{M}/{d}",                               // kNumericDayMonth
    "{MMM} {d}",                             // kDayMonthAbbreviated
    "{MMMM} {d}",                            // kDayMonthWide
    "{d} {EEE}",                             // kWeekdayDay
    "{EEE}, {MMM} {d}, {yyyy}",              // kWeekdayDate
    "{LLL} {yyyy}",                          // kMonthYearAbbreviated
    "{LLLL} {yyyy}",                         // kMonthYearWide
    "{MMMM} {d}, {yyyy}",                    // kFullDate
    "{HH}:{mm}",                             // kH23Minutes
    "{HH}:{mm}:{ss}",                        // kH23Seconds
    "{HH}:{mm}:{ss}{decimal}{SSS}",          // kH23Millis
    "{h}:{mm}\u202f{a}",                     // kH12Minutes
    "{h}:{mm}:{ss}\u202f{a}",                // kH12Seconds
    "{h}:{mm}:{ss}{decimal}{SSS}\u202f{a}",  // kH12Millis
}};

constexpr CalendarLocale kLocale = {
    "en-US",
    internal::kEnglishMonthsAbbreviatedUs,
    internal::kEnglishMonthsAbbreviatedUs,
    internal::kEnglishMonthsWide,
    internal::kEnglishMonthsWide,
    internal::kEnglishWeekdaysAbbreviated,
    internal::kEnglishWeekdaysNarrow,
    "AM",
    "PM",
    ".",
    &kPatterns,
    &internal::kLatinDigits,
    HourCycle::kH12,
    Weekday::kSunday,
    TextDirection::kLtr,
    Script::kLatin,
};

}  // namespace

const CalendarLocale& EnglishUnitedStates() { return kLocale; }

}  // namespace roo_locale
