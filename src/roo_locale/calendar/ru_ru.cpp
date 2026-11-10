// Calendar data derived from Unicode CLDR 48. See docs/calendar_locales.md
// and LICENSE-UNICODE for provenance and the Unicode-3.0 license.

#include "roo_locale/calendar_locale.h"
#include "roo_locale/internal/latin_digits.h"

namespace roo_locale {
namespace {

constexpr const char* kMonthsStandaloneAbbreviated[] = {
    "янв.", "февр.", "март",  "апр.", "май",   "июнь",
    "июль", "авг.",  "сент.", "окт.", "нояб.", "дек.",
};
static_assert(sizeof(kMonthsStandaloneAbbreviated) /
                      sizeof(kMonthsStandaloneAbbreviated[0]) ==
                  12,
              "Calendar table cardinality");

constexpr const char* kMonthsDateAbbreviated[] = {
    "янв.", "февр.", "мар.",  "апр.", "мая",   "июн.",
    "июл.", "авг.",  "сент.", "окт.", "нояб.", "дек.",
};
static_assert(sizeof(kMonthsDateAbbreviated) /
                      sizeof(kMonthsDateAbbreviated[0]) ==
                  12,
              "Calendar table cardinality");

constexpr const char* kMonthsStandaloneWide[] = {
    "январь", "февраль", "март",     "апрель",  "май",    "июнь",
    "июль",   "август",  "сентябрь", "октябрь", "ноябрь", "декабрь",
};
static_assert(sizeof(kMonthsStandaloneWide) /
                      sizeof(kMonthsStandaloneWide[0]) ==
                  12,
              "Calendar table cardinality");

constexpr const char* kMonthsDateWide[] = {
    "января", "февраля", "марта",    "апреля",  "мая",    "июня",
    "июля",   "августа", "сентября", "октября", "ноября", "декабря",
};
static_assert(sizeof(kMonthsDateWide) / sizeof(kMonthsDateWide[0]) == 12,
              "Calendar table cardinality");

constexpr const char* kWeekdaysAbbreviated[] = {
    "вс", "пн", "вт", "ср", "чт", "пт", "сб",
};
static_assert(sizeof(kWeekdaysAbbreviated) / sizeof(kWeekdaysAbbreviated[0]) ==
                  7,
              "Calendar table cardinality");

constexpr const char* kWeekdaysNarrow[] = {
    "В", "П", "В", "С", "Ч", "П", "С",
};
static_assert(sizeof(kWeekdaysNarrow) / sizeof(kWeekdaysNarrow[0]) == 7,
              "Calendar table cardinality");

constexpr CalendarPatterns kPatterns = {{
    "{dd}.{MM}.{yyyy}",                      // kNumericDate
    "{dd}.{MM}",                             // kNumericDayMonth
    "{d} {MMM}",                             // kDayMonthAbbreviated
    "{d} {MMMM}",                            // kDayMonthWide
    "{EEE}, {d}",                            // kWeekdayDay
    "{EEE}, {d} {MMM} {yyyy}\u202fг.",       // kWeekdayDate
    "{LLL} {yyyy}\u202fг.",                  // kMonthYearAbbreviated
    "{LLLL} {yyyy}\u202fг.",                 // kMonthYearWide
    "{d} {MMMM} {yyyy}\u202fг.",             // kFullDate
    "{HH}:{mm}",                             // kH23Minutes
    "{HH}:{mm}:{ss}",                        // kH23Seconds
    "{HH}:{mm}:{ss}{decimal}{SSS}",          // kH23Millis
    "{h}:{mm}\u202f{a}",                     // kH12Minutes
    "{h}:{mm}:{ss}\u202f{a}",                // kH12Seconds
    "{h}:{mm}:{ss}{decimal}{SSS}\u202f{a}",  // kH12Millis
}};

constexpr CalendarLocale kLocale = {
    "ru-RU",
    kMonthsStandaloneAbbreviated,
    kMonthsDateAbbreviated,
    kMonthsStandaloneWide,
    kMonthsDateWide,
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
    Script::kCyrillic,
};

}  // namespace

const CalendarLocale& RussianRussia() { return kLocale; }

}  // namespace roo_locale
