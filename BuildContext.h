#ifndef __BuildContext_H_
#define __BuildContext_H_

#include "Substitution.h"
#include "ClauseUse.h"

using namespace std;
using namespace ver;

class BuildContext {
private:
    Substitution *sub;
    ClauseUse *clUse;
public:
    BuildContext(Substitution *s, ClauseUse *cU) {
        sub = s;
        clUse = cU;
    }

    int findUseCount(BExpr *bexpr) {
        return clUse->findUseCount(bexpr);
    }

    // pre: bexpr->getType() == ClBExpr;
    BuildContext *createChild(BExpr *bexpr, Clause *c) {
        Substitution *s = sub->createChild(bexpr, c);
        ClauseUse *cU = clUse->createChild(bexpr);
        return new BuildContext(s, cU);
    }

    Substitution *getSubstitution() {
        return sub;
    }
};

#endif

