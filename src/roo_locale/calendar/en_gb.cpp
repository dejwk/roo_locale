// Calendar data derived from Unicode CLDR 48. See docs/calendar_locales.md
// and LICENSE-UNICODE for provenance and the Unicode-3.0 license.

#include "roo_locale/calendar_locale.h"
#include "roo_locale/internal/english_names.h"
#include "roo_locale/internal/latin_digits.h"

namespace roo_locale {
namespace {

constexpr CalendarPatterns kPatterns = {{
    "{dd}/{MM}/{yyyy}",                      // kNumericDate
    "{dd}/{MM}",                             // kNumericDayMonth
    "{d} {MMM}",                             // kDayMonthAbbreviated
    "{d} {MMMM}",                            // kDayMonthWide
    "{EEE} {d}",                             // kWeekdayDay
    "{EEE}, {d} {MMM} {yyyy}",               // kWeekdayDate
    "{LLL} {yyyy}",                          // kMonthYearAbbreviated
    "{LLLL} {yyyy}",                         // kMonthYearWide
    "{d} {MMMM} {yyyy}",                     // kFullDate
    "{HH}:{mm}",                             // kH23Minutes
    "{HH}:{mm}:{ss}",                        // kH23Seconds
    "{HH}:{mm}:{ss}{decimal}{SSS}",          // kH23Millis
    "{h}:{mm}\u202f{a}",                     // kH12Minutes
    "{h}:{mm}:{ss}\u202f{a}",                // kH12Seconds
    "{h}:{mm}:{ss}{decimal}{SSS}\u202f{a}",  // kH12Millis
}};

constexpr CalendarLocale kLocale = {
    "en-GB",
    internal::kEnglishMonthsAbbreviatedGb,
    internal::kEnglishMonthsAbbreviatedGb,
    internal::kEnglishMonthsWide,
    internal::kEnglishMonthsWide,
    internal::kEnglishWeekdaysAbbreviated,
    internal::kEnglishWeekdaysNarrow,
    "am",
    "pm",
    ".",
    &kPatterns,
    &internal::kLatinDigits,
    HourCycle::kH23,
    Weekday::kMonday,
    TextDirection::kLtr,
    Script::kLatin,
};

}  // namespace

const CalendarLocale& EnglishUnitedKingdom() { return kLocale; }

}  // namespace roo_locale
