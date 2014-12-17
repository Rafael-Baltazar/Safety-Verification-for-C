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
public:
    BooleanNode(BExpr *be) {
        bexpr = be;
    }

    EvaluationResult *accept(NodeVisitor *visitor, EvaluationContext *context) {
        return visitor->evalBoolean(this, context);
    }
};

#endif

