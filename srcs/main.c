#include "utils.c"

int main(int argc, char **argv)
{
	(void)argv;
	if (argc == 0)
	{
		printf("Not enough arguments ");
		exit(0);
	}
	printf("Test compilazione");
	return (0);
}
