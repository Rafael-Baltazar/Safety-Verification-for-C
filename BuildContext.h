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
    BuildContext(int nF) {
        sub = new Substitution(&nF);
        clUse = new ClauseUse();
    }

    BuildContext(Substitution *s, ClauseUse *cU) {
        sub = s;
        clUse = cU;
    }

    // pre: bexpr->getType() == ClBExpr;
    BuildContext *createChild(BExpr *bexpr, Clause *c) {
        Substitution *s = sub->createChild(bexpr, c);
        ClauseUse *cU = clUse->createChild(bexpr);
        return new BuildContext(s, cU);
    }
};

#endif

