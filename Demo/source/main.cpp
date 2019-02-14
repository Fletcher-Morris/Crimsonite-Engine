namespace Crimson
{
	_declspec(dllimport) void Test();
}

int main()
{
	Crimson::Test();
	while (true) {};
	return 0;
}