#include "StackFrame.h"

namespace {

void expect(bool condition, const std::exception& exception) {
  if (!condition) throw exception;
}

}  // namespace

namespace Interpreter {

bool StackFrame::nameConflict(const Identifier& name) const noexcept {
  if (!m_scopes.empty()) {
    return m_scopes.back().nameConflict(name);
  }
  return false;
}

void StackFrame::enterScope() noexcept { m_scopes.emplace_back(); }

void StackFrame::exitScope() {
  // First scope is argument scope, which lives as long as the frame itself
  expect(m_scopes.size() > 1, std::logic_error("Cannot exit scope, stack empty"));
  m_scopes.pop_back();
}

/* ---------------------------- Variable methods ---------------------------- */

bool StackFrame::varIsDeclared(const Identifier& name) const noexcept {
  for (auto it = m_scopes.crbegin(); it != m_scopes.crend(); ++it) {
    if (it->varIsDeclared(name)) return true;
  }
  return false;
}

bool StackFrame::varIsDefined(const Identifier& name) const noexcept {
  for (auto it = m_scopes.crbegin(); it != m_scopes.crend(); ++it) {
    if (it->varIsDefined(name)) return true;
  }
  return false;
}

std::pair<Scope::VariableTable::iterator, bool> StackFrame::declareVar(
    const Identifier& name, const TypeIdentifier& type, std::set<Modifier>&& modifiers) {
  expect(!m_scopes.empty(), std::logic_error("Cannot declare variable, stack empty"));
  return m_scopes.back().declareVar(name, type, std::move(modifiers));
}

std::pair<Scope::VariableTable::iterator, bool> StackFrame::defineVar(
    const Identifier& name, const TypeIdentifier& type, const Value& value,
    std::set<Modifier>&& modifiers) {
  expect(!m_scopes.empty(), std::logic_error("Cannot define variable, stack empty"));
  return m_scopes.back().defineVar(name, type, value, std::move(modifiers));
}

bool StackFrame::assignVar(const Identifier& name, const Value& value) noexcept {
  for (auto it = m_scopes.rbegin(); it != m_scopes.rend(); ++it) {
    if (it->varIsDeclared(name)) {
      return it->assignVar(name, value);
    }
  }
  return false;
}

std::optional<std::reference_wrapper<Variable>> StackFrame::getVar(
    const Identifier& name) noexcept {
  for (auto it = m_scopes.rbegin(); it != m_scopes.rend(); ++it) {
    if (it->varIsDeclared(name)) return it->getVar(name);
  }
  return std::nullopt;
}

/* ------------------------------ Type methods ------------------------------ */

bool StackFrame::typeIsDeclared(const TypeIdentifier& name) const noexcept {
  for (auto it = m_scopes.crbegin(); it != m_scopes.crend(); ++it) {
    if (it->typeIsDeclared(name)) return true;
  }
  return false;
}

bool StackFrame::typeIsDefined(const TypeIdentifier& name) const noexcept {
  for (auto it = m_scopes.crbegin(); it != m_scopes.crend(); ++it) {
    if (it->typeIsDefined(name)) return true;
  }
  return false;
}

std::pair<Scope::TypeTable::iterator, bool> StackFrame::declareType(const TypeIdentifier& name) {
  expect(!m_scopes.empty(), std::logic_error("Cannot declare type, stack empty"));
  return m_scopes.back().declareType(name);
}

std::pair<Scope::TypeTable::iterator, bool> StackFrame::defineType(const TypeIdentifier& name,
                                                                   Type&& type) {
  expect(!m_scopes.empty(), std::logic_error("Cannot define type, stack empty"));
  return m_scopes.back().defineType(name, std::move(type));
}

std::optional<TypeRef> StackFrame::getType(const TypeIdentifier& name) const noexcept {
  for (auto it = m_scopes.crbegin(); it != m_scopes.crend(); ++it) {
    if (it->typeIsDefined(name)) return it->getType(name);
  }
  return std::nullopt;
}

}  // namespace Interpreter
