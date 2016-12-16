headers = TempBasedSteganography.h

sources = Subroutines.cpp Filer.cpp Sha512.cpp\
    HighLevelCrypto.cpp LowLevelCrypto.cpp\
    HighLevelStega.cpp LowLevelStega.cpp\
      

objects = Subroutines.o Filer.o Sha512.o\
    HighLevelCrypto.o LowLevelCrypto.o\
    HighLevelStega.o LowLevelStega.o\
   
special_objects = main.o Tests.o


CC = g++ -std=c++14 -fpic
LINK_FLAGS = -shared -lboost_python3

lib = python/stega.so

all: tests $(lib)
	

$(lib): $(objects) main.o
	$(CC) -o $(lib) $(objects) main.o $(LINK_FLAGS)

tests: $(objects) Tests.o
	$(CC) -o tests $(objects) Tests.o -lboost_unit_test_framework -lboost_system
	mkdir -p input_test output_test
	./tests
	rm -rf tests input_test output_test

%.o: %.cpp
	$(CC) -c $< -o $@

	
main.o: main.cpp TempBasedSteganography.h
	$(CC) -c -I/usr/include/python3.5 main.cpp
	


clean:
	rm -f $(objects) $(lib) $(special_objects)
