#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <sstream>
#include "z3.h"
#include "verFile.h"
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

Z3_lbool endZ3(Z3_config &cfg, Z3_context &ctx, Z3_ast &ast) {
    Z3_assert_cnstr(ctx, ast);
    Z3_lbool result = Z3_check(ctx);
    Z3_del_context(ctx);
    Z3_del_config(cfg);
    return result;
}

int main(int argc, char* argv[])
{
    // Loop unroll threshold
    int threshold = (argc > 0)?atoi(argv[0]):1;
    //// dumb example of Z3 usage
    Z3_config cfg;
    Z3_context ctx;
    Z3_sort realSort, boolSort;
    startZ3(cfg, ctx, realSort, boolSort);

    Z3_ast args[2];
    // get a model for: (a + b > 0.5) AND (c OR d)
    // a1: (a + b)
    Z3_ast va = Z3_mk_const(ctx, Z3_mk_string_symbol(ctx, "a"), realSort);
    Z3_ast vb = Z3_mk_const(ctx, Z3_mk_string_symbol(ctx, "b"), realSort);
    args[0] = va;
    args[1] = vb;
    Z3_ast a1 = Z3_mk_add(ctx, 2, args);
    // a2: (a + b > 0.5)
    args[0] = a1;
    args[1] = Z3_mk_numeral(ctx, "1/2", realSort);
    Z3_ast a2 = Z3_mk_gt(ctx, args[0], args[1]);
    // a3: (c OR d) 
    Z3_ast vc = Z3_mk_const(ctx, Z3_mk_string_symbol(ctx, "c"), boolSort);
    Z3_ast vd = Z3_mk_const(ctx, Z3_mk_string_symbol(ctx, "d"), boolSort);
    args[0] = vc;
    args[1] = vd;
    Z3_ast a3 = Z3_mk_or(ctx, 2, args);
    // a4: (a + b > 0.5) AND (c OR d)
    args[0] = a2;
    args[1] = a3;
    Z3_ast a4 = Z3_mk_and(ctx, 2, args);
    // assert that a4 should be true
    Z3_lbool result = endZ3(cfg, ctx, a4);

    // print result
    if(result == Z3_L_FALSE) {
        cout << "false\n";
    } else if(result == Z3_L_TRUE) {
        cout << "true\n";
    } else if(result == Z3_L_UNDEF) {
        cout << "undef\n";
    }

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
        // Z3_ast or error
        startZ3(cfg, ctx, realSort, boolSort);
        NodeVisitor *ev = new EvaluatorVisitor(threshold);
        EvaluationResult *evalResult = ev->eval(n);
        if(evalResult->isOk()) {
            Z3_ast *ast = evalResult->getAst();
            endZ3(cfg, ctx, *ast);
            if(result == Z3_L_TRUE || result == Z3_L_UNDEF) {
                end = true;
                cout << "UNSAT\n";
            }
        } else {
            end = true;
            cout << "UNDEF\n";
        }
        delete nb;
//        delete n;
//        delete ev;
        delete evalResult;
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

