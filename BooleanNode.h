#ifndef __BooleanNode_H_
#define __BooleanNode_H_

#include "Node.h"
#include "NodeVisitor.h"

class EvaluationResult;
class EvaluationContext;

using namespace ver;

class BooleanNode : public Node {
private:
    BExpr *bexpr;
    Substitution *substitution;
public:
    BooleanNode(BExpr *be, Substitution *s) {
        bexpr = be;
        substitution = s;
    }

    EvaluationResult *accept(NodeVisitor *visitor) {
        return visitor->evalBoolean(this);
    }

    BExpr *getBExpr() {
        return bexpr;
    }

    Substitution *getSubstitution() {
        return substitution;
    }
};

#endif

