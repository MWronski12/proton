#include "ASTNode.h"

/*
 * Program
 *     = { Definition };
 */
struct Program : public ASTNode {
 public:
  using Definitions = std::unordered_map<Identifier, std::unique_ptr<Definition>>;

  Program(Position &&position, Definitions &&definitions)
      : ASTNode{std::move(position)}, definitions{std::move(definitions)} {}

  Definitions definitions;
};
