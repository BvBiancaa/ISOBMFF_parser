#include "includes/challenge.h"

unsigned char *base64_decode(const char *data, size_t input_length, size_t *output_length, int decoding_table[256])
{
	unsigned char *decoded_data;
	uint32_t triple;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (input_length % 4 != 0)
		return NULL;
	if (data[input_length - 1] == '=') (*output_length)--;
	if (data[input_length - 2] == '=') (*output_length)--;
	decoded_data = malloc(*output_length + 2);
	if (decoded_data == NULL)
		return NULL;
	while (i < input_length)
	{
		triple = (decoding_table[(unsigned char)data[i++]] << 3 * 6) + (decoding_table[(unsigned char)data[i++]] << 2 * 6) + (decoding_table[(unsigned char)data[i++]] << 1 * 6) + (decoding_table[(unsigned char)data[i++]] << 0 * 6);
		decoded_data[j++] = (triple >> 2 * 8) & 0xFF;
		decoded_data[j++] = (triple >> 1 * 8) & 0xFF;
		decoded_data[j++] = (triple >> 0 * 8) & 0xFF;
	}
	return (decoded_data);
}

//The base64_decode function checks if the length of the input data is divisible by 4 (else the input is malformed).
//If some padding characters ('=') are present, it adjusts the output length accordingly.
//It allocates memory for the output (we already know its 3/4 of the input length)
//The function then iterates through the input data, and converts each group of four Base64 characters into a 24-bit triple and then splitting this triple into three 8-bit values.
//Returns a pointer to the allocated memory containing the decoded data.

void save_metadata_to_file(const char *base_64_data, FILE *file, int decoding_table[256])
{
	size_t decoded_size = strlen(base_64_data) * 3 / 4;
	unsigned char *decoded;
	
	decoded = base64_decode(base_64_data, strlen(base_64_data), &decoded_size, decoding_table);
	if (decoded == NULL)
	{
		write(2, "Error decoding metadata\n", 24);
		return ;
	}
	fwrite(decoded, 1, decoded_size, file);
	free(decoded);
}

//the function takes the encoded data and sends it to the decoding function, it then writes the decoded data (if valid) in the file, it closes it and frees the decoded string.

void	check_metadat(char *metadata, int decoding_table[256])
{
	FILE *file;
	ssize_t	i;
	ssize_t	start;
	char		*filename;
	char		*encoded_file;

	start = ft_strstr(metadata, "<smpte:image xml:id=\"") + 1;
	if (start == -1)
		return (ft_putstr("Metadata malformed\n"));
	filename = malloc(alloc_count(start, metadata) + 2);
	if (!filename)
	{
		write(2, "malloc error\n", 14);
		return ;
	}
	find_filename(filename, metadata, start, 0);
	file = fopen(filename, "wb");
	if (file == NULL)
		return (free(filename));
	i = ft_strstr(metadata, "</smpte:image>\n") - 16 - ft_strstr(metadata, "encoding=\"Base64\">\n") + 3;
	encoded_file = malloc(i);
 	if (!encoded_file)
	{
		write(2, "malloc error\n", 14);
		return ;
	}
	i = 0;
	start = ft_strstr(metadata, "encoding=\"Base64\">\n") + 1;
	while (metadata[start + i] != '\n' && metadata[start + i])
	{
		encoded_file[i] = metadata[start + i];
		i++;
	}
	encoded_file[i] = '\0';
	save_metadata_to_file(encoded_file, file, decoding_table);
	free(filename);
	free(encoded_file);
	fclose(file);
	if (ft_strstr(metadata + start, "<smpte:image xml:id=\"") > 0)
		check_metadat(metadata + ft_strstr(metadata, "</smpte:image>\n"), decoding_table);
	return ;
}

//a recursive function that analizes all the files contained in the <metadata> tag.
//it finds the image name (which will be the same name of the file) and its content, sending it to the function that decodes is.
//it then frees the filename and the encoded file and calls itself again, until it reaches the end of the metadata tag.

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

void	print_s(const char *s, char *modify)
{
	uint32_t uncoded;
	size_t bytes;
	static int y;

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
		modify[y] = uncoded;
		y++;
	}
}

//The function iterates through a given UTF-8 encoded XML string and prints the corresponding Unicode.
//It checks the leading bits of the current byte to determine the number of bytes in the UTF-8 sequence and extracts the relevant bits for each case.
//The if statements with bitwise operations (& and ==) are used to identify the type of UTF-8 sequence (1-byte, 2-byte, 3-byte, or 4-byte). (the function it calls does this)
//For each byte, it checks and extracts the continuation bits and appends them to the uncoded using bitwise left shifts and logical OR (|) operations.
//The loop continues until the required number of bytes for the UTF-8 sequence is processed.
//It then increments the main loop index (i) by the number of bytes read and writes the calculated Unicode code point to the standard output. 
//If an invalid UTF-8 sequence or continuation byte is encountered, it prints an error message and breaks out of the loop.

long	process_mdat(char *filename, int32_t size)
{
	FILE *file = fopen(filename, "r");
	char line[4096];
	char	*metadata;
	long ret;
	int decoding_table[256];

	metadata = malloc(size);
	if (!metadata)
	{
		write(2, "Malloc error\n", 14);
		return (-1);
	}
	ft_putstr("Mdat content:\n");
	fseek(file, 189, SEEK_SET);
    	while (fgets(line, sizeof(line), file))
	     	print_s(line, metadata);
	metadata[size - 1] = '\0';
	ret = ftell(file);
      for (int i = 0; i < 64; i++)
            decoding_table[(unsigned char) "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;
	check_metadat(metadata, decoding_table);
	free(metadata);
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
	int32_t	position;
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
		{
			position = process_mdat(filename, box_size - 8);
			if (position == -1)
				return ;
			fseek(file, position, SEEK_SET);
		}
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

//the function opens the file, and loops. it reads once the size of int32_t once (so 4 bytes) from the file and puts the value in box_size.
//since the value is represented in big-endinan notation
//(the most significant byte (the "big end") is stored at the lowest memory address, and the least significant byte is stored at the highest memory address.)
//not every computer architecture is based on big-endian (x86 and x86-64 architectures commonly use little-endian), the program checks if the architecture
//uses little endian, in that case the box_size will be converted and the bytes will be swapped.
//then the program reads the box name, one charachter four times (so still 4 bytes) and stores it in name. If there are no errors the box infos will be printed.
//It checks if the boxes can contain other boxes, in that case it does the exact same thing in the function "process sub boxes" and recusively checks if the sub-box can contain other sub boxes.
//If the box is mdat it calls the function process_mdat, that will print the uncoded content of the box.
//If the box size is greater than 8 it will skip the box content, else it will print an error message and return since the box is not valid.
//The function will loop until it reaches the end of the file.
