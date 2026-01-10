CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra
SFML_DIR = /opt/homebrew
INCLUDES = -I$(SFML_DIR)/include
LIBS = -L$(SFML_DIR)/lib -lsfml-graphics -lsfml-window -lsfml-system

main: main.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) main.cpp -o main $(LIBS)
	./main

clean:
	rm -f main

.PHONY: clean
