// Calendar data derived from Unicode CLDR 48. See docs/calendar_locales.md
// and LICENSE-UNICODE for provenance and the Unicode-3.0 license.

#include "roo_locale/internal/arabic_digits.h"

namespace roo_locale {
namespace internal {
namespace {

constexpr const char* kDigits[] = {
    "٠", "١", "٢", "٣", "٤", "٥", "٦", "٧", "٨", "٩",
};
static_assert(sizeof(kDigits) / sizeof(kDigits[0]) == 10,
              "Calendar table cardinality");

}  // namespace

constexpr DigitSet kArabicDigits = {"arab", kDigits};

}  // namespace internal
}  // namespace roo_locale
