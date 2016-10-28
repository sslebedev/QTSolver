#include <gtest/gtest.h>
#include <fstream>

#include "solver_wrap.h"

class FixSolver : public ::testing::Test
{
protected:
	SolverWrap *wrap;
	std::istream *istream;

public:
	FixSolver()
	{
		istream = new std::ifstream();
	}

	virtual ~FixSolver()
	{
		delete (istream);
	}

protected:

	void SetUp()
	{
		ASSERT_NO_THROW(wrap = new SolverWrap("libSolver"));
	}

	void TearDown()
	{
		ASSERT_NO_THROW(delete (wrap));
	}
};

TEST_F(FixSolver, AllFunctionsPresented)
{
	EXPECT_STREQ("54f61d5b-cb47-48e6-b80e-459eb2ed04b5", wrap->GetUid());
	EXPECT_STREQ("System of Linear Equations", wrap->GetClassName());
	EXPECT_STREQ("Jacobi", wrap->GetMethodName());
	EXPECT_STREQ("Jacobi method is an algorithm "
						 "for determining the solutions "
						 "of a diagonally dominant system of linear equations.", wrap->GetDescription());


	EXPECT_EQ(nullptr, wrap->GetResolvedForm(*istream));

	EXPECT_STREQ("[1; 2; 3; 4.0001]", wrap->PresentResult({1, 2, 3.0, 4.0001}));

	EXPECT_EQ(nullptr, wrap->Solve(*istream));
}

int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}