#ifndef __EvaluationResult_H_
#define __EvaluationResult_H_

using namespace std;

typedef enum {
    Ok = 0,
    Error = 1,
    LoopUnrollThreshold = 2
} EvalResultType;

class EvaluationResult {
private:
    EvalResultType resultType;
    Z3_ast *tree;
public:
    EvaluationResult() {
        resultType = Error;
    }

    EvaluationResult(EvalResultType type) {
        resultType = type;
    }

    EvaluationResult(Z3_ast *t) {
        resultType = Ok;
        tree = t;
    }

    bool isOk() {
        return resultType == Ok;
    }

    Z3_ast *getAst() {
        return tree;
    }

    EvalResultType getType() {
        return resultType;
    }
};

#endif
