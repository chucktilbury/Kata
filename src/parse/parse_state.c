/**
 * @file parse_state.c
 *
 * @brief
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-29-2024
 * @copyright Copyright (c) 2024
 */
#include "util.h"
#include "parse_state.h"

ParserState* parser_state = NULL;

/**
 * @brief Set the scope object
 *
 * @param scope
 *
 */
void set_scope(ScopeType scope) {

    parser_state->scope = scope;
}

/**
 * @brief Get the scope object
 *
 * @return ScopeType
 *
 */
ScopeType get_scope() {

    return parser_state->scope;
}

