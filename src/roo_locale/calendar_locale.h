#pragma once

#include <stddef.h>
#include <stdint.h>

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
/// arrays and strings must remain immutable and outlive consumers. Consumers
/// validate custom data, including UTF-8, table cardinalities, enum values, and
/// pattern bounds. This value selects neither a timezone nor a formatter or
/// shaping engine.
struct CalendarLocale {
  const char* tag;  ///< Public locale tag, e.g. "pl-PL".
  const char* const*
      month_standalone_abbreviated;           ///< 12 entries, January first.
  const char* const* month_date_abbreviated;  ///< 12 entries, January first.
  const char* const* month_standalone_wide;   ///< 12 entries, January first.
  const char* const* month_date_wide;         ///< 12 entries, January first.
  const char* const* weekday_abbreviated;     ///< 7 entries, Sunday first.
  const char* const* weekday_narrow;          ///< 7 entries, Sunday first.
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

}  // namespace roo_locale
