#include "ASTNode.h"

/*
 * Program
 *     = { Definition };
 */
struct Program : public ASTNode {
 public:
  using IdentifierToDefinitionPtrMap = std::unordered_map<Identifier, std::unique_ptr<Definition>>;

  Program(Position &&position, IdentifierToDefinitionPtrMap &&definitions)
      : ASTNode{std::move(position)}, definitions{std::move(definitions)} {}

  IdentifierToDefinitionPtrMap definitions;
};
