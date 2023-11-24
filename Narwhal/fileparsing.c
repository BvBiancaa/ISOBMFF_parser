#include "includes/challenge.h"

int	is_file_valid(char *filename)
{
	FILE *check;

	printf("%s FILENAME\n", filename);
	check = fopen(filename, "r");
	if (check == NULL)
		return (1);
	else
        	fclose(check);
	return (0);
}

void print_utf8_xml(const char *utf8_xml)
{

	for (size_t i = 0; utf8_xml[i] != '\0';)
	{
		uint32_t code_point = 0;
		size_t bytes_to_read = 0;
		if ((utf8_xml[i] & 0x80) == 0x00)
		{
			code_point = utf8_xml[i] & 0x7F;
			bytes_to_read = 1;
		}
		else if ((utf8_xml[i] & 0xE0) == 0xC0)
		{
			code_point = utf8_xml[i] & 0x1F;
			bytes_to_read = 2;
		}
		else if ((utf8_xml[i] & 0xF0) == 0xE0)
		{
			code_point = utf8_xml[i] & 0x0F;
			bytes_to_read = 3;
		}
		else if ((utf8_xml[i] & 0xF8) == 0xF0)
		{
			code_point = utf8_xml[i] & 0x07;
			bytes_to_read = 4;
		}
		else
		{
			printf("Invalid UTF-8 sequence at position %zu\n", i);
			break;
		}
		for (size_t j = 1; j < bytes_to_read; ++j)
		{
			if ((utf8_xml[i + j] & 0xC0) != 0x80)
			{
				printf("Invalid UTF-8 continuation byte at position %zu\n", i + j);
				break;
			}
			code_point = (code_point << 6) | (utf8_xml[i + j] & 0x3F);
		}
		i += bytes_to_read;
		printf("%c", code_point);
	}
}

int32_t	swapEndianness(int32_t value)
{
    	return ((value >> 24) & 0x000000FF) | ((value >> 8)  & 0x0000FF00) | ((value << 8)  & 0x00FF0000) | ((value << 24) & 0xFF000000);
}

long	process_mdat(char *filename)
{
	FILE *file = fopen(filename, "r");
	char line[4096];
	long ret;

	fseek(file, 189, SEEK_SET);
    	while (fgets(line, sizeof(line), file))
	     	print_utf8_xml(line);
	ret = ftell(file);
    	fclose(file);
    	return (ret);
}

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
			sub_box_size = swapEndianness(sub_box_size);
		#endif
		fread(sub_box_name, 1, 4, file);
		for (int y = 0; y < ind; y++)
			printf("	");
		printf("Box ID: %s ", sub_box_name);
		printf("of size: %u\n", sub_box_size);
		if (strcmp(sub_box_name, "moof") == 0 || strcmp(sub_box_name, "traf") == 0)
        		process_sub_boxes(file, sub_box_size, ind + 1);
		if (sub_box_size >= 8)
            	fseek(file, sub_box_size - 8, SEEK_CUR);
		else
		{
			printf("Invalid box size\n");
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
		perror("Error opening file");
		return ;
	}
	while (1)
	{
		//SIZE
		fread(&box_size, sizeof(int32_t), 1, file);
		if (feof(file) || ferror(file))
			FILE_OP_ERROR(file);
		#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
			box_size = swapEndianness(box_size);
		#endif
		//NAME
		fread(name, 1, 4, file);
		if (feof(file) || ferror(file))
			FILE_OP_ERROR(file);
		printf("Box ID: %s ", name);
		printf("of size: %u\n", box_size);
		//PARSE IS SUB BOXES OR CONTENT
		if (strcmp(name, "moof") == 0 || strcmp(name, "traf") == 0)
		{
			long start = ftell(file);
        		process_sub_boxes(file, box_size, 1);
			fseek(file, start, SEEK_SET);
		}
		if (strcmp(name, "mdat") == 0)
			fseek(file, process_mdat(filename), SEEK_SET);
		else if (box_size >= 8)
            	fseek(file, box_size - 8, SEEK_CUR);
		else
		{
			printf("Invalid box size\n");
			fclose(file);
			return;
		}
	}
	fclose(file);
	return ;
}