#include "ServerServiceTestSuite.hpp"
#include "RequestParserTestSuite.hpp"

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
