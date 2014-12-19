Safety-Verification-for-C
=========================

Bounded model checker for C programs.
Receives verification conditions from a VCG (Verification Condition Generator) as input and uses the Z3 SMT Solver for constraint solving.
Prints SAT, UNSAT or UNDEF on the standard output.

Requires Z3 version C++ to be installed and its absolute path to be in the variable 'Z3DIR' in the files 'Makefile' and 'runme'.
Folder 'parser' not implemented by me.

Features pipe-and-filter pattern.
VCG's output > NodeBuilder > NodeBuilder's output > EvaluatorVisitor > Evaluator's output > Z3 > SAT, UNSAT

