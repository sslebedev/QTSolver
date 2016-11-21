//
// Created by sslebedev on 23.10.16.
//

#include <iostream>
#include <cmath>
#include <array>
#include <vector>
#include <sstream>

#include "../SolverInterface/solver_interface.h"

static const char *Uid = "54f61d5b-cb47-48e6-b80e-459eb2ed04b5";

static const char *ClassName = "System of Linear Equations";

static const char *MethodName = "Jacobi";

static const char *Description = "Jacobi method is an algorithm "
		"for determining the solutions "
		"of a diagonally dominant system of linear equations.";


const char *GetUid()
{
	return Uid;
}

const char *GetClassName()
{
	return ClassName;
}

const char *GetMethodName()
{
	return MethodName;
}

const char *GetDescription()
{
	return Description;
}

const char *GetResolvedForm(std::istream &input)
{
	// not supported for this class by design
	return nullptr;
}

const char *PresentResult(const std::vector<double> &args)
{
	std::ostringstream pres;
	pres << "[";
	for (int i = 0, n = (int) args.size(); i < n; ++i) {
		pres << args[i];
		if (i < n - 1) {
			pres << "; ";
		}
	}
	pres << "]";
	return pres.str().c_str();
}

const Result Jacobi(const ParsedTask &parsedTask);

class Task : public ParsedTask::BaseTask
{
public:
	virtual ~Task() override
	{}

	std::vector<double> a;
	std::vector<double> b;
	double eps;
};

const ParsedTask Parse(std::istream &input)
{
	char separator;
	double value;
	int n = -1;
	int row = 0;

	auto task = new Task();

	try {
		input >> task->eps;

		input >> separator;
		if (separator != '[') {
			throw std::string("Matrix A expected");
		}

		do {
			input >> value >> separator;
			task->a.push_back(value);
			if (separator == ',') {
				continue;
			}
			if (separator == ';' || separator == ']') {
				if (n == -1) {
					n = task->a.size();
				}
				if (task->a.size() != n * (row + 1)) {
					std::stringstream stream;
					stream << "Wrong matrix A input, row " << row
						   << ": expected " << n << " elements, "
						   << "have " << task->a.size() - n * row;
					throw stream.str();
				}
				++row;
				continue;
			}
			std::stringstream stream;
			stream << "Wrong matrix A input, row " << row
				   << ": wrong separator";
			throw stream.str();
		} while (separator != ']');

		if (row != n) {
			std::stringstream stream;
			stream << "Wrong matrix A metrics, rowsXcolumns: "
				   << row << "X" << n
				   << ", rows = columns expected";
			throw stream.str();
		}

		input >> separator;
		if (separator != '[') {
			throw std::string("Vector B expected");
		}

		do {
			input >> value >> separator;
			task->b.push_back(value);
			if (separator == ',') {
				std::stringstream stream;
				stream << "Wrong vector B input, row " << row
					   << ": many values in row, column vector expected";
				throw stream.str();
			}
			if (separator == ';' || separator == ']') {
				continue;
			}
			std::stringstream stream;
			stream << "Wrong vector B input, row " << row
				   << ": wrong separator";
			throw stream.str();
		} while (separator != ']');

		if (task->b.size() != n) {
			std::stringstream stream;
			stream << "Wrong vector B metrics, rows: "
				   << task->b.size() << ", but " << n << " expected";
			throw stream.str();

		}
		return ParsedTask(GetUid(), task);
	} catch (std::string what) {
		delete (task);
		ParsedTask(GetUid(), what);
	}
}

const Result SolveParsedTask(const ParsedTask &parsedTask)
{
	if (!parsedTask.IsOk()) {
		std::stringstream stream;
		stream << "Parsed task is invalid: "
			   << parsedTask.GetErrorExplanation();
		return Result(GetUid(), stream.str());
	}

	if (parsedTask.GetMethodUid() != GetUid()) {
		std::stringstream stream;
		stream << "Method uid of solver ("
			   << GetUid() << ", " << GetClassName() << ", " << GetMethodName() << ") "
			   << "and uid of parsedTask ("
			   << GetUid() << ", " << GetClassName() << ", " << GetMethodName() << ") "
			   << "are incompatible";
		return Result(GetUid(), stream.str());
	}

	return Jacobi(parsedTask);
}

const Result Solve(std::istream &input)
{
	auto parsedTask = Parse(input);

	return SolveParsedTask(parsedTask);
}

const Result Jacobi(const ParsedTask &parsedTask)
{
	auto a = static_cast<Task *>(parsedTask.GetTask())->a;
	auto b = static_cast<Task *>(parsedTask.GetTask())->b;
	auto eps = static_cast<Task *>(parsedTask.GetTask())->eps;
	auto n = (int) b.size();
	auto x = b;
	auto tx = std::vector<double>(n);

	auto q = fabs(b[0]);
	for (int i = 1; i < n; i++) {
		if (fabs(b[i]) > q) {
			q = fabs(b[i]);
		}
	}
	if (q > 1) {
		return Result(GetUid(), "|B[i]| <= 1 expected");
	}

	double norm;
	do {
		for (int i = 0; i < n; i++) {
			tx[i] = b[i];
			for (int j = 0; j < n; j++) {
				if (i != j) {
					tx[i] -= a[i * n + j] * x[j];
				}
			}
			tx[i] /= a[i * n + i];
		}
		norm = fabs(x[0] - tx[0]);
		for (int i = 0; i < n; i++) {
			if (fabs(x[i] - tx[i]) > norm) {
				norm = fabs(x[i] - tx[i]);
			}
			x[i] = tx[i];
		}
	} while (norm > eps);

	return Result(GetUid(), eps, std::move(x));
}
