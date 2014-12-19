#if !defined(verFile_P)
#define verFile_P

#include <vector>
#include "verClause.h"

extern int yyparse(void *param);

using namespace std;

namespace ver {

class File {
	private:
        Clause* _current;
		vector<Clause*> _clauses;

	public:
		File() { _current = NULL; }
		~File() {
			for(vector<Clause*>::iterator it = _clauses.begin();
				it != _clauses.end(); it++)
				delete *it;
			_clauses.clear();
		};

		Clause* getCurrentClause() { return _current; }
		Clause* nextClause() {
			_clauses.push_back(_current);
			_current = new Clause();
			return _current;
		}

        void parse() {
			_current = new Clause();
			yyparse(this);
		}
        const vector<Clause*>& getClauses() { return _clauses; }
};

}

#endif /* verFile_P */
