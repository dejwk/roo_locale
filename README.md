# roo_locale

Compile-time language IDs and compact Gregorian calendar data for embedded C++.
The eleven built-in locales provide month and weekday names, date and time
patterns, digits, the preferred clock cycle, and the first day of the week.

## Install and use

- In the **Arduino IDE**, install `roo_locale` from Library Manager.
- For **PlatformIO**, add `roo_locale` to `lib_deps` in `platformio.ini`.
- For **Bazel**, add a `bazel_dep` for `roo_locale` from the
  [Roo registry](https://github.com/dejwk/roo-registry) and link
  `@roo_locale//:calendar_locale`. Projects that use only `ROO_LANG` and the
  `roo_locale.h` header can link `@roo_locale//:roo_locale`.

```cpp
#include "roo_locale/calendar_locale.h"

const auto& locale = roo_locale::PolishPoland();
const char* heading = locale.month_standalone_wide[10];  // listopad
const char* in_date = locale.month_date_wide[10];         // listopada
```

Use an explicit accessor to choose a locale per component. Use
`DefaultCalendarLocale()` to choose one at build time via `ROO_LANG` or
`-DROO_CALENDAR_LOCALE=en_GB`. The explicit build override takes precedence.

Try the [CalendarLocaleDemo sketch](examples/CalendarLocaleDemo/CalendarLocaleDemo.ino)
and open Serial Monitor at 115200 baud. The
[calendar guide](docs/calendar_locales.md) covers locale selection, indexing,
and the pattern vocabulary. Your application evaluates patterns and renders
the resulting text.

The code is [MIT licensed](LICENSE). The calendar data comes from Unicode
CLDR 48 under the [Unicode License V3](LICENSE-UNICODE).
