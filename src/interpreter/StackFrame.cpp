#include "StackFrame.h"

namespace {

void expect(bool condition, const std::exception& exception) {
  if (!condition) throw exception;
}

}  // namespace

namespace Interpreter {

StackFrame::StackFrame(const Identifier& fnName) : m_fnName(fnName) { m_scopes.emplace_back(); }

bool StackFrame::nameConflict(const Identifier& name) const noexcept {
  return m_scopes.back().nameConflict(name);
}

void StackFrame::enterScope() noexcept { m_scopes.emplace_back(); }

void StackFrame::exitScope() {
  expect(m_scopes.size() > 1, std::logic_error("Cannot exit scope, stack empty"));
  m_scopes.pop_back();
}

/* ---------------------------- Variable methods ---------------------------- */

bool StackFrame::containsVar(const Identifier& name) const noexcept {
  for (auto it = m_scopes.crbegin(); it != m_scopes.crend(); ++it) {
    if (it->containsVar(name)) return true;
  }
  return false;
}

std::pair<Scope::VariableTable::iterator, bool> StackFrame::insertVar(const Identifier& name,
                                                                      Variable&& var) {
  expect(!m_scopes.empty(), std::logic_error("Cannot define variable, stack empty"));
  return m_scopes.back().insertVar(name, std::move(var));
}

std::optional<VariablePtr> StackFrame::getVar(
    const Identifier& name) noexcept {
  for (auto it = m_scopes.rbegin(); it != m_scopes.rend(); ++it) {
    if (it->containsVar(name)) return it->getVar(name);
  }
  return std::nullopt;
}

/* ------------------------------ Type methods ------------------------------ */

bool StackFrame::containsType(const TypeIdentifier& name) const noexcept {
  for (auto it = m_scopes.crbegin(); it != m_scopes.crend(); ++it) {
    if (it->containsType(name)) return true;
  }
  return false;
}

std::pair<Scope::TypeTable::iterator, bool> StackFrame::insertType(const TypeIdentifier& name,
                                                                   Type&& type) {
  expect(!m_scopes.empty(), std::logic_error("Cannot define type, stack empty"));
  return m_scopes.back().insertType(name, std::move(type));
}

std::optional<TypePtr> StackFrame::getType(const TypeIdentifier& name) const noexcept {
  for (auto it = m_scopes.crbegin(); it != m_scopes.crend(); ++it) {
    if (it->containsType(name)) return it->getType(name);
  }
  return std::nullopt;
}

}  // namespace Interpreter
