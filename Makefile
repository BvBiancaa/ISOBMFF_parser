all: narwhal

narwhal:
	cc -g challenge.c utils.c function_impl.c fileparsing.c -o narwhal

fclean:
	rm -f narwhal

re: fclean narwhal

.PHONY: all, re
