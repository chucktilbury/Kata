/**
 * @file except.c
 *
 * @brief
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 03-29-2024
 * @copyright Copyright (c) 2024
 */
#include "exception.h"
#include <stdio.h>

// define a home for the global state.
_ExceptionState _exception_state = { NULL, NULL, NULL, NULL, 0 };
