build:
	gcc -Wall -Wextra -Werror -pedantic ipkcp.c -o ipkcpc
clean:
	rm -f ipkcpc *.o
