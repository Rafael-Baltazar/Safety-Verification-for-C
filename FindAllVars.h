#ifndef __FindAllVars_H_
#define __FindAllVars_H_

using namespace std;
using namespace ver;

class FindAllVars {
public:
    vector<Variable*> *findClause(Clause *c) {
        vector<Variable*> *allVars = new vector<Variable*>();
        vector<BExpr*> bexprs = c->getBExprs();
        for(vector<BExpr*>::iterator it = bexprs.begin(); it != bexprs.end(); it++) {
            BExpr *bexpr = *it;
            vector<Variable*> *bexprVars = findBExpr(bexpr);
            allVars->insert(allVars->end(), bexprVars->begin(), bexprVars->end());
        }
        return allVars;
    }

    vector<Variable*> *findBExpr(BExpr *bexpr) {
        vector<Variable*> *allVars = new vector<Variable*>();
        BExprType type = bexpr->getType();
        if(type == ErrBExpr) {
            ;// do nothing
        } else if(type == ClBExpr) {
            vector<Variable*> args = bexpr->getArgs();
            allVars->insert(allVars->end(), args.begin(), args.end());
        } else {
            Expr *leftExpr = bexpr->getLeft();
            vector<Variable*> *leftVars = findExpr(leftExpr);
            allVars->insert(allVars->end(), leftVars->begin(), leftVars->end());
            Expr *rightExpr = bexpr->getRight();
            vector<Variable*> *rightVars = findExpr(rightExpr);
            allVars->insert(allVars->end(), rightVars->begin(), rightVars->end());
        }
        return allVars;
    }

    vector<Variable*> *findExpr(Expr *expr) {
        vector<Variable*> *allVars = new vector<Variable*>();
        ExprType type = expr->getType();
        if(type == ErrExpr || type == ConExpr) {
            ;// do nothing
        } else if(type == VarExpr) {
            Variable *var = expr->getVar();
            allVars->push_back(var);
        } else {
            Expr *leftExpr = expr->getLeft();
            vector<Variable*> *leftVars = findExpr(leftExpr);
            allVars->insert(allVars->end(), leftVars->begin(), leftVars->end());
            Expr *rightExpr = expr->getRight();
            vector<Variable*> *rightVars = findExpr(rightExpr);
            allVars->insert(allVars->end(), rightVars->begin(), rightVars->end());
        }
        return allVars;
    }
};

#endif

