#pragma once

#include "ASTNode.h"

/*
 * Block
 *     = "{", { Statement }, "}";
 */
struct Block : public ASTNode {};
