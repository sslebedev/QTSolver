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
	for (int i = 0, n = (int)args.size(); i < n; ++i) {
		pres << args[i];
		if (i < n - 1) {
			pres << "; ";
		}
	}
	pres << "]";
	return pres.str().c_str();
}

const Result Jacobi(const std::vector<double> &a, const std::vector<double> &b, double eps);

const Result Solve(std::istream &input)
{
	char separator;
	double value;
	int n = -1;
	int row = 0;

	double eps;

	input >> eps;

	std::vector<double> a;

	input >> separator;
	if (separator != '[') {
		return Result(GetUid(), "Matrix A expected");
	}

	do {
		input >> value >> separator;
		a.push_back(value);
		if (separator == ',') {
			continue;
		}
		if (separator == ';' || separator == ']') {
			if (n == -1) {
				n = a.size();
			}
			if (a.size() != n * (row + 1)) {
				std::stringstream stream;
				stream << "Wrong matrix A input, row " << row
					   << ": expected " << n << " elements, "
					   << "have " << a.size() - n * row;
				return Result(GetUid(), stream.str());
			}
			++row;
			continue;
		}
		std::stringstream stream;
		stream << "Wrong matrix A input, row " << row
			   << ": wrong separator";
		return Result(GetUid(), stream.str());
	} while (separator != ']');

	if (row != n) {
		std::stringstream stream;
		stream << "Wrong matrix A metrics, rowsXcolumns: "
			   << row << "X" << n
			   << ", rows = columns expected";
		return Result(GetUid(), stream.str());
	}

	std::vector<double> b;

	input >> separator;
	if (separator != '[') {
		return Result(GetUid(), "Vector B expected");
	}

	do {
		input >> value >> separator;
		b.push_back(value);
		if (separator == ',') {
			std::stringstream stream;
			stream << "Wrong vector B input, row " << row
				   << ": many values in row, column vector expected";
			return Result(GetUid(), stream.str());
		}
		if (separator == ';' || separator == ']') {
			continue;
		}
		std::stringstream stream;
		stream << "Wrong vector B input, row " << row
			   << ": wrong separator";
		return Result(GetUid(), stream.str());
	} while (separator != ']');

	if (b.size() != n) {
		std::stringstream stream;
		stream << "Wrong vector B metrics, rows: "
			   << b.size() << ", but " << n << " expected";
		return Result(GetUid(), stream.str());
	}

	return Jacobi(a, b, eps);
}

const Result Jacobi(const std::vector<double> &a, const std::vector<double> &b, double eps)
{
	auto n = (int)b.size();
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
