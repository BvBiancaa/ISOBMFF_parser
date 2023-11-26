#include "includes/challenge.h"

int	main(int argc, char **argv, char **env)
{
	char	*buffer;

	if (argc > 1)
		if (is_file_valid(argv[1]) == 0)
		{
			file_parser(argv[1], env);
			return (0);
		}
	buffer = NULL;
	while (1)
	{
		printf("Insert /path_to_file/filename. Press only Enter to exit the program.\n");
		buffer = ft_scanf();
		if (buffer == NULL || buffer[0] == '\0')
			EXIT_MALLOC_ERROR(buffer);
		if (is_file_valid(buffer) == 0)
			break ;
	}
	file_parser(buffer, env);
	free(buffer);
  	return (0);
}

//I love argc and argv, unfortunately for a person that has never used a program it's difficult to guess the argument before starting the program
//so there's argc and argv for people that used the program already (probably will not happen).
//the program checks if argc is more than one, so if the user did type the filename (or something random) and checks if it exists, if it does it just executes the program.
//If the argument is invalid or argc is 1, until the user inserts a valid parameter or enter to exit. it then parses the file and frees the buffer.
//the main also takes the **env as input to check if the user wants to save the files.
