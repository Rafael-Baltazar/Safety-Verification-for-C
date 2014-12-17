#ifndef __ClauseNode_H_
#define __ClauseNode_H_

#include "Node.h"
#include "NodeVisitor.h"

class EvaluationResult;
class EvaluationContext;

class ClauseNode : public Node {
private:
    int loopUnroll;
public:
    ClauseNode() {
        loopUnroll = 0;
    }

    EvaluationResult *accept(NodeVisitor *visitor, EvaluationContext *context) {
        return visitor->evalClause(this, context);
    }

    int getLoopUnroll() {
        return loopUnroll;
    }
};

#endif

