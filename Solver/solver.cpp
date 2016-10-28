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

const char *GetResolvedForm(const std::istream &input)
{
	// not supported for this class by my will
	return nullptr;
}

const char *PresentResult(const std::vector<double> &args)
{
	std::ostringstream pres;
	pres << "[";
	for (int i = 0, n = args.size(); i < n; ++i) {
		pres << args[i];
		if (i < n - 1) {
			pres << "; ";
		}
	}
	pres << "]";
	return pres.str().c_str();
}

const Result *Solve(const std::istream &input)
{
	return nullptr;
}

static void Jacobi(int N, double **A, double *B, double *X, double eps)
{
	double *TempX = new double[N];
	double norm;

	do {
		for (int i = 0; i < N; i++) {
			TempX[i] = B[i];
			for (int g = 0; g < N; g++) {
				if (i != g) {
					TempX[i] -= A[i][g] * X[g];
				}
			}
			TempX[i] /= A[i][i];
		}
		norm = fabs(X[0] - TempX[0]);
		for (int h = 0; h < N; h++) {
			if (fabs(X[h] - TempX[h]) > norm) {
				norm = fabs(X[h] - TempX[h]);
			}
			X[h] = TempX[h];
		}
	} while (norm > eps);
	delete[] TempX;
}