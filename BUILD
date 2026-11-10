load(":test/calendar_selection_tests.bzl", "calendar_selection_tests")
load("@rules_cc//cc:cc_library.bzl", "cc_library")
load("@rules_cc//cc:cc_test.bzl", "cc_test")

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
    includes = ["src"],
    visibility = ["//visibility:public"],
    deps = [":roo_locale"],
)

cc_test(
    name = "calendar_locale_test",
    size = "small",
    srcs = ["test/calendar_locale_test.cpp"],
    linkstatic = True,
    data = [":invalid_calendar_override"],
    deps = [":calendar_locale", "@googletest//:gtest_main"],
)

cc_test(
    name = "language_only_test",
    size = "small",
    srcs = ["test/language_only.cpp"],
    deps = [":roo_locale"],
)

calendar_selection_tests()
