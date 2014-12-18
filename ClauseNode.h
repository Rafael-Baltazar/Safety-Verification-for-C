#ifndef __ClauseNode_H_
#define __ClauseNode_H_

#include "Node.h"
#include "NodeVisitor.h"

class EvaluationResult;
class EvaluationContext;

class ClauseNode : public Node {
private:
    int useCount;
    vector<Node*> *children;
public:
    ClauseNode() {
        useCount = 0;
        children = new vector<Node*>();
    }

    EvaluationResult *accept(NodeVisitor *visitor) {
        return visitor->evalClause(this);
    }

    void add(Node *n) {
        children->push_back(n);
    }

    vector<Node*> *getNodes() {
        return children;
    }

    int getUseCount() {
        return useCount;
    }

    void setUseCount(int i) {
        useCount = i;
    }
};

#endif

