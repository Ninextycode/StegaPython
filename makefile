headers = sha512.h TempBasedStegonography.h
sources = Filer.cpp High_Level.cpp Low_Level.cpp sha512.cpp main.cpp
objects = Filer.o High_Level.o Low_Level.o sha512.o main.o
CC = g++ -std=c++11 -fpic
output = stega.so

$(output): $(objects)
	$(CC)  -shared -lboost_python -o $(output) $(objects) 

Filer.o: Filer.cpp TempBasedStegonography.h
	$(CC) -c Filer.cpp
	
High_Level.o: High_Level.cpp TempBasedStegonography.h
	$(CC) -c High_Level.cpp
	
Low_Level.o: Low_Level.cpp TempBasedStegonography.h
	$(CC) -c Low_Level.cpp
	
sha512.o: sha512.cpp sha512.h
	$(CC) -c sha512.cpp

main.o: main.cpp TempBasedStegonography.h
	$(CC) -c -I/usr/include/python3.4 main.cpp
	
clean:
	rm -f $(objects) $(output) 
