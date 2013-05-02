do: domino
	./domino

domino: test.cpp domino.h domino.cpp gl.h gl.cpp kika.cpp mymath.h mymath.cpp
	g++ -pg -O3 -o domino -Wall $^ -lglut -lGLU -lGL -lsupc++ -lm

clean:
	rm domino domino.h.gch gmon.out
