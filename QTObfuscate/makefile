CC=g++
CFLAGS=-c -Wall
LDFLAGS=  
INCLUDES  := -I/home/koh/QtSDK/Desktop/Qt/474/gcc/include -I/usr/share/qt4/include
VPATH=/image /main   
SOURCES= FileManager.cpp File.cpp Algorithm1.cpp main.cpp Engine.cpp mainwindow.cpp Plugin1.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=test

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC)  $(INCLUDES) $(CFLAGS) $< -o $@

clean:
	rm $(OBJECTS)



