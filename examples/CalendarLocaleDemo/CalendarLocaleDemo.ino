// Open Serial Monitor at 115200 baud to see both calendar locales.
#include <Arduino.h>

#include "roo_locale/calendar_locale.h"

static void PrintCalendarLocale(const char* label,
                                const roo_locale::CalendarLocale& locale) {
  constexpr unsigned kNovember = 10;
  constexpr unsigned kDaysPerWeek = 7;
  const auto full_date =
      static_cast<unsigned>(roo_locale::CalendarPatternId::kFullDate);

  Serial.print(label);
  Serial.print(" (");
  Serial.print(locale.tag);
  Serial.println(")");
  Serial.print("  Standalone November: ");
  Serial.println(locale.month_standalone_wide[kNovember]);
  Serial.print("  November in a date: ");
  Serial.println(locale.month_date_wide[kNovember]);

  Serial.print("  Weekday headings:");
  const auto first_day = static_cast<unsigned>(locale.first_weekday);
  for (unsigned column = 0; column < kDaysPerWeek; ++column) {
    const unsigned day = (first_day + column) % kDaysPerWeek;
    Serial.print(' ');
    Serial.print(locale.weekday_narrow[day]);
  }
  Serial.println();

  Serial.print("  Full-date pattern: ");
  Serial.println(locale.patterns->values[full_date]);
}

void setup() { Serial.begin(115200); }

void loop() {
  PrintCalendarLocale("Build default", roo_locale::DefaultCalendarLocale());
  PrintCalendarLocale("Explicit choice", roo_locale::PolishPoland());
  Serial.println();
  delay(3000);
}
