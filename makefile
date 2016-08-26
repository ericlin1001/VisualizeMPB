all:run

run:main.cpp
	g++ -std=c++11  main.cpp 
	./a.out

genData:main.cpp
	g++ -std=c++11  main.cpp 
	./a.out>data.txt
	cp data.txt ../drawFunction/

drawData:genData
	cd ../drawFunction/ && python3 drawHist.py

rmdata:*
	rm *.txt
