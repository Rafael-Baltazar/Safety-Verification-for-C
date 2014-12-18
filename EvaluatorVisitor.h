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
using namespace ver;

class EvaluatorVisitor : public NodeVisitor {
private:
    int threshold;
    Z3_context *ctx;
    Z3_sort *realSort;

    string toString(int n) {
        stringstream ss;
        ss << n;
        return ss.str();
    }

    EvaluationResult *evalExpr(Expr *expr, Substitution *substitution) {
        cout << "Evaluating expression: ";
        EvaluationResult *result, *left, *right;
        ExprType type = expr->getType();
        if(type == SumExpr) {
            cout << "+\n";
            left = evalExpr(expr->getLeft(), substitution);
            right = evalExpr(expr->getRight(), substitution);
            if(left->isOk() && right->isOk()) {
                Z3_ast args[2];
                args[0] = *(left->getAst());
                args[1] = *(right->getAst());
                Z3_ast a = Z3_mk_add(*ctx, 2, args);
                result = new EvaluationResult(&a);
            } else {
                result = new EvaluationResult(Error);
            }
        } else if(type == SubExpr) {
            cout << "-\n";
            left = evalExpr(expr->getLeft(), substitution);
            right = evalExpr(expr->getRight(), substitution);
            if(left->isOk() && right->isOk()) {
                Z3_ast args[2];
                args[0] = *(left->getAst());
                args[1] = *(right->getAst());
                Z3_ast a = Z3_mk_sub(*ctx, 2, args);
                result = new EvaluationResult(&a);
            } else {
                result = new EvaluationResult(Error);
            }
        } else if(type == MulExpr) {
            cout << "*\n";
            left = evalExpr(expr->getLeft(), substitution);
            right = evalExpr(expr->getRight(), substitution);
            if(left->isOk() && right->isOk()) {
                Z3_ast args[2];
                args[0] = *(left->getAst());
                args[1] = *(right->getAst());
                Z3_ast a = Z3_mk_mul(*ctx, 2, args);
                result = new EvaluationResult(&a);
            } else {
                result = new EvaluationResult(Error);
            }
        } else if(type == DivExpr) {
            cout << "/\n";
            left = evalExpr(expr->getLeft(), substitution);
            right = evalExpr(expr->getRight(), substitution);
            if(left->isOk() && right->isOk()) {
                Z3_ast args[2];
                args[0] = *(left->getAst());
                args[1] = *(right->getAst());
                Z3_ast a = Z3_mk_div(*ctx, args[0], args[1]);
                result = new EvaluationResult(&a);
            } else {
                result = new EvaluationResult(Error);
            }
        } else if(type == ConExpr) {
            cout << expr->getVal() << " val\n";
            const char *val = toString(expr->getVal()).c_str();
            Z3_ast a = Z3_mk_numeral(*ctx, val, *realSort);
            result = new EvaluationResult(&a);
        } else if(type == VarExpr) {
            string declaredName = expr->getVar()->getName();
            cout << declaredName << " dn, ";
            string concreteName = substitution->findConcreteName(declaredName);
            cout << concreteName << " cn\n";
            const char *var = concreteName.c_str();
            Z3_ast a = Z3_mk_const(*ctx, Z3_mk_string_symbol(*ctx, var), *realSort);
            result = new EvaluationResult(&a);
        } else {
            result = new EvaluationResult(Error);
        }
        cout << "Finished evaluating expression\n";
        return result;
    }
public:
    EvaluatorVisitor(int k, Z3_context *c, Z3_sort *rS) {
        threshold = k;
        ctx = c;
        realSort = rS;
    }

    EvaluationResult *eval(Node *node) {
        return node->accept(this);
    }

    EvaluationResult *evalClause(ClauseNode *clause){
        cout << "Evaluating clause\n";
        EvaluationResult *result;
        if(clause->getUseCount() > threshold) {
            result = new EvaluationResult(LoopUnrollThreshold);
        } else {
            vector<EvaluationResult*> *results = new vector<EvaluationResult*>();
            vector<Node*> *nodes = clause->getNodes();
            for(vector<Node*>::iterator it = nodes->begin(); it != nodes->end(); it++) {
                Node *node = *it;
                EvaluationResult *result = eval(node);
                EvalResultType type = result->getType();
                if(type == Ok) {
                    results->push_back(result);
                } else if(type == Error) {
                    cout << "Finished evaluating clause: Error\n";
                    return new EvaluationResult(Error);
                }
            }
            if(results->size() > 0) {
                Z3_ast *args = new Z3_ast[results->size()];
                uint i = 0;
                for(vector<EvaluationResult*>::iterator it = results->begin(); it != results->end() && i < results->size(); it++, i++) {
                    EvaluationResult *eR = *it;
                    Z3_ast *a = eR->getAst();
                    args[i] = *a;
                }
                Z3_ast a = Z3_mk_or(*ctx, results->size(), args);
                result = new EvaluationResult(&a);
            } else {
                cout << "Finished evaluating clause: all sub-nodes are loopUnrollThreshold\n";
                return new EvaluationResult(LoopUnrollThreshold);
            }
        }
        cout << "Finished evaluating clause\n";
        return result;
    }

