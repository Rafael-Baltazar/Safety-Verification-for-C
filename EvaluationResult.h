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
    Z3_ast ast;
public:
    EvaluationResult() {
        resultType = Error;
    }

    EvaluationResult(EvalResultType type) {
        resultType = type;
    }

    EvaluationResult(Z3_ast a) {
        resultType = Ok;
        ast = a;
    }

    bool isOk() {
        return resultType == Ok;
    }

    Z3_ast *getAst() {
        return &ast;
    }

    EvalResultType getType() {
        return resultType;
    }
};

#endif
