load("@rules_cc//cc:cc_test.bzl", "cc_test")

def calendar_selection_tests():
    """Compile and execute every supported default and explicit override."""
    cases = [
        ("default", [], "EnglishUnitedStates"),
        ("language_pl", ["ROO_LANG=ROO_LANG_pl"], "PolishPoland"),
        ("language_en", ["ROO_LANG=ROO_LANG_en"], "EnglishUnitedStates"),
        ("language_en_US", ["ROO_LANG=ROO_LANG_en_US"], "EnglishUnitedStates"),
        ("language_en_GB", ["ROO_LANG=ROO_LANG_en_GB"], "EnglishUnitedKingdom"),
        ("language_fr", ["ROO_LANG=ROO_LANG_fr"], "FrenchFrance"),
        ("language_es", ["ROO_LANG=ROO_LANG_es"], "SpanishSpain"),
        ("language_es_ES", ["ROO_LANG=ROO_LANG_es_ES"], "SpanishSpain"),
        ("language_it", ["ROO_LANG=ROO_LANG_it"], "ItalianItaly"),
        ("language_de", ["ROO_LANG=ROO_LANG_de"], "GermanGermany"),
        ("language_ru", ["ROO_LANG=ROO_LANG_ru"], "RussianRussia"),
        ("language_zh", ["ROO_LANG=ROO_LANG_zh"], "ChineseSimplifiedChina"),
        ("language_zh_Hans", ["ROO_LANG=ROO_LANG_zh_Hans"], "ChineseSimplifiedChina"),
        ("language_hi", ["ROO_LANG=ROO_LANG_hi"], "HindiIndia"),
        ("language_ar", ["ROO_LANG=ROO_LANG_ar"], "ArabicEgypt"),
        ("fallback_en_AU", ["ROO_LANG=ROO_LANG_en_AU"], "EnglishUnitedStates"),
        ("fallback_en_CA", ["ROO_LANG=ROO_LANG_en_CA"], "EnglishUnitedStates"),
        ("fallback_fr_CA", ["ROO_LANG=ROO_LANG_fr_CA"], "EnglishUnitedStates"),
        ("fallback_ar_001", ["ROO_LANG=ROO_LANG_ar_001"], "EnglishUnitedStates"),
        ("fallback_zh_Hant", ["ROO_LANG=ROO_LANG_zh_Hant"], "EnglishUnitedStates"),
        ("fallback_ja", ["ROO_LANG=ROO_LANG_ja"], "EnglishUnitedStates"),
        ("fallback_pt", ["ROO_LANG=ROO_LANG_pt"], "EnglishUnitedStates"),
        ("fallback_unknown", ["ROO_LANG=123456"], "EnglishUnitedStates"),
        ("override_pl_PL", ["ROO_LANG=ROO_LANG_en", "ROO_CALENDAR_LOCALE=pl_PL"], "PolishPoland"),
        ("override_en_US", ["ROO_LANG=ROO_LANG_pl", "ROO_CALENDAR_LOCALE=en_US"], "EnglishUnitedStates"),
        ("override_en_GB", ["ROO_LANG=ROO_LANG_en", "ROO_CALENDAR_LOCALE=en_GB"], "EnglishUnitedKingdom"),
        ("override_fr_FR", ["ROO_LANG=ROO_LANG_en", "ROO_CALENDAR_LOCALE=fr_FR"], "FrenchFrance"),
        ("override_es_ES", ["ROO_LANG=ROO_LANG_en", "ROO_CALENDAR_LOCALE=es_ES"], "SpanishSpain"),
        ("override_it_IT", ["ROO_LANG=ROO_LANG_en", "ROO_CALENDAR_LOCALE=it_IT"], "ItalianItaly"),
        ("override_de_DE", ["ROO_LANG=ROO_LANG_en", "ROO_CALENDAR_LOCALE=de_DE"], "GermanGermany"),
        ("override_ru_RU", ["ROO_LANG=ROO_LANG_en", "ROO_CALENDAR_LOCALE=ru_RU"], "RussianRussia"),
        ("override_zh_Hans_CN", ["ROO_LANG=ROO_LANG_en", "ROO_CALENDAR_LOCALE=zh_Hans_CN"], "ChineseSimplifiedChina"),
        ("override_hi_IN", ["ROO_LANG=ROO_LANG_en", "ROO_CALENDAR_LOCALE=hi_IN"], "HindiIndia"),
        ("override_ar_EG", ["ROO_LANG=ROO_LANG_en", "ROO_CALENDAR_LOCALE=ar_EG"], "ArabicEgypt"),
    ]
    for name, defines, accessor in cases:
        cc_test(
            name = "calendar_selection_" + name + "_test",
            size = "small",
            srcs = ["test/calendar_selection_test.cpp"],
            local_defines = defines + ["ROO_TEST_CALENDAR_ACCESSOR=" + accessor],
            deps = [":calendar_locale"],
        )

    native.genrule(
        name = "invalid_calendar_override",
        srcs = [
            "src/roo_locale.h",
            "src/roo_locale/languages.h",
            "src/roo_locale/calendar_locale.h",
            "test/calendar_selection_test.cpp",
        ],
        outs = ["invalid_calendar_override.ok"],
        toolchains = ["@bazel_tools//tools/cpp:current_cc_toolchain"],
        cmd = """
            for value in unsupported pl en_AU zh_Hant 0 2 en_US+0; do
            if $(CC) -x c++ -std=c++11 -fsyntax-only -Isrc \
                -DROO_CALENDAR_LOCALE=$$value -DROO_TEST_CALENDAR_ACCESSOR=PolishPoland \
                $(location test/calendar_selection_test.cpp) >$@.log 2>&1; then
                echo 'Unsupported calendar override unexpectedly compiled' >&2
                exit 1
            fi
            grep -q 'Unsupported ROO_CALENDAR_LOCALE' $@.log || { cat $@.log; exit 1; }
            done
            cp $@.log $@
        """,
    )
