#include "includes/challenge.h"

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

int	check_bytes(size_t *bytes, uint32_t *uncoded, char c)
{
	if ((c & 0x80) == 0x00)
	{
		*uncoded = c & 0x7F;
		*bytes = 1;
	}
	else if ((c & 0xE0) == 0xC0)
	{
		*uncoded = c & 0x1F;
		*bytes = 2;
	}
	else if ((c & 0xF0) == 0xE0)
	{
		*uncoded = c & 0x0F;
		*bytes = 3;
	}
	else if ((c & 0xF8) == 0xF0)
	{
		*uncoded = c & 0x07;
		*bytes = 4;
	}
	else
		return (0);
	return (1);
}


void print_s(const char *s)
{
	uint32_t uncoded;
	size_t bytes;

	for (size_t i = 0; s[i] != '\0';)
	{
		uncoded = 0;
		bytes = 0;
		if (check_bytes(&bytes, &uncoded, s[i]) == 0)
			break;
		for (size_t j = 1; j < bytes; ++j)
		{
			if ((s[i + j] & 0xC0) != 0x80)
				break;
			uncoded = (uncoded << 6) | (s[i + j] & 0x3F);
		}
		i += bytes;
		write(1, &uncoded, 1);
	}
}

//The function iterates through a given UTF-8 encoded XML string and prints the corresponding Unicode. Bitwise are useful for decodedification.
//It checks the leading bits of the current byte to determine the number of bytes in the UTF-8 sequence and extracts the relevant bits for each case.
//The if statements with bitwise operations (& and ==) are used to identify the type of UTF-8 sequence (1-byte, 2-byte, 3-byte, or 4-byte). (the function it calls does this)
//For each byte, it checks and extracts the continuation bits and appends them to the uncoded using bitwise left shifts and logical OR (|) operations.
//The loop continues until the required number of bytes for the UTF-8 sequence is processed.
//It then increments the main loop index (i) by the number of bytes read and writes the calculated Unicode code point to the standard output. 
//If an invalid UTF-8 sequence or continuation byte is encountered, it prints an error message and breaks out of the loop.

int32_t	swap_endianness(int32_t value)
{
    	return ((value >> 24) & 0x000000FF) | ((value >> 8)  & 0x0000FF00) | ((value << 8)  & 0x00FF0000) | ((value << 24) & 0xFF000000);
}

// the function is designed to swap the endianness of a 32-bit integer.
//the first value goes from pos 0 to pos 3 (shifted of 24 bits), the second goes from pos 1 to pos 2, the third goes from pos 2 to pos 1 and the last one goes from pos 3 to pos 0.

long	process_mdat(char *filename)
{
	FILE *file = fopen(filename, "r");
	char line[4096];
	long ret;

	fseek(file, 189, SEEK_SET);
    	while (fgets(line, sizeof(line), file))
	     	print_s(line);
	ret = ftell(file);
    	fclose(file);
    	return (ret);
}

//it reads the data from mdat and prints it uncoded.

void process_sub_boxes(FILE *file, int32_t box_size, int ind) 
{
	int32_t sub_box_size;
	char sub_box_name[5] = {0};

	box_size -= 8;
	while (box_size > 0)
	{
		fread(&sub_box_size, sizeof(int32_t), 1, file);
		if (feof(file) || ferror(file))
			FILE_OP_ERROR(file);
		#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
			sub_box_size = swap_endianness(sub_box_size);
		#endif
		fread(sub_box_name, 1, 4, file);
		for (int y = 0; y < ind; y++)
			printf("	");
		printf("Box ID: %s ", sub_box_name);
		printf("of size: %u\n", sub_box_size);
		if (ft_strcmp(sub_box_name, "moof") == 0 || ft_strcmp(sub_box_name, "traf") == 0)
        		process_sub_boxes(file, sub_box_size, ind + 1);
		if (sub_box_size >= 8)
            	fseek(file, sub_box_size - 8, SEEK_CUR);
		else
		{
			ft_putstr("Invalid box size\n");
			return;
		}
		box_size -= sub_box_size;
	}
}

void	file_parser(char *filename)
{
	int32_t	box_size;
	char	name[5] = {0};
	FILE *file;

	file = fopen(filename, "r");
	if (file == NULL)
	{
		write(2, "Error opening file\n", 20);
		return ;
	}
	while (1)
	{
		//GET SIZE
		fread(&box_size, sizeof(int32_t), 1, file);
		if (feof(file) || ferror(file))
			FILE_OP_ERROR(file);
		#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
			box_size = swap_endianness(box_size);
		#endif
		//GET NAME
		fread(name, 1, 4, file);
		if (feof(file) || ferror(file))
			FILE_OP_ERROR(file);
		printf("Box ID: %s ", name);
		printf("of size: %u\n", box_size);
		//PARSE ITS SUB BOXES OR CONTENT OR JUST GO ON
		if (ft_strcmp(name, "moof") == 0 || ft_strcmp(name, "traf") == 0)
		{
			long start = ftell(file);
        		process_sub_boxes(file, box_size, 1);
			fseek(file, start, SEEK_SET);
		}
		if (ft_strcmp(name, "mdat") == 0)
			fseek(file, process_mdat(filename), SEEK_SET);
		else if (box_size >= 8)
            	fseek(file, box_size - 8, SEEK_CUR);
		else
		{
			ft_putstr("Invalid box size\n");
			fclose(file);
			return;
		}
	}
	fclose(file);
	return ;
}

//the function 
