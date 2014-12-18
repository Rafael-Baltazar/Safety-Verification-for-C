#ifndef __Substitution_H_
#define __Substitution_H_

#include "FindAllVars.h"

using namespace std;
using namespace ver;

class Substitution {
private:
    map<string, string> *boundVars;
    map<string, string> *vars;
    int *nFree;

    string toString(int n) {
        stringstream ss;
        ss << n;
        return ss.str();
    }

    string createFreeVar() {
        string newVar = "v" + toString(*nFree);
        cout << "Created var: " << newVar << "\n";
        (*nFree)++;
        return newVar;
    }

    void findConcreteForAllVars(Clause *c, Substitution *s) {
        vector<Variable*> *allVars = (new FindAllVars())->findClause(c);
        for(vector<Variable*>::iterator it = allVars->begin(); it != allVars->end(); it++) {
            string varName = (*it)->getName();
            s->findConcreteName(varName);//Ignore return value;
        }
    }

    Substitution(int *nF) {
        boundVars = new map<string, string>();
        vars = new map<string, string>();
        nFree = nF;
    }
public:
    Substitution(int *nF, Clause *c) {
        boundVars = new map<string, string>();
        vars = new map<string, string>();
        nFree = nF;
        findConcreteForAllVars(c, this);
    }

    string findConcreteName(string declaredName) {
        if(boundVars->count(declaredName) > 0) {
            string concreteName = (*boundVars)[declaredName];
            return concreteName;
        } else if(vars->count(declaredName) > 0) {
            string concreteName = (*vars)[declaredName];
            return concreteName;
        } else {
            string newVar = createFreeVar();
            (*vars)[declaredName] = newVar;
            return newVar;
        }
    }

    void addSub(string concrete, string declared) {
        cout << "Adding sub for concrete: " << concrete << " and declared: " << declared << "\n";
        (*boundVars)[declared] = concrete;
    }

    /* pre: bexpr->getType() == ClBExpr;
     * Example: currentSub = [v0/W], newSub = [v0/X], bexpr = h1(W), c = h1(X) :-;
     * for each cargs { newSub->add([findSub(carg)/declared(same position as carg)]) }
     * for each Variable in c: findSub(var->name());
     */
    Substitution *createChild(BExpr *bexpr, Clause *c) {
        cout << "Creating substitution child\n";
        Substitution *newSub = new Substitution(nFree);
        vector<Variable*> concreteArgs = bexpr->getArgs();
        vector<Variable*> declaredArgs = c->getArgs();
        for(vector<Variable*>::iterator itC = concreteArgs.begin(), itD = declaredArgs.begin();
            itC != concreteArgs.end() && itD != declaredArgs.end(); itC++, itD++) {
            string semiConcreteName = (*itC)->getName();
            string declaredName = (*itD)->getName();
            string concreteName = findConcreteName(semiConcreteName);
            newSub->addSub(concreteName, declaredName);
        }
        findConcreteForAllVars(c, newSub);
        return newSub;
    }
};

#endif

