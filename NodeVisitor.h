#ifndef __NodeVisitor_H_
#define __NodeVisitor_H_

class Node;
class ClauseNode;
class ConjunctionNode;
class BooleanNode;
class EvaluationContext;
class EvaluationResult;

class NodeVisitor {
public:
    virtual EvaluationResult *eval(Node *node) = 0;

    virtual EvaluationResult *evalClause(ClauseNode *clause) = 0;

    virtual EvaluationResult *evalConjunction(ConjunctionNode *clause) = 0;

    virtual EvaluationResult *evalBoolean(BooleanNode *clause) = 0;
};

#endif
