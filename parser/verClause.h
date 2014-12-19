#if !defined(verClause_P)
#define verClause_P

#include <vector>
#include <map>
#include "verVariable.h"
#include "verBExpr.h"

using namespace std;

namespace ver {

class Clause {
	private:
		string _name;
		map<string, Variable*> _vars;
		vector<Variable*> _args;
		vector<BExpr*> _bexprs;

	public:
		Clause() { }
		~Clause() {
			for(map<string, Variable*>::iterator it = _vars.begin();
				it != _vars.end(); it++)
				delete it->second;
			_vars.clear();
			_args.clear();
			_name.clear();
		};

		string getName() { return _name; }
		void setName(const string& name) { _name = name; }

		Variable* getVar(string name) {
            map<string, Variable*>::iterator it = _vars.find(name);
			if(it == _vars.end())
				return _vars.insert(pair<string,Variable*>(name,
					new Variable(name))).first->second;
			else
				return it->second;
		}

		void addArg(Variable* v) { _args.push_back(v); }
		const vector<Variable*>& getArgs() { return _args; }

		void addBExpr(BExpr* bexpr) { _bexprs.push_back(bexpr); }
		const vector<BExpr*>& getBExprs() { return _bexprs; }
};

}

#endif /* verClause_P */
