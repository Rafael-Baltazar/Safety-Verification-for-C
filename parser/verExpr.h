#if !defined(verExpr_P)
#define verExpr_P

#include "verVariable.h"

namespace ver {

typedef enum {
	SumExpr = 0,	// +
	SubExpr = 1,	// -
	MulExpr = 2,	// *
	DivExpr = 3,	// /
	ErrExpr,		// error
	ConExpr,		// integer constant
	VarExpr			// variable
} ExprType;

class Expr {
	private:
		ExprType _type;
		Expr* _left;
		Expr* _right;
		Variable* _variable;
		int _value;

	public:
		Expr() {
			_type = ErrExpr;
			_left = NULL;
			_right = NULL; 
			_variable = NULL;
			_value = 0;
		}
		Expr(ExprType type) { _type = type; }
		Expr(ExprType type, Expr* left, Expr* right) {
			_type = type;
			_left = left;
			_right = right;
		}
		Expr(int value) { _type = ConExpr; _value = value; }
		Expr(Variable* variable) { _type = VarExpr; _variable = variable; }
		~Expr() { }

		ExprType getType() { return _type; }

		Expr* getLeft() { return _left; }
		void setLeft(Expr* e) { _left = e; }

		Expr* getRight() { return _right; }
		void setRight(Expr* e) { _right = e; }

		Variable* getVar() { return _variable; }
		void setVar(Variable* v) { _variable = v; }

		int getVal() { return _value; }
		void setVal(int n) { _value = n; }
};

}

#endif /* verExpr_P */
