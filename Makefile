# Variables
CXX = clang++
CXXFLAGS = -std=c++17 -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# Target name
TARGET = DyingAstronaut

# Source files
SRC = main.cpp

# Default rule
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

# Rule to run the game
run: all
	./$(TARGET)

# Rule to clean up
clean:
	rm -f $(TARGET)