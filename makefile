# script to compile the project
# type 'make' in the terminal to compile the project

compiled.bin: *.cpp
	g++ -std=c++17 -o output/render.out *.cc