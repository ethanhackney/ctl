main: /home/ethanhackney/code/ctl/test/arr.c /home/ethanhackney/code/ctl/lib
	gcc -Wall -Werror -pedantic -DDBUG -fsanitize=address,undefined $^
