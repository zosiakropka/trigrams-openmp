all: analytics detection
	
analytics: objects objects/analytics.o objects/stats.o
	g++ -Wall -fopenmp objects/utils.o objects/stats.o objects/analytics.o -o analytics

detection: objects objects/detection.o objects/detector.o objects/utils.o objects/stats.o
	g++ -Wall -fopenmp objects/detector.o objects/utils.o objects/stats.o objects/detection.o -o detection

objects/detection.o: src/detection.cpp objects/utils.o
	gcc -c -Wall src/detection.cpp -o objects/detection.o

objects/analytics.o: src/analytics.cpp objects/utils.o
	gcc -c -Wall src/analytics.cpp -o objects/analytics.o

objects/stats.o: src/stats.cpp
	gcc -c -fopenmp -Wall src/stats.cpp -o objects/stats.o

objects/detector.o: src/detector.cpp
	gcc -c -fopenmp -Wall src/detector.cpp -o objects/detector.o

objects/utils.o: src/utils.cpp
	gcc -c -fopenmp -Wall src/utils.cpp -o objects/utils.o

objects:
	mkdir objects

clean:
	rm -rf objects
	rm -rf analytics
	rm -rf detection
