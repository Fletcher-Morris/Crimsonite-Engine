#include <string>

namespace Crimson
{
	_declspec(dllimport) void Test();
}

int main()
{
	Crimson::Test();
	return 0;
}