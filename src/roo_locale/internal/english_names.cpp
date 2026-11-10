// Calendar data derived from Unicode CLDR 48. See docs/calendar_locales.md
// and LICENSE-UNICODE for provenance and the Unicode-3.0 license.

#include "roo_locale/internal/english_names.h"

namespace roo_locale {
namespace internal {

const char* const kEnglishMonthsAbbreviatedUs[] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec",
};
static_assert(sizeof(kEnglishMonthsAbbreviatedUs) /
                      sizeof(kEnglishMonthsAbbreviatedUs[0]) ==
                  12,
              "Calendar table cardinality");

const char* const kEnglishMonthsAbbreviatedGb[] = {
    "Jan", "Feb", "Mar",  "Apr", "May", "Jun",
    "Jul", "Aug", "Sept", "Oct", "Nov", "Dec",
};
static_assert(sizeof(kEnglishMonthsAbbreviatedGb) /
                      sizeof(kEnglishMonthsAbbreviatedGb[0]) ==
                  12,
              "Calendar table cardinality");

const char* const kEnglishMonthsWide[] = {
    "January", "February", "March",     "April",   "May",      "June",
    "July",    "August",   "September", "October", "November", "December",
};
static_assert(sizeof(kEnglishMonthsWide) / sizeof(kEnglishMonthsWide[0]) == 12,
              "Calendar table cardinality");

const char* const kEnglishWeekdaysAbbreviated[] = {
    "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat",
};
static_assert(sizeof(kEnglishWeekdaysAbbreviated) /
                      sizeof(kEnglishWeekdaysAbbreviated[0]) ==
                  7,
              "Calendar table cardinality");

const char* const kEnglishWeekdaysNarrow[] = {
    "S", "M", "T", "W", "T", "F", "S",
};
static_assert(sizeof(kEnglishWeekdaysNarrow) /
                      sizeof(kEnglishWeekdaysNarrow[0]) ==
                  7,
              "Calendar table cardinality");

}  // namespace internal
}  // namespace roo_locale
