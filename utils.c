#include "includes/challenge.h"

int32_t	swap_endianness(int32_t value)
{
    	return ((value >> 24) & 0x000000FF) | ((value >> 8)  & 0x0000FF00) | ((value << 8)  & 0x00FF0000) | ((value << 24) & 0xFF000000);
}

//the function is designed to swap the endianness of a 32-bit integer.
//the first value goes from pos 0 to pos 3 (shifted of 24 bits), the second goes from pos 1 to pos 2, the third goes from pos 2 to pos 1 and the last one goes from pos 3 to pos 0.

int	is_file_valid(char *filename)
{
	FILE *check;

	check = fopen(filename, "r");
	if (check == NULL)
		return (1);
	else
        	fclose(check);
	return (0);
}

//it opens the file, if there are errors the file is not valid, else it closes it and returns 0.

size_t	alloc_count(size_t start, char *metadata)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (metadata[start + i] != '"' && metadata[start + i])
		i++;
	start = ft_strstr(metadata, "imagetype=\"") + 1;
	while (metadata[start + count] != '"' && metadata[start + count])
		count++;
	return (count + i);
}

//the function counts how many bytes the filename needs.
//it looks for the image id and counts how many characters it contains and does the same for the file type, returning the sum of the two numbers

void	find_filename(char *filename, char *metadata, size_t start, size_t index)
{
	while (metadata[start + index] != '"' && metadata[start + index])
	{
		filename[index] = metadata[start + index];
		index++;
	}
	filename[index] = '.';
	start = ft_strstr(metadata, "imagetype=\"") - index + 1;
	while (metadata[start + index] != '"' && metadata[start + index])
	{
		filename[index + 1] = metadata[start + index];
		index++;
	}
	filename[index + 1] = '\0';
}

//the function finds the image id and the file type and puts them in the filename string, terminating it with a '\0' character 

