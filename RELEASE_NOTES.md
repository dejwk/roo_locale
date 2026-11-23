# roo_locale 1.2.0

- Added `CalendarLocale` with Unicode CLDR 48 Gregorian data for 11 locales: `pl-PL`, `en-US`, `en-GB`, `fr-FR`, `es-ES`, `it-IT`, `de-DE`, `ru-RU`, `zh-Hans-CN`, `hi-IN`, and `ar-EG`. Each includes localized month and weekday names, date and time patterns, digits, week-start and clock preferences, and script and text-direction metadata.
- Choose a locale per component or use `DefaultCalendarLocale()`. The `ROO_CALENDAR_LOCALE` build override takes precedence over `ROO_LANG` and rejects unsupported IDs at compile time. With no override, unmapped languages fall back to `en-US`.
- Added the optional Bazel `calendar_locale` target and selective-linking checks. ESP32 link probes measured 760–1,188 bytes of flash per locale and zero additional RAM.
- Added a calendar locale guide and the Arduino `CalendarLocaleDemo` sketch, and included both in the PlatformIO export.
- Updated hosted CI to `roo_today` 2.7.0, enabled host tests, and added GoogleTest `1.18.0.bcr.1` as a Bazel development dependency.

---

# roo_locale 1.1.4

- Updated the Bazel `rules_cc` dependency from 0.2.17 to 0.2.25.
- Added consolidated release notes for previous releases.

---

# [roo_locale 1.1.3](https://github.com/dejwk/roo_locale/releases/tag/1.1.3)

Published 2026-08-30.

This release modernizes the project’s Bazel and GitHub Actions setup.

### Changed

- Updated GitHub Actions CI to use the shared `roo_testing` client workflow.
- CI now runs on pushes, pull requests, and manual dispatches, with concurrency control to cancel superseded runs.
- Pinned the project to Bazel 9.2.0.
- Centralized AddressSanitizer configuration in a vendored shared Bazel fragment; use `--config=asan` with Bazel commands.
- Bumped package metadata and Bazel module version to `1.1.3`.

No public API or locale-data changes are included in this release.

---

# [roo_locale 1.1.2](https://github.com/dejwk/roo_locale/releases/tag/1.1.2)

Published 2026-02-26.

* Fixing bazel-related build issues in test.
* Added doxygen comments.

**Full Changelog**: https://github.com/dejwk/roo_locale/compare/1.1.1...1.1.2

---

# [roo_locale 1.1.1](https://github.com/dejwk/roo_locale/releases/tag/1.1.1)

Published 2025-11-01.

Basic CI, .gitignore.

---

# [roo_locale 1.1.0](https://github.com/dejwk/roo_locale/releases/tag/1.1.0)

Published 2025-10-19.

Initial release.

---

