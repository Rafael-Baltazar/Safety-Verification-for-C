#ifndef __Node_H_
#define __Node_H_

class EvaluationResult;
class NodeVisitor;
class EvaluationContext;

class Node {
public:
    virtual EvaluationResult *accept(NodeVisitor *visitor, EvaluationContext *context) = 0;
};

#endif

