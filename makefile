compile:
	g++ csvread.cpp factory.cpp builder.cpp mylib.cpp main.cpp -o main

run:
	clear
	make compile
	./main "NetflixViewingHistoryEng.csv"

clean:
	rm -f main