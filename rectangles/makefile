CXX      = g++
CXXFLAGS = -Wall -pedantic -std=c++11
EXEC     = rectangles

all: $(EXEC)

run: $(EXEC)
	./test.sh

clean:
	rm -f $(EXEC) *~ ./test/*my*

rectangles: rectangles.c
	$(CXX) $(CXXFLAGS) $< -o $@

