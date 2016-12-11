headers = TempBasedSteganography.h

sources = VectorSubroutines.cpp Filer.cpp Sha512.cpp\
    HighLevelCrypto.cpp LowLevelCrypto.cpp\
    HighLevelStega.cpp LowLevelStega.cpp\
      

objects = VectorSubroutines.o Filer.o Sha512.o\
    HighLevelCrypto.o LowLevelCrypto.o\
    HighLevelStega.o LowLevelStega.o\
   
special_objects = main.o Tests.o


CC = g++ -std=c++14 -fpic -g
LINK_FLAGS = -shared -lboost_python 

lib = python/stega.so

all: tests $(lib)
	

$(lib): $(objects) main.o
	$(CC) $(LINK_FLAGS) -o $(lib) $(objects) main.o

tests: $(objects) Tests.o
	$(CC) -o tests $(objects) Tests.o -lboost_unit_test_framework
	./tests
	rm -f tests

%.o: %.cpp
	$(CC) -c $< -o $@

	
main.o: main.cpp TempBasedSteganography.h
	$(CC) -c -I/usr/include/python3.4 main.cpp
	


clean:
	rm -f $(objects) $(lib) $(special_objects)
