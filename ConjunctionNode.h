#ifndef __ConjunctionNode_H_
#define __ConjunctionNode_H_

#include "Node.h"
#include "NodeVisitor.h"

class EvaluationResult;
class EvaluationContext;

class ConjunctionNode : public Node{
private:
    vector<Node*> *children;
public:
    ConjunctionNode() {
        children = new vector<Node*>();
    }
    EvaluationResult *accept(NodeVisitor *visitor, EvaluationContext *context) {
        return visitor->evalConjunction(this, context);
    }
};

#endif

