all: narwhal

narwhal:
	cc -g challenge.c utils.c fileparsing.c -o narwhal

fclean:
	rm -f narwhal

re: fclean narwhal

.PHONY: all, re