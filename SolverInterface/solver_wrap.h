//
// Created by sslebedev on 23.10.16.
//

#ifndef QTSOLVER_SOLVER_WRAP_H
#define QTSOLVER_SOLVER_WRAP_H

#include <iosfwd>

class Result;

/**
 * Cross-platform wrap for solver calls
 */
class SolverWrap final
{
	SolverWrap(const SolverWrap &) = delete;

	SolverWrap &operator=(SolverWrap const &) = delete;

#if defined(__linux__)
	typedef void *DlHandle;
#else
#error PLATFORM IS NOT SUPPORTED
#endif

	DlHandle dlHandle;

	void *GetFunction(const char *nameFunction) const;

	// Functor types for shorter casting
	typedef const char *(*FuncGetStr)();

	typedef const char *(*FuncPresent)(double []);

	typedef Result *(*FuncSolve)(const std::istream &);

	FuncGetStr funcGetUid;
	FuncGetStr funcGetClassName;
	FuncGetStr funcGetMethodName;
	FuncGetStr funcGetDescription;
	FuncGetStr funcGetResolvedForm;
	FuncPresent funcPresentResult;
	FuncSolve funcSolve;

public:
	explicit SolverWrap(char *pathSolver);

	~SolverWrap();

	const char *GetUid() const;

	const char *GetClassName() const;

	const char *GetMethodName() const;

	const char *GetDescription() const;

	const char *GetResolvedForm(const std::istream &input) const;

	const char *PresentResult(double args[]) const;

	const Result *Solve(const std::istream &input);
};


#endif //QTSOLVER_SOLVER_WRAP_H
