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
	return (-1); \
}

#define FILE_OP_ERROR(file) \
{ \
	if (ferror(file)) \
		write(1, "Error handling file\n", 21); \
	fclose(file); \
	return ; \
}

// from file function_impl.c

char	*ft_scanf(void);
void	ft_putstr(char *s);
int	ft_strcmp(char *s1, char *s2);
int	ft_strstr(const char *haystack, const char *needle);

// from file fileparsing.c

int	is_file_valid(char *filename);
void	file_parser(char *filename);

//from file utils.c

int32_t	swap_endianness(int32_t value);
int	is_file_valid(char *filename);
size_t	alloc_count(size_t start, char *metadata);
void	find_filename(char *filename, char *metadata, size_t start, size_t index);


#endif