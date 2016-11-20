//
// Created by sslebedev on 23.10.16.
//

#include <vector>
#include <cstring>
#include <stdexcept>

#include <dlfcn.h>

#include "solver_wrap.h"
#include "solver_interface.h"

/**
 * ctor - only initializer for SolverWrap
 * @param pathSolver
 * @return
 * @throws
 */
SolverWrap::SolverWrap(const char *pathSolver)
{
	char *concretePath = new char[strlen(pathSolver) + 4];
	strcpy(concretePath, pathSolver);
	strcat(concretePath, ".so");
	dlHandle = dlopen(concretePath, RTLD_LAZY);
	delete[](concretePath);

	if (!dlHandle) {
		throw new std::runtime_error(concretePath);
	}

	funcGetUid = reinterpret_cast<FuncGetStr>(GetFunction("GetUid"));
	assert(funcGetUid);
	funcGetClassName = reinterpret_cast<FuncGetStr>(GetFunction("GetClassName"));
	assert(funcGetClassName);
	funcGetMethodName = reinterpret_cast<FuncGetStr>(GetFunction("GetMethodName"));
	assert(funcGetMethodName);
	funcGetDescription = reinterpret_cast<FuncGetStr>(GetFunction("GetDescription"));
	assert(funcGetDescription);
	funcGetResolvedForm = reinterpret_cast<FuncResolve>(GetFunction("GetResolvedForm"));
	assert(funcGetResolvedForm);
	funcPresentResult = reinterpret_cast<FuncPresent>(GetFunction("PresentResult"));
	assert(funcPresentResult);
	funcSolve = reinterpret_cast<FuncSolve>(GetFunction("Solve"));
	assert(funcSolve);
}

/**
 * dtor
 */
SolverWrap::~SolverWrap()
{
	if (!dlHandle) {
		return;
	}

	dlclose(dlHandle);
}

/**
 * Cross-platform wrap to get pointer-to-function
 * @param nameFunction
 * @return functor
 */
inline void *SolverWrap::GetFunction(const char *nameFunction) const
{
	auto function = dlsym(dlHandle, nameFunction);

	return function;
}

/**
 * Unique identifier of solver module.
 * Solvers with the same uid have identical in/out format
 * @return uuid
 */
const char *SolverWrap::GetUid() const
{
	return funcGetUid();
}

/**
 * Class of the solving problem
 * @return name
 */
const char *SolverWrap::GetClassName() const
{
	return funcGetClassName();
}

/**
 * Method to solve the problem
 * @return name
 */
const char *SolverWrap::GetMethodName() const
{
	return funcGetMethodName();
}

/**
 * Informal method description, help etc.
 * @return text
 */
const char *SolverWrap::GetDescription() const
{
	return funcGetDescription();
}

/**
 * Unifyed representation for given (parced and recognized) task
 * This can be not supported or partially supported for module
 * @param input
 * @return nullptr if cannot be resolved OR form
 */
const char *SolverWrap::GetResolvedForm(std::istream &input) const
{
	return funcGetResolvedForm(input);
}

/**
 * Text representation for result
 * @param results
 * @return text
 */
const char *SolverWrap::PresentResult(const std::vector<double> &results) const
{
	return funcPresentResult(results);
}

/**
 * Solve given task
 * @param input
 * @return Result
 */
const Result SolverWrap::Solve(std::istream &input) const
{
	return funcSolve(input);
}