    EvaluationResult *evalConjunction(ConjunctionNode *conjunction){
        cout << "Evaluating conjunction\n";
        vector<Node*> *nodes = conjunction->getNodes();
        Z3_ast *args = new Z3_ast[nodes->size()];
        uint i = 0;
        for(vector<Node*>::iterator it = nodes->begin(); it != nodes->end() && i < nodes->size(); it++, i++) {
            Node *node = *it;
            EvaluationResult *result = eval(node);
            EvalResultType type = result->getType();
            if(type == Ok) {
                Z3_ast *a = result->getAst();
                args[i] = *a;
            } else if(type == LoopUnrollThreshold) {
                cout << "Finished evaluating conjunction: loopUnrollThreshold\n";
                return new EvaluationResult(LoopUnrollThreshold);
            } else {
                cout << "Finished evaluating conjunction: Error\n";
                return new EvaluationResult(Error);
            }
        }

        Z3_ast a = Z3_mk_and(*ctx, nodes->size(), args);
        cout << "Finished evaluating conjunction\n";
        return new EvaluationResult(&a);
    }

    // pre: boolean->getBExpr()->getType != ClBExpr
    EvaluationResult *evalBoolean(BooleanNode *boolean){
        cout << "Evaluating boolean: ";
        EvaluationResult *result, *left, *right;
        Substitution *substitution = boolean->getSubstitution();
        BExpr *bexpr = boolean->getBExpr();
        BExprType type = bexpr->getType();
        if(type == EqBExpr) {
            cout << "=\n";
            left = evalExpr(bexpr->getLeft(), substitution);
            right = evalExpr(bexpr->getRight(), substitution);
            if(left->isOk() && right->isOk()) {
                Z3_ast *la = left->getAst();
                Z3_ast *ra = right->getAst();
                Z3_ast a = Z3_mk_eq(*ctx, *la, *ra);
                result = new EvaluationResult(&a);
            } else {
                result = new EvaluationResult(Error);
            }
        } else if(type == NeBExpr) {
            cout << "!=\n";
            left = evalExpr(bexpr->getLeft(), substitution);
            right = evalExpr(bexpr->getRight(), substitution);
            if(left->isOk() && right->isOk()) {
                Z3_ast *la = left->getAst();
                Z3_ast *ra = right->getAst();
                Z3_ast a = Z3_mk_not(*ctx, Z3_mk_eq(*ctx, *la, *ra));
                result = new EvaluationResult(&a);
            } else {
                result = new EvaluationResult(Error);
            }
        } else if(type == LtBExpr) {
            cout << "<\n";
            left = evalExpr(bexpr->getLeft(), substitution);
            right = evalExpr(bexpr->getRight(), substitution);
            if(left->isOk() && right->isOk()) {
                Z3_ast *la = left->getAst();
                Z3_ast *ra = right->getAst();
                Z3_ast a = Z3_mk_lt(*ctx, *la, *ra);
                result = new EvaluationResult(&a);
            } else {
                result = new EvaluationResult(Error);
            }
        } else if(type == GtBExpr) {
            cout << ">\n";
            left = evalExpr(bexpr->getLeft(), substitution);
            right = evalExpr(bexpr->getRight(), substitution);
            if(left->isOk() && right->isOk()) {
                Z3_ast *la = left->getAst();
                Z3_ast *ra = right->getAst();
                Z3_ast a = Z3_mk_gt(*ctx, *la, *ra);
                result = new EvaluationResult(&a);
            } else {
                result = new EvaluationResult(Error);
            }
        } else if(type == LeBExpr) {
            cout << "<=\n";
            left = evalExpr(bexpr->getLeft(), substitution);
            right = evalExpr(bexpr->getRight(), substitution);
            if(left->isOk() && right->isOk()) {
                Z3_ast *la = left->getAst();
                Z3_ast *ra = right->getAst();
                Z3_ast a = Z3_mk_le(*ctx, *la, *ra);
                result = new EvaluationResult(&a);
            } else {
                result = new EvaluationResult(Error);
            }
        } else if(type == GeBExpr) {
            cout << ">=\n";
            left = evalExpr(bexpr->getLeft(), substitution);
            right = evalExpr(bexpr->getRight(), substitution);
            if(left->isOk() && right->isOk()) {
                Z3_ast *la = left->getAst();
                Z3_ast *ra = right->getAst();
                Z3_ast a = Z3_mk_ge(*ctx, *la, *ra);
                result = new EvaluationResult(&a);
            } else {
                result = new EvaluationResult(Error);
            }
        } else {
            result = new EvaluationResult(Error);
        }
        cout << "Finished evaluating boolean\n";
        return result;
    }
};

#endif

