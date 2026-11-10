// Calendar data derived from Unicode CLDR 48. See docs/calendar_locales.md
// and LICENSE-UNICODE for provenance and the Unicode-3.0 license.

#include "roo_locale/calendar_locale.h"
#include "roo_locale/internal/arabic_digits.h"

namespace roo_locale {
namespace {

constexpr const char* kMonthsStandaloneAbbreviated[] = {
    "يناير", "فبراير", "مارس",   "أبريل",  "مايو",   "يونيو",
    "يوليو", "أغسطس",  "سبتمبر", "أكتوبر", "نوفمبر", "ديسمبر",
};
static_assert(sizeof(kMonthsStandaloneAbbreviated) /
                      sizeof(kMonthsStandaloneAbbreviated[0]) ==
                  12,
              "Calendar table cardinality");

constexpr const char* kWeekdaysAbbreviated[] = {
    "الأحد", "الاثنين", "الثلاثاء", "الأربعاء", "الخميس", "الجمعة", "السبت",
};
static_assert(sizeof(kWeekdaysAbbreviated) / sizeof(kWeekdaysAbbreviated[0]) ==
                  7,
              "Calendar table cardinality");

constexpr const char* kWeekdaysNarrow[] = {
    "ح", "ن", "ث", "ر", "خ", "ج", "س",
};
static_assert(sizeof(kWeekdaysNarrow) / sizeof(kWeekdaysNarrow[0]) == 7,
              "Calendar table cardinality");

constexpr CalendarPatterns kPatterns = {{
    "{d}\u200f/{M}\u200f/{yyyy}",       // kNumericDate
    "{d}\u200f/{M}",                    // kNumericDayMonth
    "{d} {MMM}",                        // kDayMonthAbbreviated
    "{d} {MMMM}",                       // kDayMonthWide
    "{EEE}، {d}",                       // kWeekdayDay
    "{EEE}، {d} {MMM} {yyyy}",          // kWeekdayDate
    "{LLL} {yyyy}",                     // kMonthYearAbbreviated
    "{LLLL} {yyyy}",                    // kMonthYearWide
    "{d} {MMMM} {yyyy}",                // kFullDate
    "{HH}:{mm}",                        // kH23Minutes
    "{HH}:{mm}:{ss}",                   // kH23Seconds
    "{HH}:{mm}:{ss}{decimal}{SSS}",     // kH23Millis
    "{h}:{mm} {a}",                     // kH12Minutes
    "{h}:{mm}:{ss} {a}",                // kH12Seconds
    "{h}:{mm}:{ss}{decimal}{SSS} {a}",  // kH12Millis
}};

constexpr CalendarLocale kLocale = {
    "ar-EG",
    kMonthsStandaloneAbbreviated,
    kMonthsStandaloneAbbreviated,
    kMonthsStandaloneAbbreviated,
    kMonthsStandaloneAbbreviated,
    kWeekdaysAbbreviated,
    kWeekdaysNarrow,
    "ص",
    "م",
    "٫",
    &kPatterns,
    &internal::kArabicDigits,
    HourCycle::kH12,
    Weekday::kSaturday,
    TextDirection::kRtl,
    Script::kArabic,
};

}  // namespace

const CalendarLocale& ArabicEgypt() { return kLocale; }

}  // namespace roo_locale
