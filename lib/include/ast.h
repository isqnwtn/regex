#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <variant>
#include <vector>

struct RegexNode;
using RegexPtr = std::shared_ptr<RegexNode>;
using u8 = std::uint8_t;

// TODO : Change this trait bound to codegen
template <typename D, typename ...Args>
struct regex_trait_bound {
    template <typename U, typename = decltype(std::declval<U>().parse_self(std::declval<Args>()...))>
    static std::true_type test(int);
    template <typename U>
    static std::false_type test(...);
    static constexpr bool value = decltype(test<D>(0))::value;
};

// CRTP Based implementation for Nodes
template <typename Derived,typename ...Args>
class NodeTrait {
    public:
    NodeTrait() = default;
    std::string debug(u8 depth) {
        static_cast<Derived*>(this)->debug(depth);
    }
    // DONE : make sure this fails when the derived class doesn't implement it, remove the static_assert to enable it
    // static_assert(regex_trait_bound<Derived,Args...>::value, "parse_self not implemented");
    RegexPtr parse_self(Args... args) {
        static_cast<Derived*>(this)->parse_self(args...);
    }
    private:
};

struct CharNode : public NodeTrait<CharNode> {
  char value;
  std::string debug(u8 depth);
  public:
  CharNode(char value) : value(value) {}
};



struct WildCardnode : public NodeTrait<WildCardnode> {
    WildCardnode() = default;
    std::string debug(u8 depth);
};

struct GroupNode : public NodeTrait<GroupNode> {
  RegexPtr node;
  public:
  GroupNode(RegexPtr node) : node(node) {}
  std::string debug(u8 depth);
};

enum class RepeatKind {
  ZeroOrMore,
  OneOrMore,
  Optional,
};
struct RepeatNode : public NodeTrait<RepeatNode> {
  RegexPtr expr;
  RepeatKind kind;
  public:
  RepeatNode(RegexPtr expr, RepeatKind kind) : expr(expr), kind(kind) {}
  std::string debug(u8 depth);
};

struct ConcatNode : public NodeTrait<ConcatNode> {
  std::vector<RegexPtr> seq;
  public:
  ConcatNode(std::vector<RegexPtr> seq) : seq(seq) {};
  std::string debug(u8 depth);
};

struct AltNode : public NodeTrait<AltNode> {
  RegexPtr left;
  RegexPtr right;
  public:
  AltNode(RegexPtr left, RegexPtr right) : left(left), right(right) {}
  std::string debug(u8 depth);
};

using RegexVariant = std::variant<CharNode, WildCardnode, GroupNode, RepeatNode,
                                  ConcatNode, AltNode>;

struct RegexNode  {
  RegexVariant node;
  std::string debug(u8 depth);
  public:
  RegexNode(RegexVariant node) : node(std::move(node)) {}
  std::string debug_print();
};
