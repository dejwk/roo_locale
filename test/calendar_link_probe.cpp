#include "roo_locale/calendar_locale.h"

static_assert(sizeof(void*) != 4 || sizeof(roo_locale::CalendarLocale) <= 64,
              "32-bit locale descriptor must fit the embedded budget");

// References descriptors across object boundaries so the link retains their
// data.
int main() {
#ifdef ROO_TEST_ALL_CALENDARS
  const roo_locale::CalendarLocale* locales[] = {
      &roo_locale::PolishPoland(),
      &roo_locale::EnglishUnitedStates(),
      &roo_locale::EnglishUnitedKingdom(),
      &roo_locale::FrenchFrance(),
      &roo_locale::SpanishSpain(),
      &roo_locale::ItalianItaly(),
      &roo_locale::GermanGermany(),
      &roo_locale::RussianRussia(),
      &roo_locale::ChineseSimplifiedChina(),
      &roo_locale::HindiIndia(),
      &roo_locale::ArabicEgypt(),
  };
  for (const roo_locale::CalendarLocale* locale : locales) {
    if (locale->tag[0] == '\0') return 1;
  }
  return 0;
#else
  return roo_locale::ROO_TEST_CALENDAR_ACCESSOR().tag[0] == '\0' ? 1 : 0;
#endif
}
