//
// Created by sslebedev on 23.10.16.
//

#include <iostream>

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

const char *PresentResult(double args[])
{
	return "I'm a correct vector-column";
}

const Result *Solve(const std::istream &input)
{
	return nullptr;
}