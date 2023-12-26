#include <stack>
#include <unordered_map>

template <typename T>
class ScopedTable {
 public:
  ScopedTable() = default;
  ~ScopedTable() = default;

  ScopedTable(const ScopedTable&) = delete;
  ScopedTable(ScopedTable&&) = delete;
  ScopedTable& operator=(const ScopedTable&) = delete;
  ScopedTable& operator=(ScopedTable&&) = delete;

  void enterScope() noexcept { m_scopes.push(T()); }

  void exitScope() {
    if (!m_scopes.empty()) {
      m_scopes.pop();
    }
    throw std::logic_error("Empty ScopedTable!");
  }

  auto insert(const Identifier& id, T&& value) {
    if (!m_scopes.empty()) {
      return m_scopes.top().insert({id, std::move(value)});
    }
    throw std::logic_error("Empty ScopedTable!");
  }

  bool contains(const Identifier& id) const noexcept {
    for (auto it = m_scopes.crbegin(); it != m_scopes.crend(); ++it) {
      if (it->contains(id)) {
        return true;
      }
    }
    return false;
  }

 private:
  using Scope = std::unordered_map<Identifier, T>;

  std::stack<Scope> m_scopes;
};
