	#include <stdio.h>
	#include "get_next_line.h"
	int main(void)
	{
		int ret = 1;
		char *line;

		while (ret)
		{
			ret = get_next_line(&line);
			printf("%s\n", line);
			free(line);
		}
		return (0);
	}
