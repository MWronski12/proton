#include "ASTNode.h"

/*
 * Program
 *     = { Definition };
 */
struct Program : public ASTNode, public VisitableNode {
 public:
  using Definitions = std::unordered_map<Identifier, std::unique_ptr<Definition>>;

  Program(Position &&position, Definitions &&definitions)
      : ASTNode{std::move(position)}, definitions{std::move(definitions)} {}

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); };

  Definitions definitions;
};
