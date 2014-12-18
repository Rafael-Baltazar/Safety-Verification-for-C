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
                result = new EvaluationResult(Z3_mk_add(*ctx, 2, args));
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
                result = new EvaluationResult(Z3_mk_sub(*ctx, 2, args));
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
                result = new EvaluationResult(Z3_mk_mul(*ctx, 2, args));
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
                result = new EvaluationResult(Z3_mk_div(*ctx, args[0], args[1]));
            } else {
                result = new EvaluationResult(Error);
            }
        } else if(type == ConExpr) {
            cout << expr->getVal() << " val\n";
            const char *val = toString(expr->getVal()).c_str();
            result = new EvaluationResult(Z3_mk_numeral(*ctx, val, *realSort));
        } else if(type == VarExpr) {
            string declaredName = expr->getVar()->getName();
            cout << declaredName << " dn, ";
            string concreteName = substitution->findConcreteName(declaredName);
            cout << concreteName << " cn\n";
            const char *var = concreteName.c_str();
            result = new EvaluationResult(Z3_mk_const(*ctx, Z3_mk_string_symbol(*ctx, var), *realSort));
        } else {
            result = new EvaluationResult(Error);
        }
cout << result << "\n";
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
        if(clause->getUseCount() > threshold) {
            cout << "Evaluated  clause: LoopUnrollThreshold\n";
            return new EvaluationResult(LoopUnrollThreshold);
        }

        vector<EvaluationResult*> *results = new vector<EvaluationResult*>();
        vector<Node*> *nodes = clause->getNodes();
        for(vector<Node*>::iterator it = nodes->begin(); it != nodes->end(); it++) {
            Node *node = *it;
            EvaluationResult *result = eval(node);
            EvalResultType type = result->getType();
            if(type == Ok) {
                results->push_back(result);
            } else if(type == Error) {
                cout << "Evaluated  clause: Error\n";
                return new EvaluationResult(Error);
            }
        }

        if(results->size() == 0) {
            cout << "Evaluated  clause: all sub-nodes are loopUnrollThreshold\n";
            return new EvaluationResult(LoopUnrollThreshold);
        }

        cout << "Ok nodes: " << results->size() << "\n";
        Z3_ast args[2], *prev = NULL;// not in heap
        for(vector<EvaluationResult*>::iterator it = results->begin(); it != results->end(); it++) {
            EvaluationResult *eR = *it;
            Z3_ast *a = eR->getAst();
cout << "bexpr: " << *a << "\n";
            if(prev != NULL) {
                args[0] = *prev;
                args[1] = *a;
                *a = Z3_mk_or(*ctx, 2, args);
            }
            prev = a;
        }
        cout << "Evaluated  clause\n";
        return new EvaluationResult(*prev);
    }

    EvaluationResult *evalConjunction(ConjunctionNode *conjunction){
        cout << "Evaluating conjunction\n";
        vector<Node*> *nodes = conjunction->getNodes();
        cout << "Nodes: " << nodes->size() << "\n";
        Z3_ast *args = new Z3_ast[nodes->size()];
        uint i = 0;
        for(vector<Node*>::iterator it = nodes->begin(); it != nodes->end() && i < nodes->size(); it++, i++) {
            Node *node = *it;
            EvaluationResult *result = eval(node);
            EvalResultType type = result->getType();
            if(type == Ok) {
                Z3_ast *a = result->getAst();
                args[i] = *a;
cout << "after indexing args: " << i << ": " << args[i] << "\n";
            } else if(type == LoopUnrollThreshold) {
                cout << "Evaluated  conjunction: loopUnrollThreshold\n";
                return new EvaluationResult(LoopUnrollThreshold);
            } else {
                cout << "Evaluated  conjunction: Error\n";
                return new EvaluationResult(Error);
            }
        }
        if(nodes->size() > 1) {
            cout << "Evaluated  conjunction and\n";
            return new EvaluationResult(Z3_mk_and(*ctx, nodes->size(), args));
        } else {
            cout << "Evaluated  conjunction single\n";
            return new EvaluationResult(args[0]);
        }
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
                result = new EvaluationResult(Z3_mk_eq(*ctx, *la, *ra));
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
                result = new EvaluationResult(Z3_mk_not(*ctx, Z3_mk_eq(*ctx, *la, *ra)));
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
                result = new EvaluationResult(Z3_mk_lt(*ctx, *la, *ra));
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
                result = new EvaluationResult(Z3_mk_gt(*ctx, *la, *ra));
            } else {
                result = new EvaluationResult(Error);
            }
        } else if(type == LeBExpr) {
            cout << "<=\n";
            left = evalExpr(bexpr->getLeft(), substitution);
            right = evalExpr(bexpr->getRight(), substitution);
            if(left->isOk() && right->isOk()) {
                Z3_ast *la = left->getAst();
cout << "left: " << *la << "\n";
                Z3_ast *ra = right->getAst();
cout << "right: " << *ra << "\n";
                result = new EvaluationResult(Z3_mk_le(*ctx, *la, *ra));
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
                result = new EvaluationResult(Z3_mk_ge(*ctx, *la, *ra));
            } else {
                result = new EvaluationResult(Error);
            }
        } else {
            result = new EvaluationResult(Error);
        }
        cout << "Evaluated  boolean\n";
        return result;
    }
};

#endif

