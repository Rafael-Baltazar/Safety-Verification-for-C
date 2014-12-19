#if !defined(verBExpr_P)
#define verBExpr_P

#include <vector>
#include "verExpr.h"

namespace ver {

typedef enum {
	EqBExpr = 0,	// =
	NeBExpr = 1,	// =\=
	LtBExpr = 2,	// <
	GtBExpr = 3,	// >
	LeBExpr = 4,	// =<
	GeBExpr = 5,	// >=
	ErrBExpr,		// error
	ClBExpr			// hNN()
} BExprType;

class BExpr {
	private:
		BExprType _type;
		Expr* _left;
		Expr* _right;
		std::string _clauseName;
		std::vector<Variable*> _args;

	public:
		BExpr() {
			_type = ErrBExpr;
			_left = NULL;
			_right = NULL; 
		}
		BExpr(BExprType type) { _type = type; }
		BExpr(BExprType type, Expr* left, Expr* right) {
			_type = type;
			_left = left;
			_right = right;
		}
		~BExpr() {
			_clauseName.clear();
			_args.clear();
		}

		BExprType getType() { return _type; }

		Expr* getLeft() { return _left; }
		void setLeft(Expr* e) { _left = e; }

		Expr* getRight() { return _right; }
		void setRight(Expr* e) { _right = e; }

		std::string getClauseName() { return _clauseName; }
		void setClauseName(std::string name) { _clauseName = name; }
		void addArg(Variable* v) { _args.push_back(v); }
		const std::vector<Variable*>& getArgs() { return _args; }
};

}

#endif /* verBExpr_P */
