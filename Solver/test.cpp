#include <iostream>
#include <gtest/gtest.h>

#include "solver_wrap.h"

TEST(SolverMain, InitDeinit)
{
	SolverWrap *wrap;
	ASSERT_NO_THROW(wrap = new SolverWrap("libSolver"));
	ASSERT_NO_THROW(delete(wrap));
}

int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}