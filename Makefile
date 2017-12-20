
build: hangman

hangman: Hangman.c
	gcc -Wall -g Hangman.c -o hangman -lpanel -lncurses

run: hangman
	./hangman

clean:
	rm -r hangman 

.PHONY: build run clean


