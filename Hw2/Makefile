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
		-L/usr/sww/bin -L/usr/sww/pkg/Mesa/lib -lGLEW -lglut -lGLU -lGL -lX11
endif
RM = /bin/rm -f
all: my_triangles my_white my_FirstOpenGL
my_triangles: my_triangles.o 
	$(CC) $(CFLAGS) -o my_triangles my_triangles.o $(INCFLAGS) $(LDFLAGS)
my_triangles.o: my_triangles.cpp 
	$(CC) $(CFLAGS) $(INCFLAGS) -c my_triangles.cpp 
my_white: my_white.o 
	$(CC) $(CFLAGS) -o my_white my_white.o $(INCFLAGS) $(LDFLAGS)
my_white.o: my_white.cpp 
	$(CC) $(CFLAGS) $(INCFLAGS) -c my_white.cpp 
my_FirstOpenGL: my_FirstOpenGL.o 
	$(CC) $(CFLAGS) -o my_FirstOpenGL my_FirstOpenGL.o $(INCFLAGS) $(LDFLAGS)
my_FirstOpenGL.o: my_FirstOpenGL.cpp 
	$(CC) $(CFLAGS) $(INCFLAGS) -c my_FirstOpenGL.cpp 
clean: 
	$(RM) *.o my_triangles my_white my_FirstOpenGL
 
