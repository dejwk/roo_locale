#pragma once

#include <stddef.h>
#include <stdint.h>

#include "roo_locale.h"

namespace roo_locale {

/// Sunday-first weekday indices; independent of any time library's enums.
enum class Weekday : uint8_t {
  kSunday = 0,
  kMonday = 1,
  kTuesday = 2,
  kWednesday = 3,
  kThursday = 4,
  kFriday = 5,
  kSaturday = 6,
};

/// Preferred clock cycle: 0–23 or 1–12 with a day period.
enum class HourCycle : uint8_t { kH23 = 0, kH12 = 1 };

/// Logical text direction; consumers remain responsible for bidi layout.
enum class TextDirection : uint8_t { kLtr = 0, kRtl = 1 };

/// Native name script, for consumers selecting fonts and shaping support.
enum class Script : uint8_t {
  kLatin = 0,
  kCyrillic = 1,
  kHan = 2,
  kDevanagari = 3,
  kArabic = 4,
};

/// Semantic date and clock presentations, independent of label layout policy.
/// Month/year forms use standalone months; day/month forms use date months.
/// Every locale supplies both clock cycles at each precision.
enum class CalendarPatternId : uint8_t {
  kNumericDate = 0,
  kNumericDayMonth = 1,
  kDayMonthAbbreviated = 2,
  kDayMonthWide = 3,
  kWeekdayDay = 4,
  kWeekdayDate = 5,
  kMonthYearAbbreviated = 6,
  kMonthYearWide = 7,
  kFullDate = 8,
  kH23Minutes = 9,
  kH23Seconds = 10,
  kH23Millis = 11,
  kH12Minutes = 12,
  kH12Seconds = 13,
  kH12Millis = 14,
  kCount = 15,
};

/// Finite UTF-8 patterns indexed by CalendarPatternId, with no evaluator here.
/// Tokens: yyyy, M, MM, d, dd, MMM, MMMM, LLL, LLLL, EEE, H, HH, h,
/// mm, ss, SSS, a, decimal, each enclosed in braces. Escape literal braces
/// as {{ and }}. Each pattern has at most 255 bytes and 32 substitutions.
/// Unknown tokens, unmatched braces, and embedded NULs are invalid.
struct CalendarPatterns {
  const char* values[static_cast<size_t>(CalendarPatternId::kCount)];
};

/// Ten UTF-8 digit symbols, indexed zero through nine, in logical order.
/// All strings and the array are immutable and borrowed for the locale
/// lifetime.
struct DigitSet {
  const char* id;             ///< Numbering identifier, e.g. "latn" or "arab".
  const char* const* digits;  ///< Ten NUL-terminated, single-digit strings.
};

/// Shared Gregorian calendar names, patterns, and regional conventions.
/// Built-ins and their strings have program lifetime and constant
/// initialization. Custom aggregates borrow every pointer: all referenced
/// arrays and strings must remain immutable and outlive consumers. Every
/// pointer is nonnull; all strings are NUL-terminated valid UTF-8. Consumers
/// validate custom data, including table cardinalities, enum values, and
/// pattern bounds. This value selects neither a timezone nor a formatter or
/// shaping engine.
struct CalendarLocale {
  const char* tag;  ///< Public locale tag, e.g. "pl-PL".
  const char* const* month_standalone_abbreviated;  ///< 12 months, Jan first.
  const char* const* month_date_abbreviated;        ///< 12 months, Jan first.
  const char* const* month_standalone_wide;         ///< 12 months, Jan first.
  const char* const* month_date_wide;               ///< 12 months, Jan first.
  const char* const* weekday_abbreviated;           ///< 7 days, Sunday first.
  const char* const* weekday_narrow;                ///< 7 days, Sunday first.
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

/// Returns the program-lifetime Gregorian locale for Polish in Poland.
const CalendarLocale& PolishPoland();

/// Returns the program-lifetime Gregorian locale for en-US.
const CalendarLocale& EnglishUnitedStates();

/// Returns the program-lifetime Gregorian locale for en-GB.
const CalendarLocale& EnglishUnitedKingdom();

/// Returns the program-lifetime Gregorian locale for fr-FR.
const CalendarLocale& FrenchFrance();

/// Returns the program-lifetime Gregorian locale for es-ES.
const CalendarLocale& SpanishSpain();

/// Returns the program-lifetime Gregorian locale for it-IT.
const CalendarLocale& ItalianItaly();

/// Returns the program-lifetime Gregorian locale for de-DE.
const CalendarLocale& GermanGermany();

/// Returns the program-lifetime Gregorian locale for ru-RU.
const CalendarLocale& RussianRussia();

/// Returns the program-lifetime Gregorian locale for zh-Hans-CN.
const CalendarLocale& ChineseSimplifiedChina();

/// Returns the program-lifetime Gregorian locale for hi-IN.
const CalendarLocale& HindiIndia();

/// Returns the program-lifetime Gregorian locale for ar-EG.
const CalendarLocale& ArabicEgypt();

// Private preprocessor dispatch rejects unsupported explicit identifiers.
#ifdef ROO_CALENDAR_LOCALE
#define ROO_LOCALE_INTERNAL_CALENDAR_pl_PL_ID 1
#define ROO_LOCALE_INTERNAL_CALENDAR_en_US_ID 2
#define ROO_LOCALE_INTERNAL_CALENDAR_en_GB_ID 3
#define ROO_LOCALE_INTERNAL_CALENDAR_fr_FR_ID 4
#define ROO_LOCALE_INTERNAL_CALENDAR_es_ES_ID 5
#define ROO_LOCALE_INTERNAL_CALENDAR_it_IT_ID 6
#define ROO_LOCALE_INTERNAL_CALENDAR_de_DE_ID 7
#define ROO_LOCALE_INTERNAL_CALENDAR_ru_RU_ID 8
#define ROO_LOCALE_INTERNAL_CALENDAR_zh_Hans_CN_ID 9
#define ROO_LOCALE_INTERNAL_CALENDAR_hi_IN_ID 10
#define ROO_LOCALE_INTERNAL_CALENDAR_ar_EG_ID 11
#define ROO_LOCALE_INTERNAL_CALENDAR_ID_IMPL(value) \
  ROO_LOCALE_INTERNAL_CALENDAR_##value##_ID
#define ROO_LOCALE_INTERNAL_CALENDAR_ID(value) \
  ROO_LOCALE_INTERNAL_CALENDAR_ID_IMPL(value)
#endif

/// Returns the build-selected program-lifetime Gregorian calendar locale.
/// ROO_CALENDAR_LOCALE accepts pl_PL, en_US, en_GB, fr_FR, es_ES, it_IT, de_DE,
/// ru_RU, zh_Hans_CN, hi_IN, or ar_EG and takes precedence over ROO_LANG.
/// Without an override, exact supported ROO_LANG IDs select their documented
/// locale; other languages, regions, and scripts fall back to en-US.
/// Build macros must be consistent across translation units. Explicit consumer
/// references override this default without changing any global state.
inline const CalendarLocale& DefaultCalendarLocale() {
#ifdef ROO_CALENDAR_LOCALE
#if ROO_LOCALE_INTERNAL_CALENDAR_ID(ROO_CALENDAR_LOCALE) == 1
  return PolishPoland();
#elif ROO_LOCALE_INTERNAL_CALENDAR_ID(ROO_CALENDAR_LOCALE) == 2
  return EnglishUnitedStates();
#elif ROO_LOCALE_INTERNAL_CALENDAR_ID(ROO_CALENDAR_LOCALE) == 3
  return EnglishUnitedKingdom();
#elif ROO_LOCALE_INTERNAL_CALENDAR_ID(ROO_CALENDAR_LOCALE) == 4
  return FrenchFrance();
#elif ROO_LOCALE_INTERNAL_CALENDAR_ID(ROO_CALENDAR_LOCALE) == 5
  return SpanishSpain();
#elif ROO_LOCALE_INTERNAL_CALENDAR_ID(ROO_CALENDAR_LOCALE) == 6
  return ItalianItaly();
#elif ROO_LOCALE_INTERNAL_CALENDAR_ID(ROO_CALENDAR_LOCALE) == 7
  return GermanGermany();
#elif ROO_LOCALE_INTERNAL_CALENDAR_ID(ROO_CALENDAR_LOCALE) == 8
  return RussianRussia();
#elif ROO_LOCALE_INTERNAL_CALENDAR_ID(ROO_CALENDAR_LOCALE) == 9
  return ChineseSimplifiedChina();
#elif ROO_LOCALE_INTERNAL_CALENDAR_ID(ROO_CALENDAR_LOCALE) == 10
  return HindiIndia();
#elif ROO_LOCALE_INTERNAL_CALENDAR_ID(ROO_CALENDAR_LOCALE) == 11
  return ArabicEgypt();
#else
#error "Unsupported ROO_CALENDAR_LOCALE; use a supported underscore identifier"
#endif
#elif ROO_LANG == ROO_LANG_pl
  return PolishPoland();
#elif ROO_LANG == ROO_LANG_en || ROO_LANG == ROO_LANG_en_US
  return EnglishUnitedStates();
#elif ROO_LANG == ROO_LANG_en_GB
  return EnglishUnitedKingdom();
#elif ROO_LANG == ROO_LANG_fr
  return FrenchFrance();
#elif ROO_LANG == ROO_LANG_es || ROO_LANG == ROO_LANG_es_ES
  return SpanishSpain();
#elif ROO_LANG == ROO_LANG_it
  return ItalianItaly();
#elif ROO_LANG == ROO_LANG_de
  return GermanGermany();
#elif ROO_LANG == ROO_LANG_ru
  return RussianRussia();
#elif ROO_LANG == ROO_LANG_zh || ROO_LANG == ROO_LANG_zh_Hans
  return ChineseSimplifiedChina();
#elif ROO_LANG == ROO_LANG_hi
  return HindiIndia();
#elif ROO_LANG == ROO_LANG_ar
  return ArabicEgypt();
#else
  return EnglishUnitedStates();
#endif
}

#ifdef ROO_CALENDAR_LOCALE
#undef ROO_LOCALE_INTERNAL_CALENDAR_ID
#undef ROO_LOCALE_INTERNAL_CALENDAR_ID_IMPL
#undef ROO_LOCALE_INTERNAL_CALENDAR_pl_PL_ID
#undef ROO_LOCALE_INTERNAL_CALENDAR_en_US_ID
#undef ROO_LOCALE_INTERNAL_CALENDAR_en_GB_ID
#undef ROO_LOCALE_INTERNAL_CALENDAR_fr_FR_ID
#undef ROO_LOCALE_INTERNAL_CALENDAR_es_ES_ID
#undef ROO_LOCALE_INTERNAL_CALENDAR_it_IT_ID
#undef ROO_LOCALE_INTERNAL_CALENDAR_de_DE_ID
#undef ROO_LOCALE_INTERNAL_CALENDAR_ru_RU_ID
#undef ROO_LOCALE_INTERNAL_CALENDAR_zh_Hans_CN_ID
#undef ROO_LOCALE_INTERNAL_CALENDAR_hi_IN_ID
#undef ROO_LOCALE_INTERNAL_CALENDAR_ar_EG_ID
#endif

}  // namespace roo_locale
