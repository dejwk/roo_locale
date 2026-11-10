// Calendar data derived from Unicode CLDR 48. See docs/calendar_locales.md
// and LICENSE-UNICODE for provenance and the Unicode-3.0 license.

#include "roo_locale/calendar_locale.h"
#include "roo_locale/internal/latin_digits.h"

namespace roo_locale {
namespace {

constexpr const char* kMonthsStandaloneAbbreviated[] = {
    "जन॰", "फ़र॰", "मार्च", "अप्रैल", "मई",  "जून",
    "जुल॰", "अग॰", "सित॰", "अक्टू॰", "नव॰", "दिस॰",
};
static_assert(sizeof(kMonthsStandaloneAbbreviated) /
                      sizeof(kMonthsStandaloneAbbreviated[0]) ==
                  12,
              "Calendar table cardinality");

constexpr const char* kMonthsStandaloneWide[] = {
    "जनवरी", "फ़रवरी", "मार्च",  "अप्रैल",  "मई",   "जून",
    "जुलाई",  "अगस्त",  "सितंबर", "अक्टूबर", "नवंबर", "दिसंबर",
};
static_assert(sizeof(kMonthsStandaloneWide) /
                      sizeof(kMonthsStandaloneWide[0]) ==
                  12,
              "Calendar table cardinality");

constexpr const char* kWeekdaysAbbreviated[] = {
    "रवि", "सोम", "मंगल", "बुध", "गुरु", "शुक्र", "शनि",
};
static_assert(sizeof(kWeekdaysAbbreviated) / sizeof(kWeekdaysAbbreviated[0]) ==
                  7,
              "Calendar table cardinality");

constexpr const char* kWeekdaysNarrow[] = {
    "र", "सो", "मं", "बु", "गु", "शु", "श",
};
static_assert(sizeof(kWeekdaysNarrow) / sizeof(kWeekdaysNarrow[0]) == 7,
              "Calendar table cardinality");

constexpr CalendarPatterns kPatterns = {{
    "{d}/{M}/{yyyy}",                   // kNumericDate
    "{d}/{M}",                          // kNumericDayMonth
    "{d} {MMM}",                        // kDayMonthAbbreviated
    "{d} {MMMM}",                       // kDayMonthWide
    "{EEE} {d}",                        // kWeekdayDay
    "{EEE}, {d} {MMM} {yyyy}",          // kWeekdayDate
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
    "hi-IN",
    kMonthsStandaloneAbbreviated,
    kMonthsStandaloneAbbreviated,
    kMonthsStandaloneWide,
    kMonthsStandaloneWide,
    kWeekdaysAbbreviated,
    kWeekdaysNarrow,
    "am",
    "pm",
    ".",
    &kPatterns,
    &internal::kLatinDigits,
    HourCycle::kH12,
    Weekday::kSunday,
    TextDirection::kLtr,
    Script::kDevanagari,
};

}  // namespace

const CalendarLocale& HindiIndia() { return kLocale; }

}  // namespace roo_locale
