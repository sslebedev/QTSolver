//
// Created by sslebedev on 23.10.16.
//

#ifndef QTSOLVER_SOLVER_WRAP_H
#define QTSOLVER_SOLVER_WRAP_H

#include <iosfwd>
#include <vector>

class Result;

/**
 * Cross-platform wrap for solver calls
 */
class SolverWrap final
{
	SolverWrap(const SolverWrap &) = delete;

	SolverWrap &operator=(SolverWrap const &) = delete;

	typedef void *DlHandle;

	DlHandle dlHandle;

	void *GetFunction(const char *nameFunction) const;

	// Functor types for shorter casting
	typedef const char *(*FuncGetStr)();

	typedef const char *(*FuncResolve)(std::istream &);

	typedef const char *(*FuncPresent)(const std::vector<double> &results);

	typedef const Result (*FuncSolve)(std::istream &);

	FuncGetStr funcGetUid;
	FuncGetStr funcGetClassName;
	FuncGetStr funcGetMethodName;
	FuncGetStr funcGetDescription;
	FuncResolve funcGetResolvedForm;
	FuncPresent funcPresentResult;
	FuncSolve funcSolve;

public:
	explicit SolverWrap(const char *pathSolver);

	~SolverWrap();

	const char *GetUid() const;

	const char *GetClassName() const;

	const char *GetMethodName() const;

	const char *GetDescription() const;

	const char *GetResolvedForm(std::istream &input) const;

	const char *PresentResult(const std::vector<double> &results) const;

	const Result Solve(std::istream &input) const;
};


#endif //QTSOLVER_SOLVER_WRAP_H
