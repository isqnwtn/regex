#include "ast.h"
#include <string>
#include <variant>

inline std::string space(u8 depth) { return std::string(depth, ' '); }

std::string CharNode::debug(u8 depth) {
  return space(depth) + "char(" + value + ")";
}

std::string WildCardnode::debug(u8 depth) { return space(depth) + "wildcard"; }

std::string GroupNode::debug(u8 depth) {
    std::string res = space(depth)+"group{\n";
    res += (node->debug(depth+1)+"\n");
    res += space(depth)+"}\n";
    return res;
}

std::string RepeatNode::debug(u8 depth) {
    std::string res = expr->debug(depth+1);

    return space(depth)+"repeat(\n"+res+space(depth)+")\n";
}

std::string ConcatNode::debug(u8 depth) {
   std::string res;
   res += (space(depth)+"concat{\n");
   for (RegexPtr x : seq){
       res += (x->debug(depth+1)+"\n");
   }
   res += (space(depth)+"}\n");
   return res;
}

std::string AltNode::debug(u8 depth) {
    std::string left_dbg = left->debug(depth+1);
    std::string right_dbg = right->debug(depth+1);
    std::string res = space(depth)+"alt{\n";
    res += (left_dbg+"\n");
    res += (space(depth)+"|\n");
    res += (right_dbg+"\n"+space(depth)+"}\n");
    return res;
}


std::string RegexNode::debug(u8 depth) {
    struct RegexDebugVisitor{
        u8 depth;
        RegexDebugVisitor(u8 d) : depth(d){}
        std::string operator()(CharNode c){return c.debug(depth+1);}
        std::string operator()(WildCardnode w){return w.debug(depth+1);}
        std::string operator()(GroupNode g){return g.debug(depth+1);}
        std::string operator()(RepeatNode r){return r.debug(depth+1);}
        std::string operator()(ConcatNode c){return c.debug(depth+1);}
        std::string operator()(AltNode a){return a.debug(depth+1);}
    };
    std::string res = std::visit(RegexDebugVisitor(depth), node);
    return res;
}

std::string RegexNode::debug_print() {
    return debug(0);
}
