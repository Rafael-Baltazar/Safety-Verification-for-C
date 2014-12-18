#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <sstream>
#include "z3.h"
#include "verFile.h"
#include "DEBUG.h"
#include "NodeBuilder.h"
#include "EvaluatorVisitor.h"
#include "EvaluationResult.h"

using namespace std;

void startZ3(Z3_config &cfg, Z3_context &ctx, Z3_sort &realSort, Z3_sort &boolSort) {
    cfg = Z3_mk_config();
    Z3_set_param_value(cfg, "MODEL", "true");
    ctx = Z3_mk_context(cfg);
    realSort = Z3_mk_real_sort(ctx);
    boolSort = Z3_mk_bool_sort(ctx);
}

void stopZ3(Z3_config &cfg, Z3_context &ctx) {
    Z3_del_context(ctx);
    Z3_del_config(cfg);
}

Z3_lbool checkZ3(Z3_context &ctx, Z3_ast &ast) {
    Z3_assert_cnstr(ctx, ast);
    return Z3_check(ctx);
}

int main(int argc, char* argv[])
{
    int threshold = (argc > 0)?atoi(argv[0]) + 1:5;
if(DEBUG)cout << "Loop unroll threshold: " << threshold << "\n";
    Z3_config cfg;
    Z3_context ctx;
    Z3_sort realSort, boolSort;
    // build tree from VCG file
    File *f = new File();
    f->parse();
    vector<Clause*> *falseClauses = new vector<Clause*>();
    vector<Clause*> clauses = f->getClauses();
    for(vector<Clause*>::iterator it = clauses.begin(); it != clauses.end(); it++) {
        Clause *c = *it;
        if(!c->getName().compare("false")) {
            falseClauses->push_back(c);
        }
    }
    for(vector<Clause*>::iterator it = falseClauses->begin(); it != falseClauses->end(); it++) {
        bool end = false;
        Clause *c = *it;
        // build tree from c
        NodeBuilder *nb = new NodeBuilder(threshold, f);
        Node *n = nb->build(c);

        startZ3(cfg, ctx, realSort, boolSort);

        NodeVisitor *ev = new EvaluatorVisitor(threshold, &ctx, &realSort);
        EvaluationResult *evalResult = ev->eval(n);

        if(evalResult->isOk()) {
            Z3_ast *ast = evalResult->getAst();
            Z3_lbool res = checkZ3(ctx, *ast);

            if(res == Z3_L_TRUE) {
                end = true;
                cout << "UNSAT\n";
            } else if(res == Z3_L_UNDEF) {
                end = true;
                cout << "UNDEF\n";
            }
        } else {
            end = true;
            cout << "UNDEF\n";
        }
        delete nb;
//        delete n;
//        delete ev;
        delete evalResult;

        stopZ3(cfg, ctx);

        if(end) {
            delete f;
            return 0;
        }
    }
    // cleanup
    delete f;
    // gracefully terminate
    cout << "SAT\n";
    return 0;
}

