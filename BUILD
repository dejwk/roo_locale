load("@rules_cc//cc:cc_library.bzl", "cc_library")
load("@rules_cc//cc:cc_test.bzl", "cc_test")
load(":test/calendar_link_probes.bzl", "calendar_link_probes")
load(":test/calendar_selection_tests.bzl", "calendar_selection_tests")

cc_library(
    name = "roo_locale",
    hdrs = [
        "src/roo_locale.h",
        "src/roo_locale/languages.h",
    ],
    includes = ["src"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "calendar_locale",
    srcs = glob(["src/roo_locale/**/*.cpp"]),
    hdrs = glob(["src/roo_locale/internal/*.h"]) + ["src/roo_locale/calendar_locale.h"],
    copts = [
        "-fdata-sections",
        "-ffunction-sections",
    ],
    includes = ["src"],
    visibility = ["//visibility:public"],
    deps = [":roo_locale"],
)

# ASAN global registration changes section retention and initialization.
# Validate data with ASAN; measure link placement in the normal configuration.
config_setting(
    name = "address_sanitizer",
    values = {"copt": "-fsanitize=address"},
)

cc_test(
    name = "calendar_locale_test",
    size = "small",
    srcs = ["test/calendar_locale_test.cpp"],
    data = [":invalid_calendar_override"] + select({
        ":address_sanitizer": [],
        "//conditions:default": [":calendar_link_acceptance"],
    }),
    linkstatic = True,
    deps = [
        ":calendar_locale",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "language_only_test",
    size = "small",
    srcs = ["test/language_only.cpp"],
    deps = [":roo_locale"],
)

calendar_selection_tests()

calendar_link_probes()
