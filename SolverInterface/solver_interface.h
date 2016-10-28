//
// Created by sslebedev on 26.10.16.
//

#ifndef QTSOLVER_SOLVER_INTERFACE_H
#define QTSOLVER_SOLVER_INTERFACE_H

#if defined _WIN32 || defined __CYGWIN__
	#ifdef BUILDING_DLL
		#ifdef __GNUC__
			#define DLL_PUBLIC __attribute__ ((dllexport))
		#else
			#define DLL_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
		#endif
	#else
		#ifdef __GNUC__
			#define DLL_PUBLIC __attribute__ ((dllimport))
		#else
			#define DLL_PUBLIC __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
		#endif
	#endif
	#define DLL_LOCAL
#else
	#if __GNUC__ >= 4
		#define DLL_PUBLIC __attribute__ ((visibility ("default")))
		#define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
	#else
		#define DLL_PUBLIC
		#define DLL_LOCAL
	#endif
#endif

class Result
{

};

/**
 * Unique identifier of solver module.
 * Solvers with the same uid have identical in/out format
 * @return uuid
 */
extern "C" DLL_PUBLIC const char *GetUid();

/**
 * Class of the solving problem
 * @return name
 */
extern "C" DLL_PUBLIC const char *GetClassName();

/**
 * Method to solve the problem
 * @return name
 */
extern "C" DLL_PUBLIC const char *GetMethodName();

/**
 * Informal method description, help etc.
 * @return text
 */
extern "C" DLL_PUBLIC const char *GetDescription();

/**
 * Unifyed representation for given (parced and recognized) task.
 * The same task even writen in defferent form shold have the same resolved form.
 * This can be not supported or partially supported for module
 * @param input
 * @return nullptr if cannot be resolved OR form
 */
extern "C" DLL_PUBLIC const char *GetResolvedForm(const std::istream &input);

/**
 * Text representation for result
 * @param results
 * @return text
 */
extern "C" DLL_PUBLIC const char *PresentResult(const std::vector<double> &results);

/**
 * Solve given task
 * @param input
 * @return Result
 */
extern "C" DLL_PUBLIC const Result *Solve(const std::istream &input);

#endif //QTSOLVER_SOLVER_INTERFACE_H
