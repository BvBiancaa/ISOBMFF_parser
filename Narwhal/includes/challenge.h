#ifndef CHALLENGE_H
#define CHALLENGE_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include <arpa/inet.h>

#define EXIT_MALLOC_ERROR(buffer) \
{ \
	free(buffer); \
	write(1, "Malloc returned a NULL ptr\n", 27); \
	return (-1); \
}

#define FILE_OP_ERROR(file) \
{ \
	if (ferror(file)) \
		write(1, "Error handling file\n", 21); \
	fclose(file); \
	return ; \
}

// from file IOutils.c

char	*ft_scanf(void);

// from file fileparsing.c

int	is_file_valid(char *filename);
void	file_parser(char *filename);

#endif