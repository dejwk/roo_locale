#include "roo_locale/calendar_locale.h"

// Verifies the build macro selects the exact regional accessor, without state.
int main() {
  return &roo_locale::DefaultCalendarLocale() ==
                 &roo_locale::ROO_TEST_CALENDAR_ACCESSOR()
             ? 0
             : 1;
}
