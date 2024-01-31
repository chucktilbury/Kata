/**
 * @file queue_test.c
 *
 * @brief Test the token queue. This test implements a small subset parser
 * that is contrived to verify that the token queue is working correctly. The
 * crux of the queue is that tokens can be marked as having been used and the
 * protocol for discarding tokens is working correctly. This test also serves
 * as a test bed to try out parser implementation ideas.
 *
 * Grammar:
 *
 *  module
 *      : (module_element)+
 *  module_element
 *      : scope_operator
 *      | namespace_definition
 *      | SYMBOL
 *  scope_operator
 *      : PUBLIC
 *      | PRIVATE
 *      | PROTECTED
 *  namespace_definition
 *      : NAMESPACE (SYMBOL)? namespace_body
 *  namespace_body
 *      : OCBRACE (module_element)* CCBRACE
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 01-12-2024
 * @copyright Copyright (c) 2024
 */
#include "util.h"
#include "scanner.h"

extern void print_token(Token* tok);

/**
 * @brief Parse for a complete module.
 *
 *  # One or more module items.
 *  module
 *      : (module_item)+
 */
void* module() {

    void* node;

    if(NULL == (node = module_element())) {

    }

    return node;
}

/**
 * @brief Parse for a single module element.
 *
 *  # Exactly one of these alternatives
 *  module_element
 *      : scope_operator
 *      | namespace_definition
 *      | SYMBOL
 */
void* module_element() {

}

/**
 * @brief Parse for a single keyword.
 *
 *  # A scope operator is a single keyword.
 *  scope_operator
 *      : PUBLIC
 *      | PRIVATE
 *      | PROTECTED
 */
void* scope_operator() {

}

/**
 * @brief A namespace defintion has a keyword, an optional name, and a body.
 * Note that this is contrived for the test. A real namespace requires the
 * name.
 *
 *  # Keyword with an optional name and a body
 *  namespace_definition
 *      : NAMESPACE (SYMBOL)? namespace_body
 */
void* namespace_definition() {

}

/**
 * @brief This is a recursive rule that parses for zero or more
 * module_elements.
 *
 *  # Zero or more module elements enclosed in braces.
 *  namespace_body
 *      : OCBRACE (module_element)* CCBRACE
 *
 */
void* namespace_body() {

}

int main(int argc, char** argv) {

    if(argc < 2) {
        fprintf(stderr, "Error: Need file name\n");
        return 1;
    }

    init_scanner();
    // runs from the build directory.
    open_file(argv[1]);

    // since the module is a list, this is only called once.
    module();

    return 0;
}