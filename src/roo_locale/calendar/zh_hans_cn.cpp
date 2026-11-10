// Calendar data derived from Unicode CLDR 48. See docs/calendar_locales.md
// and LICENSE-UNICODE for provenance and the Unicode-3.0 license.

#include "roo_locale/calendar_locale.h"
#include "roo_locale/internal/latin_digits.h"

namespace roo_locale {
namespace {

constexpr const char* kMonthsStandaloneAbbreviated[] = {
    "1月", "2月", "3月", "4月",  "5月",  "6月",
    "7月", "8月", "9月", "10月", "11月", "12月",
};
static_assert(sizeof(kMonthsStandaloneAbbreviated) /
                      sizeof(kMonthsStandaloneAbbreviated[0]) ==
                  12,
              "Calendar table cardinality");

constexpr const char* kMonthsStandaloneWide[] = {
    "一月", "二月", "三月", "四月", "五月",   "六月",
    "七月", "八月", "九月", "十月", "十一月", "十二月",
};
static_assert(sizeof(kMonthsStandaloneWide) /
                      sizeof(kMonthsStandaloneWide[0]) ==
                  12,
              "Calendar table cardinality");

constexpr const char* kWeekdaysAbbreviated[] = {
    "周日", "周一", "周二", "周三", "周四", "周五", "周六",
};
static_assert(sizeof(kWeekdaysAbbreviated) / sizeof(kWeekdaysAbbreviated[0]) ==
                  7,
              "Calendar table cardinality");

constexpr const char* kWeekdaysNarrow[] = {
    "日", "一", "二", "三", "四", "五", "六",
};
static_assert(sizeof(kWeekdaysNarrow) / sizeof(kWeekdaysNarrow[0]) == 7,
              "Calendar table cardinality");

constexpr CalendarPatterns kPatterns = {{
    "{yyyy}/{M}/{d}",                  // kNumericDate
    "{M}/{d}",                         // kNumericDayMonth
    "{M}月{d}日",                      // kDayMonthAbbreviated
    "{M}月{d}日",                      // kDayMonthWide
    "{d}日{EEE}",                      // kWeekdayDay
    "{yyyy}年{M}月{d}日{EEE}",         // kWeekdayDate
    "{yyyy}年{M}月",                   // kMonthYearAbbreviated
    "{yyyy}年{M}月",                   // kMonthYearWide
    "{yyyy}年{M}月{d}日",              // kFullDate
    "{HH}:{mm}",                       // kH23Minutes
    "{HH}:{mm}:{ss}",                  // kH23Seconds
    "{HH}:{mm}:{ss}{decimal}{SSS}",    // kH23Millis
    "{a}{h}:{mm}",                     // kH12Minutes
    "{a}{h}:{mm}:{ss}",                // kH12Seconds
    "{a}{h}:{mm}:{ss}{decimal}{SSS}",  // kH12Millis
}};

constexpr CalendarLocale kLocale = {
    "zh-Hans-CN",
    kMonthsStandaloneAbbreviated,
    kMonthsStandaloneAbbreviated,
    kMonthsStandaloneWide,
    kMonthsStandaloneWide,
    kWeekdaysAbbreviated,
    kWeekdaysNarrow,
    "上午",
    "下午",
    ".",
    &kPatterns,
    &internal::kLatinDigits,
    HourCycle::kH23,
    Weekday::kMonday,
    TextDirection::kLtr,
    Script::kHan,
};

}  // namespace

const CalendarLocale& ChineseSimplifiedChina() { return kLocale; }

}  // namespace roo_locale
