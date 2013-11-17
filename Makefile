all: run

run: objectTracking
	./objectTracking

objectTracking: objectTracking.cpp
	g++ objectTracking.cpp `pkg-config --cflags --libs opencv` -lXtst -o objectTracking