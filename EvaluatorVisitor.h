#ifndef __EvaluatorVisitor_H_
#define __EvaluatorVisitor_H_

#include "NodeVisitor.h"
#include "Node.h"
#include "ClauseNode.h"
#include "ConjunctionNode.h"
#include "BooleanNode.h"
#include "EvaluationContext.h"
#include "EvaluationResult.h"

class ConjunctionNode;
class BooleanNode;

using namespace std;

class EvaluatorVisitor : public NodeVisitor {
private:
    int loopUnroll;
public:
    EvaluatorVisitor(int k) {
        loopUnroll = k;
    }

    EvaluationResult *eval(Node *node) {
        return node->accept(this, new EvaluationContext());
    }

    EvaluationResult *evalClause(ClauseNode *clause, EvaluationContext *context){
        if(clause->getLoopUnroll() >= loopUnroll) {
            return new EvaluationResult(LoopUnrollThreshold);
        } else {
            return new EvaluationResult();
        }
    }

    EvaluationResult *evalConjunction(ConjunctionNode *clause, EvaluationContext *context){
        return new EvaluationResult();
    }

    EvaluationResult *evalBoolean(BooleanNode *clause, EvaluationContext *context){
        return new EvaluationResult();
    }
};

#endif
