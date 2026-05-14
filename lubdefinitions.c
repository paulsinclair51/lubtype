/**
 * @file lubdefinitions.c
 * @brief Provide function definitions for the LUB API.
 * @copyright Copyright (c) 2026 paulsinclair51
 * SPDX-License-Identifier: MIT
 * @details Emit the extern function definitions from lubtype.h for this
 *          binary.
 * @note Compile and link this file exactly once per final binary. All other
 *       translation units should include lubtype.h without defining
 *       LUB_DEFINITIONS.
 * @note This file is intentionally minimal and should usually contain only the
 *       LUB_DEFINITIONS define and the lubtype.h include.
 */

#if !defined(LUB_DEFINITIONS)
#define LUB_DEFINITIONS
#endif

#include "lubtype.h"

/* End of lubdefinitions.c */
