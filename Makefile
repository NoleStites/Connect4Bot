connect4: connect4.cc game.cc ai.cc game.h ai.h
	g++ -g connect4.cc game.cc ai.cc -lncurses -o connect4 -Wall -Wextra

.PHONY: clean
clean:
	rm -f connect4
