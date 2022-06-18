CXX = g++
objects = ex3.o producer.o dispatcher.o coeditor.o screen_manager.o infrastructure.o

all: ex3.out

ex3.out: $(objects)
	    $(CXX) -o ex3.out $(objects)

ex3.o : ex3.cpp ex3.h
	        $(CXX) -c ex3.cpp
producer.o : producer.cpp ex3.h
	        $(CXX) -c producer.cpp
dispatcher.o : dispatcher.cpp ex3.h
	        $(CXX) -c dispatcher.cpp
coeditor.o : coeditor.cpp ex3.h
	        $(CXX) -c coeditor.cpp
screen_manager.o : screen_manager.cpp ex3.h
	        $(CXX) -c screen_manager.cpp
infrastructure.o : infrastructure.cpp ex3.h
	        $(CXX) -c infrastructure.cpp

