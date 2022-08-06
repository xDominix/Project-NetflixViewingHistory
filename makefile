compile:
	g++ csvread.cpp factory.cpp builder.cpp mylib.cpp main.cpp -o main -std=c++11

test:
	clear
	make compile
	./main "NetflixViewingHistory.csv"

clean:
	rm -f main
