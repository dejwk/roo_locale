#include "roo_locale/calendar_locale.h"

#include <cstring>
#include <string>
#include <type_traits>
#include <vector>

#include "gtest/gtest.h"

namespace roo_locale {
namespace {

static_assert(std::is_trivial<CalendarLocale>::value,
              "No runtime construction");
static_assert(std::is_standard_layout<CalendarLocale>::value,
              "Borrowed aggregate");
static_assert(sizeof(void*) != 4 || sizeof(CalendarLocale) <= 64,
              "Embedded locale descriptor budget");

// Rejects overlong encodings, surrogates, truncated sequences, and non-scalars.
bool IsUtf8(const std::string& text) {
  size_t i = 0;
  while (i < text.size()) {
    const unsigned char first = text[i++];
    if (first == 0) return false;
    if (first < 0x80) continue;
    int remaining;
    uint32_t scalar;
    uint32_t minimum;
    if (first >= 0xc2 && first <= 0xdf) {
      remaining = 1;
      scalar = first & 0x1f;
      minimum = 0x80;
    } else if (first >= 0xe0 && first <= 0xef) {
      remaining = 2;
      scalar = first & 0x0f;
      minimum = 0x800;
    } else if (first >= 0xf0 && first <= 0xf4) {
      remaining = 3;
      scalar = first & 7;
      minimum = 0x10000;
    } else {
      return false;
    }
    while (remaining-- > 0) {
      if (i == text.size()) return false;
      const unsigned char next = text[i++];
      if ((next & 0xc0) != 0x80) return false;
      scalar = (scalar << 6) | (next & 0x3f);
    }
    if (scalar < minimum || scalar > 0x10ffff ||
        (scalar >= 0xd800 && scalar <= 0xdfff)) {
      return false;
    }
  }
  return true;
}

// Parses only the finite public vocabulary, never interpreting inserted names.
bool ParsePattern(const std::string& text, std::vector<std::string>* tokens) {
  const char* const vocabulary[] = {
      "yyyy", "M", "MM", "d", "dd", "MMM", "MMMM", "LLL", "LLLL",
      "EEE",  "H", "HH", "h", "mm", "ss",  "SSS",  "a",   "decimal"};
  tokens->clear();
  if (text.size() > 255 || !IsUtf8(text)) return false;
  for (size_t i = 0; i < text.size();) {
    const char c = text[i++];
    if (c != '{' && c != '}') continue;
    if (i < text.size() && text[i] == c) {
      ++i;
      continue;
    }
    if (c == '}') return false;
    const size_t end = text.find('}', i);
    if (end == std::string::npos) return false;
    const std::string token = text.substr(i, end - i);
    bool known = false;
    for (const char* entry : vocabulary) {
      if (token == entry) known = true;
    }
    if (!known || tokens->size() == 32) return false;
    tokens->push_back(token);
    i = end + 1;
  }
  return true;
}

bool Has(const std::vector<std::string>& tokens, const char* token) {
  for (const std::string& entry : tokens) {
    if (entry == token) return true;
  }
  return false;
}

const char* Pattern(const CalendarLocale& locale, CalendarPatternId id) {
  return locale.patterns->values[static_cast<size_t>(id)];
}

void ValidateText(const char* text) {
  ASSERT_NE(text, nullptr);
  EXPECT_TRUE(IsUtf8(text)) << text;
  EXPECT_EQ(std::string(text).find("↑↑↑"), std::string::npos);
}

// Verifies cardinality-readable entries, grammar, semantic fields, and digits.
void ValidateLocale(const CalendarLocale& locale) {
  ValidateText(locale.tag);
  ValidateText(locale.am);
  ValidateText(locale.pm);
  ValidateText(locale.fraction_separator);
  for (const char* const* months :
       {locale.month_standalone_abbreviated, locale.month_date_abbreviated,
        locale.month_standalone_wide, locale.month_date_wide}) {
    ASSERT_NE(months, nullptr);
    for (size_t i = 0; i < 12; ++i) ValidateText(months[i]);
  }
  for (const char* const* weekdays :
       {locale.weekday_abbreviated, locale.weekday_narrow}) {
    ASSERT_NE(weekdays, nullptr);
    for (size_t i = 0; i < 7; ++i) ValidateText(weekdays[i]);
  }
  ASSERT_NE(locale.digits, nullptr);
  ASSERT_NE(locale.digits->digits, nullptr);
  ValidateText(locale.digits->id);
  const bool arabic = std::strcmp(locale.digits->id, "arab") == 0;
  EXPECT_TRUE(arabic || std::strcmp(locale.digits->id, "latn") == 0);
  for (int i = 0; i < 10; ++i) {
    ValidateText(locale.digits->digits[i]);
    const std::string expected =
        arabic ? std::string("\xd9") + static_cast<char>(0xa0 + i)
               : std::string(1, static_cast<char>('0' + i));
    EXPECT_EQ(expected, locale.digits->digits[i]);
  }
  ASSERT_NE(locale.patterns, nullptr);
  for (size_t i = 0; i < static_cast<size_t>(CalendarPatternId::kCount); ++i) {
    SCOPED_TRACE(i);
    const char* pattern = locale.patterns->values[i];
    ValidateText(pattern);
    ASSERT_NE(pattern, nullptr);
    std::vector<std::string> tokens;
    ASSERT_TRUE(ParsePattern(pattern, &tokens));
    const bool month_number = Has(tokens, "M") || Has(tokens, "MM");
    const bool day = Has(tokens, "d") || Has(tokens, "dd");
    if (i <= 8) {
      const bool year = i == 0 || i == 5 || i == 6 || i == 7 || i == 8;
      EXPECT_EQ(Has(tokens, "yyyy"), year);
      EXPECT_EQ(day, i != 6 && i != 7);
      EXPECT_EQ(Has(tokens, "EEE"), i == 4 || i == 5);
      if (i == 0 || i == 1) {
        EXPECT_TRUE(month_number);
      }
      if (i == 2 || i == 5) {
        EXPECT_TRUE(month_number || Has(tokens, "MMM"));
      }
      if (i == 3 || i == 8) {
        EXPECT_TRUE(month_number || Has(tokens, "MMMM"));
      }
      if (i == 6) {
        EXPECT_TRUE(month_number || Has(tokens, "LLL"));
      }
      if (i == 7) {
        EXPECT_TRUE(month_number || Has(tokens, "LLLL"));
      }
      EXPECT_FALSE(Has(tokens, "a"));
    } else {
      const bool h12 = i >= 12;
      EXPECT_EQ(Has(tokens, "h"), h12);
      EXPECT_EQ(Has(tokens, "a"), h12);
      EXPECT_EQ(Has(tokens, "H") || Has(tokens, "HH"), !h12);
      EXPECT_TRUE(Has(tokens, "mm"));
      EXPECT_EQ(Has(tokens, "ss"), i != 9 && i != 12);
      EXPECT_EQ(Has(tokens, "SSS"), i == 11 || i == 14);
      EXPECT_EQ(Has(tokens, "decimal"), i == 11 || i == 14);
    }
  }
}

// Verifies test validators catch invalid input and the published byte/token
// limits.
TEST(CalendarLocaleTest, ValidationBoundaries) {
  EXPECT_FALSE(IsUtf8("\xc0\xaf"));
  EXPECT_FALSE(IsUtf8("\xed\xa0\x80"));
  EXPECT_FALSE(IsUtf8("\xf4\x90\x80\x80"));
  EXPECT_FALSE(IsUtf8("\xe2\x82"));
  std::vector<std::string> tokens;
  for (const char* invalid :
       {"{unknown}", "{yyyy", "yyyy}", "{", "}", "{M{d}}"}) {
    EXPECT_FALSE(ParsePattern(invalid, &tokens)) << invalid;
  }
  EXPECT_FALSE(ParsePattern(std::string("a\0b", 3), &tokens));
  EXPECT_TRUE(ParsePattern("{{{yyyy}}}", &tokens));
  ASSERT_EQ(tokens.size(), 1u);
  EXPECT_EQ(tokens[0], "yyyy");
  EXPECT_TRUE(ParsePattern(std::string(255, 'x'), &tokens));
  EXPECT_FALSE(ParsePattern(std::string(256, 'x'), &tokens));
  std::string repeated;
  for (int i = 0; i < 32; ++i) repeated += "{M}";
  EXPECT_TRUE(ParsePattern(repeated, &tokens));
  EXPECT_FALSE(ParsePattern(repeated + "{M}", &tokens));
}

// Verifies Polish grammatical context, Sunday-first storage, and regional
// defaults.
TEST(CalendarLocaleTest, PolishPoland) {
  const CalendarLocale& locale = PolishPoland();
  ValidateLocale(locale);
  EXPECT_STREQ(locale.tag, "pl-PL");
  EXPECT_STREQ(locale.month_standalone_wide[10], "listopad");
  EXPECT_STREQ(locale.month_date_wide[10], "listopada");
  EXPECT_STREQ(locale.month_standalone_wide[0], "styczeń");
  EXPECT_STREQ(locale.month_date_wide[0], "stycznia");
  EXPECT_STREQ(locale.month_date_abbreviated[9], "paź");
  EXPECT_EQ(locale.month_date_abbreviated, locale.month_standalone_abbreviated);
  EXPECT_STREQ(locale.weekday_abbreviated[0], "niedz.");
  EXPECT_STREQ(locale.weekday_abbreviated[6], "sob.");
  EXPECT_STREQ(locale.weekday_narrow[3], "Ś");
  EXPECT_EQ(locale.first_weekday, Weekday::kMonday);
  EXPECT_EQ(locale.hour_cycle, HourCycle::kH23);
  EXPECT_EQ(locale.script, Script::kLatin);
  EXPECT_EQ(locale.direction, TextDirection::kLtr);
  EXPECT_STREQ(Pattern(locale, CalendarPatternId::kNumericDate),
               "{d}.{MM}.{yyyy}");
  EXPECT_STREQ(Pattern(locale, CalendarPatternId::kFullDate),
               "{d} {MMMM} {yyyy}");
  EXPECT_STREQ(Pattern(locale, CalendarPatternId::kMonthYearWide),
               "{LLLL} {yyyy}");
  EXPECT_STREQ(Pattern(locale, CalendarPatternId::kH12Millis),
               "{h}:{mm}:{ss}{decimal}{SSS}\u202f{a}");
  EXPECT_STREQ(locale.fraction_separator, ",");
  EXPECT_EQ(&locale, &PolishPoland());
}

}  // namespace
}  // namespace roo_locale
