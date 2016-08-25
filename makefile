all:main.cpp
	g++ main.cpp 
	./a.out>data.txt
	cp data.txt ../drawFunction/
