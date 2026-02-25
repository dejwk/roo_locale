#pragma once

#include "roo_locale/languages.h"

/// Compile-time default language code used by localization helpers.
///
/// Define `ROO_LANG` before including this header to override the default.
#ifndef ROO_LANG
#define ROO_LANG ROO_LANG_en
#endif
