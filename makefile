
all: analytics_test detection_test

analytics_test: objects objects/timer.o objects/utils.o objects/stats.o objects/analytics_test.o
	g++ -Wall -fopenmp objects/timer.o objects/utils.o objects/stats.o objects/analytics_test.o -o analytics_test

detection_test: objects objects/timer.o objects/utils.o objects/stats.o objects/detection_test.o objects/detector.o
	g++ -Wall -fopenmp objects/timer.o objects/utils.o objects/stats.o objects/detector.o objects/detection_test.o -o detection_test

objects/detection_test.o: src/detection_test.cpp
	gcc -c -Wall src/detection_test.cpp -o objects/detection_test.o

objects/analytics_test.o: src/analytics_test.cpp
	gcc -c -Wall src/analytics_test.cpp -o objects/analytics_test.o

objects/stats.o: src/stats.cpp
	gcc -c -fopenmp -Wall src/stats.cpp -o objects/stats.o

objects/detector.o: src/detector.cpp
	gcc -c -fopenmp -Wall src/detector.cpp -o objects/detector.o

objects/utils.o: src/utils.cpp
	gcc -c -fopenmp -Wall src/utils.cpp -o objects/utils.o

objects/timer.o: src/timer.cpp
	gcc -c -fopenmp -Wall src/timer.cpp -o objects/timer.o

objects:
	mkdir objects

clean:
	rm -rf objects
	rm -rf analytics_test
	rm -rf detection_test

clean_dat:
	rm *.dat
