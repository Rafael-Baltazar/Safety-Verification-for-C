#ifndef __ClauseUse_H_
#define __ClauseUse_H_

using namespace std;
using namespace ver;

class ClauseUse {
private:
    map<string, int> *clauseUses;

    string toString(int n) {
        stringstream ss;
        ss << n;
        return ss.str();
    }

    string getClauseId(BExpr *bexpr) {
        int nargs = bexpr->getArgs().size();
        string name = bexpr->getClauseName();
        return name + toString(nargs);
    }
public:
    ClauseUse() {
        clauseUses = new map<string, int>();
    }

    int findUseCount(BExpr *bexpr) {
        string cId = getClauseId(bexpr);
        if(clauseUses->count(cId) > 0) {
            return (*clauseUses)[cId];
        } else {
            (*clauseUses)[cId] = 0;
            return 0;
        }
    }

    // pre: bexpr->getType() == ClBExpr
    void addUseCount(BExpr *bexpr) {
        cout << "Adding use count to: " << bexpr->getClauseName() << "\n";
        string cId = getClauseId(bexpr);
        if(clauseUses->count(cId) > 0) {
            (*clauseUses)[cId] += 1;
        } else {
            (*clauseUses)[cId] = 1;
        }
    }

    void copyClauseUses(map<string, int> *cU) {
        clauseUses->insert(cU->begin(), cU->end());
    }

    ClauseUse *createChild(BExpr *bexpr) {
        cout << "Creating clauseUse child\n";
        ClauseUse *cU = new ClauseUse();
        cU->copyClauseUses(clauseUses);
        cU->addUseCount(bexpr);
        return cU;
    }
};

#endif

