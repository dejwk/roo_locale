// Calendar data derived from Unicode CLDR 48. See docs/calendar_locales.md
// and LICENSE-UNICODE for provenance and the Unicode-3.0 license.

#include "roo_locale/internal/latin_digits.h"

namespace roo_locale {
namespace internal {
namespace {

constexpr const char* kDigits[] = {
    "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
};
static_assert(sizeof(kDigits) / sizeof(kDigits[0]) == 10,
              "Calendar table cardinality");

}  // namespace

constexpr DigitSet kLatinDigits = {"latn", kDigits};

}  // namespace internal
}  // namespace roo_locale
