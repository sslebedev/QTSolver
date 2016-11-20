#include <gtest/gtest.h>
#include <fstream>
#include <cmath>

#include "solver_interface.h"
#include "solver_wrap.h"

class FixSolver : public ::testing::Test
{
protected:
	SolverWrap *wrap;
	std::istream *istream;
	std::vector<double> answer;
	double eps;

public:
	FixSolver()
	{
		istream = new std::istringstream("0.000001 [0.5, 0.5; -0.142857143, 0.285714286] [1; 1]");
		answer = {1, 1};
		eps = 0.000001;
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

// linPack - tests
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

	auto solution = wrap->Solve(*istream);
	EXPECT_TRUE(solution.IsOk());
	if (solution.IsOk()) {
		std::cout << wrap->PresentResult(solution.GetResult()) << std::endl;
		for (int i = 0; i < 0; ++i) {
			EXPECT_TRUE(eps = solution.GetEps());
			EXPECT_TRUE(fabs(answer[i] - solution.GetResult()[i]) <= eps);
		}
	} else {
		std::cout << solution.GetErrorExplanation() << std::endl;
	}

}

int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}