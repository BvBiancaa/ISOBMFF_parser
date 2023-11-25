NAME = narwhal

all: {NAME}

{NAME}:
	cc -g challenge.c fileparsing.c function_impl.c utils.c -o narwhal
	@clear
	@echo "compiled"
	@echo "usage: ./narwhal /path_to_file/file_name or just ./narwhal"

fclean:
	rm -f narwhal
	@clear
	@echo "executable removed"

re: fclean {NAME}

.PHONY: all, re
