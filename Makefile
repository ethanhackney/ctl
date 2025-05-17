main: /home/ethanhackney/code/ctl/test/arr.c /home/ethanhackney/code/ctl/lib/lib.c
	gcc -Wall -Werror -pedantic -DDBUG -fsanitize=address,undefined $^
