#include <print>

int main()
{
	try
	{
		throw std::exception("Hello World!");
	}
	catch(const std::exception& e)
	{
		std::println(stderr, "Error: {}", e.what());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}