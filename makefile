headers = sha512.h TempBasedSteganography.h

sources = VectorSubroutines.cpp Filer.cpp Sha512.cpp\
    HighLevelCrypto.cpp LowLevelCrypto.cpp\
    HighLevelStega.cpp LowLevelStega.cpp\
      

objects = VectorSubroutines.o Filer.o Sha512.o\
    HighLevelCrypto.o LowLevelCrypto.o\
    HighLevelStega.o LowLevelStega.o\
   
special_objects = main.o Tests.o

CC = g++ -std=c++14 -fpic -g
LINK_FLAGS = -shared -lboost_python 
output = stega.so
exec = stega

all: tests $(exec)
	

$(exec): $(objects) main.o
	$(CC) -o $(exec) $(objects) main.o

tests: $(objects) Tests.o
	$(CC) -o tests $(objects) Tests.o -lboost_unit_test_framework
	./tests --log_level=test_suite

Sha512.o: Sha512.cpp TempBasedSteganography.h
	$(CC) -c Sha512.cpp

VectorSubroutines.o: VectorSubroutines.cpp TempBasedSteganography.h
	$(CC) -c VectorSubroutines.cpp

Filer.o: Filer.cpp TempBasedSteganography.h
	$(CC) -c Filer.cpp
	
HighLevelStega.o: HighLevelStega.cpp TempBasedSteganography.h
	$(CC) -c HighLevelStega.cpp
	
HighLevelCrypto.o: HighLevelCrypto.cpp TempBasedSteganography.h
	$(CC) -c HighLevelCrypto.cpp
	
LowLevelStega.o: LowLevelStega.cpp TempBasedSteganography.h
	$(CC) -c LowLevelStega.cpp

LowLevelCrypto.o: LowLevelCrypto.cpp TempBasedSteganography.h
	$(CC) -c LowLevelCrypto.cpp

	
main.o: main.cpp TempBasedSteganography.h
	$(CC) -c -I/usr/include/python3.4 main.cpp
	
Tests.o: Tests.cpp TempBasedSteganography.h
	$(CC) -c Tests.cpp
	

clean:
	rm -f $(objects) $(output) $(special_objects)
