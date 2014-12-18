#ifndef __NodeBuilder_H_
#define __NodeBuilder_H_

#include "ClauseNode.h"
#include "ConjunctionNode.h"
#include "BooleanNode.h"
#include "BuildContext.h"

class Node;

using namespace ver;

class NodeBuilder {
private:
    int threshold, nFree;
    File *file;

    vector<Clause*> *findClausesByName(string name) {
        vector<Clause*> fileClauses = file->getClauses(), *clauses = new vector<Clause*>();
        for(vector<Clause*>::iterator it = fileClauses.begin(); it != fileClauses.end(); it++) {
            Clause *c = *it;
            if(!c->getName().compare(name)) {
                clauses->push_back(c);
            }
        }
        return clauses;
    }
public:
    NodeBuilder(int k, File* f) {
        threshold = k;
        file = f;
    }

    Node *build(Clause *c) {
        vector<BExpr*> bexprs = c->getBExprs();
        nFree = 0;
        Substitution *s = new Substitution(&nFree, c);
        ClauseUse *cU = new ClauseUse();
        return buildConjunction(&bexprs, new BuildContext(s, cU));
    }

    // pre: bexpr->getType() == ClBExpr;
    ClauseNode *buildClause(BExpr* bexpr, BuildContext *ctx) {
if(DEBUG)cout << "Buildind clause\n";
        int useCount = ctx->findUseCount(bexpr);
        ClauseNode *clause = new ClauseNode();
        clause->setUseCount(useCount);
        if(useCount <= threshold) {
            vector<Clause*> *clauses = findClausesByName(bexpr->getClauseName());
            for(vector<Clause*>::iterator it = clauses->begin(); it != clauses->end(); it++) {
                Clause *c = *it;
                vector<BExpr*> bexprs = c->getBExprs();
                BuildContext *newCtx = ctx->createChild(bexpr, c); // new ctx
                Node *node = buildConjunction(&bexprs, newCtx);
                clause->add(node);
            }
        }
        return clause;
    }

    ConjunctionNode *buildConjunction(vector<BExpr*> *bexprs, BuildContext *ctx) {
if(DEBUG)cout << "Buildind conjunction\n";
        ConjunctionNode *conjunction = new ConjunctionNode();
        for(vector<BExpr*>::iterator it = bexprs->begin(); it != bexprs->end(); it++) {
            BExpr *bexpr = *it;
            if(bexpr->getType() == ClBExpr) {
                Node *node = buildClause(bexpr, ctx);
                conjunction->add(node);
            } else {
                Node *node = buildBoolean(bexpr, ctx);
                conjunction->add(node);
            }
        }
        return conjunction;
    }

    // pre: bexpr->getType() != ClBExpr;
    BooleanNode *buildBoolean(BExpr *bexpr, BuildContext *ctx) {
if(DEBUG)cout << "Buildind boolean\n";
        return new BooleanNode(bexpr, ctx->getSubstitution());
    }
};

#endif

