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

#include <cassert>
#include <string>
#include <stdexcept>

class Result
{
public:
	Result(const std::string &methodUid, const std::string &errorExplanation)
			: methodUid(methodUid.c_str())
			  , errorExplanation(errorExplanation)
	{
		assert(errorExplanation != "");
	}

	Result(const std::string &methodUid, double eps, const std::vector<double> &result)
			: methodUid(methodUid.c_str())
			  , errorExplanation("")
			  , result(result)
			  , eps(eps)
	{}

	bool IsOk() const
	{
		return errorExplanation.size() == 0;
	}

	const std::vector<double> &GetResult() const
	{
		if (!IsOk()) {
			throw std::logic_error("Bad Result");
		}
		return result;
	}

	double GetEps() const
	{
		if (!IsOk()) {
			throw std::logic_error("Bad Result");
		}
		return eps;
	}

	const char *GetMethodUid() const
	{
		return methodUid;
	}

	const std::string &GetErrorExplanation() const
	{
		return errorExplanation;
	}

private:
	const std::vector<double> result;
	double eps;
	const char *methodUid;
	const std::string errorExplanation;
};

class ParsedTask
{
public:
	class BaseTask
	{
	public:
		virtual ~BaseTask() {};
	};

	ParsedTask(const char *methodUid, BaseTask *task)
			: methodUid(methodUid)
			  , task(task)
			  , errorExplanation("")
	{}

	ParsedTask(const char *methodUid, const std::string &errorExplanation)
			: methodUid(methodUid)
			  , task(nullptr)
			  , errorExplanation(errorExplanation)
	{}

	~ParsedTask()
	{
		if (task != nullptr) {
			delete (task);
		}
	}

	BaseTask *GetTask() const
	{
		return task;
	}

	const char *GetMethodUid() const
	{
		return methodUid;
	}

	bool IsOk() const
	{
		return errorExplanation.size() == 0;
	}

	const std::string &GetErrorExplanation() const
	{
		return errorExplanation;
	}

private:
	const char *methodUid;
	BaseTask *task;
	std::string errorExplanation;
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
extern "C" DLL_PUBLIC const char *GetResolvedForm(std::istream &input);

/**
 * Text representation for result
 * @param results
 * @return text
 */
extern "C" DLL_PUBLIC const char *PresentResult(const std::vector<double> &results);

/**
 * Parse input as task
 * @param input
 * @return ParsedTask
 */
extern "C" DLL_PUBLIC const ParsedTask Parse(std::istream &input);

/**
 * Solve given parsed task
 * @param parsedTask
 * @return Result
 */
extern "C" DLL_PUBLIC const Result SolveParsedTask(const ParsedTask &parsedTask);

/**
 * Parse input and solve task
 * @param input
 * @return Result
 */
extern "C" DLL_PUBLIC const Result Solve(std::istream &input);

#endif //QTSOLVER_SOLVER_INTERFACE_H
