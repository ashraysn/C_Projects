
# design names
DESIGN = PhotoLab PhotoLabTest

#libs
LIBS   = libfilter.a libFileIO.a

CC     = gcc
DEBUG  = -DDEBUG # used for PhotoLabTest only; for normal program, need to add manually
CFLAGS = -std=c11 -Wall -c
LFLAGS = -std=c11 -Wall
AR     = ar rc
RANLIB = ranlib

IMAGES = negative.ppm \
         bw.ppm \
         colorfilter.ppm \
         edge.ppm \
		 shuffle.ppm \
		 hlip.ppm \
		 hmirror.ppm \
		 border.ppm \
		 normalize.ppm \
		 posterize.ppm \
		 rotate.ppm \
		 motionblur.ppm \
		 crop.ppm \
		 brightnessandcontrast.ppm
		 
all: PhotoLab PhotoLabTest


########### generate object files ###########

#target to generate FileIO.o
FileIO.o: FileIO.h FileIO.c Constants.h  
	$(CC) $(CFLAGS) FileIO.c -o FileIO.o

#target to generate DIPs.o
DIPs.o: Image.h DIPs.h DIPs.c Constants.h
	$(CC) $(CFLAGS) DIPs.c -o DIPs.o

#target to generate Advanced.o
Advanced.o: Image.h Advanced.h Advanced.c Constants.h
	$(CC) $(CFLAGS) Advanced.c -o Advanced.o

#target to generate Image.o
Image.o: Image.c Image.h Constants.h 
	$(CC) $(CFLAGS) Image.c -o Image.o

#target to generate Test.o 
Test.o: Image.h Test.h Test.c Constants.h
	$(CC) $(CFLAGS) Test.c -o Test.o 

#target to generate Test_DEBUG.o, debug version of Test.o 
Test_DEBUG.o: Image.h Test.h Test.c Constants.h 
	$(CC) $(CFLAGS) $(DEBUG) Test.c -o Test_DEBUG.o 

#target to generate PhotoLab.o
#debugging is optional for regular PhotoLab (must hardcode in)
PhotoLab.o: PhotoLab.c Image.h DIPs.h Advanced.h FileIO.h Constants.h
	$(CC) $(CFLAGS) PhotoLab.c -o PhotoLab.o

#target to generate FileIO_DEBUG.o

# hardcoding debug def because DEBUG version should always debug
PhotoLab_DEBUG.o: PhotoLab.c Test.h Image.h DIPs.h Advanced.h FileIO.h Constants.h
	$(CC) $(CFLAGS) $(DEBUG) PhotoLab.c -o PhotoLab_DEBUG.o

########### generate library files ###########

#target to generate libFilter.a
libfilter.a: DIPs.o Advanced.o
	$(AR) libfilter.a Advanced.o DIPs.o
	$(RANLIB) libfilter.a


########### generate executables ###########

#target to generate PhotoLab
PhotoLab: PhotoLab.o FileIO.o Image.o Test.o libfilter.a
	$(CC) $(LFLAGS) -g PhotoLab.o FileIO.o Image.o Test.o -L. -lfilter -o PhotoLab -lm

#target to generate test
PhotoLabTest: PhotoLab_DEBUG.o Test_DEBUG.o FileIO.o Image.o libfilter.a
	$(CC) $(LFLAGS) $(DEBUG) PhotoLab_DEBUG.o Test_DEBUG.o FileIO.o Image.o -L. -lfilter -o PhotoLabTest -lm

#target to clean the directory
clean:
	rm -f *.o *.jpg $(DESIGN) $(IMAGES) $(LIBS)
	rm -f PhotoLab
	rm -f PhotoLabTest
