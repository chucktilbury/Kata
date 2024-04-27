
add_custom_target(parse_test
    COMMENT "Run automated parser tests"
    COMMAND python ${PROJECT_SOURCE_DIR}/test/scripts/run_tests.py
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/test/parse
)

