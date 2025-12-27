CC = gcc
ifeq ($(shell sw_vers 2>/dev/null | grep mac | awk '{ print $$2}'),macOS)
CFLAGS = -g -Wno-deprecated-declarations
INCFLAGS = -I/usr/X11/include
LDFLAGS = -framework OpenGL -framework GLUT -framework Cocoa 
#CFLAGS = -g -DGL_GLEXT_PROTOTYPES -DGL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED -DOSX -Wno-deprecated-register -Wno-deprecated-declarations -Wno-shift-op-parentheses -Wno-parentheses-equality
#INCFLAGS = -I/usr/X11/include
#LDFLAGS = -framework GLUT -framework OpenGL \
#		-L"/System/Library/Frameworks/OpenGL.framework/Libraries" \
#		-lGL -lGLU -lm -lstdc++ 
else
CFLAGS = -g #-DGL_GLEXT_PROTOTYPES 
INCFLAGS = -I/usr/X11R6/include -I/sw/include \
		-I/usr/sww/include -I/usr/sww/pkg/Mesa/include
LDFLAGS = -L/usr/X11R6/lib -L/sw/lib -L/usr/sww/lib \
		-L/usr/sww/bin -L/usr/sww/pkg/Mesa/lib -lGLEW -lglut -lGLU -lGL -lX11 -lm -lstdc++
endif
RM = /bin/rm -f
all: views3Dhouse
views3Dhouse: views3Dhouse.o Object.o Camera.o
	$(CC) $(CFLAGS) -o views3Dhouse views3Dhouse.o Object.o Camera.o $(INCFLAGS) $(LDFLAGS)
views3Dhouse.o: views3Dhouse.cpp 
	$(CC) $(CFLAGS) $(INCFLAGS) -c views3Dhouse.cpp
Object.o: Object.cpp Object.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c Object.cpp
Camera.o: Camera.cpp Camera.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c Camera.cpp


#all: Sphere_red
#lab2_example1: lab2_example1.o 
#	$(CC) $(CFLAGS) -o lab2_example1 lab2_example1.o $(INCFLAGS) $(LDFLAGS)
#lab2_example1.o: lab2_example1.cpp 
#	$(CC) $(CFLAGS) $(INCFLAGS) -c lab2_example1.cpp 
clean: 
	$(RM) *.o views views3Dhouse
 
