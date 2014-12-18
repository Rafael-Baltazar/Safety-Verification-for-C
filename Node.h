#ifndef __Node_H_
#define __Node_H_

class EvaluationResult;
class NodeVisitor;
class EvaluationContext;
class Substitution;

class Node {
public:
    virtual EvaluationResult *accept(NodeVisitor *visitor) = 0;
};

#endif

