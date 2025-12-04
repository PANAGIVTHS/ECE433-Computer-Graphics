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
CFLAGS = -Wall -g #-DGL_GLEXT_PROTOTYPES 
INCFLAGS = -I/usr/X11R6/include -I/sw/include \
		-I/usr/sww/include -I/usr/sww/pkg/Mesa/include
LDFLAGS = -L/usr/X11R6/lib -L/sw/lib -L/usr/sww/lib \
		-L/usr/sww/bin -L/usr/sww/pkg/Mesa/lib -lGLEW -lglut -lGLU -lGL -lX11 -lm -lstdc++
endif
RM = /bin/rm -f
all: exercise12
exercise12: exercise12.o polygon.o edge.o clippingWindow.o
	$(CC) $(CFLAGS) -o exercise12 exercise12.o polygon.o edge.o clippingWindow.o $(INCFLAGS) $(LDFLAGS)
exercise12.o: exercise12.cpp 
	$(CC) $(CFLAGS) $(INCFLAGS) -c exercise12.cpp
polygon.o: polygon.cpp polygon.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c polygon.cpp
edge.o: edge.cpp edge.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c edge.cpp
clippingWindow.o: clippingWindow.cpp clippingWindow.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c clippingWindow.cpp
clean: 
	$(RM) *.o exercise12
 
