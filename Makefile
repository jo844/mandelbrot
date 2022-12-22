COMPILER=g++
EXECUTABLE_NAME=mandelbrot_zoom

OPENCV_LIBPATH=/usr/local/lib
OPENCV_INCLUDEPATH=/usr/local/include/opencv4

OPENCV_LIBS=-lopencv_core -lopencv_imgcodecs
OTHER_LIBS=-lpthread

GCC_OPTS=-O3 -Wall -Wextra -m64

# Thread pool requires C++17
mandelbrot_zoom: main.o
	$(COMPILER) -o $(EXECUTABLE_NAME) main.o -L $(OPENCV_LIBPATH) $(OPENCV_LIBS) $(OTHER_LIBS)

main.o: main.cpp
	$(COMPILER) -std=c++17 -c main.cpp $(GCC_OPTS) -I $(OPENCV_INCLUDEPATH)

clean:
	rm -f *.o $(EXECUTABLE_NAME)
