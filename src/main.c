#include <stdio.h>

int main(void)
{
#ifdef _DEBUG
	printf("%s\n", "Hello World!");
#endif
	return 0;
}