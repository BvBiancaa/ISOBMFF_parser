#include "includes/challenge.h"

int	main(int argc, char **argv)
{
	char	*buffer;

	if (argc > 1)
		if (is_file_valid(argv[1]) == 0)
		{
			file_parser(argv[1]);
			return (0);
		}
	buffer = NULL;
	while (1)
	{
		printf("Usage: %s path_to_file/filename\n", argv[0]);
		buffer = ft_scanf();
		if (buffer == NULL || buffer[0] == '\0')
			EXIT_MALLOC_ERROR(buffer);
		if (is_file_valid(buffer) == 0)
			break ;
	}
	file_parser(buffer);
	free(buffer);
  	return (0);
}