#include <iostream>
#include <stdlib.h>
#include <vector>
#include "z3.h"
#include "verFile.h"
#include "ClauseNode.h"
#include "EvaluatorVisitor.h"
#include "EvaluationResult.h"

using namespace std;

int main(int argc, char* argv[])
{
    // Loop unroll threshold
    int threshold = (argc > 0)?atoi(argv[0]):1;
    //// dumb example of Z3 usage
    // initializations
    Z3_config cfg = Z3_mk_config();
    Z3_set_param_value(cfg, "MODEL", "true");
    Z3_context ctx = Z3_mk_context(cfg);
    Z3_sort realSort = Z3_mk_real_sort(ctx);
    Z3_sort boolSort = Z3_mk_bool_sort(ctx);
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
    Z3_assert_cnstr(ctx, a4);
    // check and get model
    Z3_model model;
    Z3_lbool result = Z3_check_and_get_model(ctx, &model);
    // dump model
    Z3_ast value;
    Z3_eval(ctx, model, va, &value);
    cout << "a: " << Z3_ast_to_string(ctx, value) << endl;
    Z3_eval(ctx, model, vb, &value);
    cout << "b: " << Z3_ast_to_string(ctx, value) << endl;
    Z3_eval(ctx, model, vc, &value);
    cout << "c: " << Z3_ast_to_string(ctx, value) << endl;
    Z3_eval(ctx, model, vd, &value);
    cout << "d: " << Z3_ast_to_string(ctx, value) << endl;

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
        Clause *c = *it;
        // build tree from clause
        ;
        // evaluate tree to extract Z3_ast or error
        NodeVisitor *ev = new EvaluatorVisitor(threshold);
        Node *n = new ClauseNode();
        EvaluationResult *evalResult = ev->eval(n);
        if(evalResult->isOk()) {
            Z3_ast ast = evalResult->getAst();
        } else {
            cout << "UNDEF\n";
            return 0;
        }
    }

    // cleanup
    Z3_del_context(ctx);
    Z3_del_config(cfg);
    delete f;

    // gracefully terminate
    cout << "SAT\n";
    return 0;
}
