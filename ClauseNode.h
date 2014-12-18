#ifndef __ClauseNode_H_
#define __ClauseNode_H_

#include "Node.h"
#include "NodeVisitor.h"

class EvaluationResult;
class EvaluationContext;

class ClauseNode : public Node {
private:
    int loopUnroll;
    vector<Node*> *children;
public:
    ClauseNode() {
        loopUnroll = 0;
        children = new vector<Node*>();
    }

    EvaluationResult *accept(NodeVisitor *visitor, EvaluationContext *context) {
        return visitor->evalClause(this, context);
    }

    void add(Node *n) {
        children->push_back(n);
    }

    int getLoopUnroll() {
        return loopUnroll;
    }

    void setLoopUnroll(int i) {
        loopUnroll = i;
    }
};

#endif

