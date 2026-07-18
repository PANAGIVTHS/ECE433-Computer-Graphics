CC = gcc

# MacOS settings (For reference)
#ifeq ($(shell sw_vers 2>/dev/null | grep mac | awk '{ print $$2}'),macOS)
#    CFLAGS = -g -Wno-deprecated-declarations
#    INCFLAGS = -I/usr/X11/include
#    LDFLAGS = -framework OpenGL -framework GLUT -framework Cocoa
#else

CFLAGS = -g
INCFLAGS = -I/usr/X11R6/include -I/usr/include
LDFLAGS = -L/usr/X11R6/lib -lglut -lGLU -lGL -lX11
RM = /bin/rm -f

SRC = triangles.cpp white.cpp firstOpenGL.cpp
OBJS = $(SRC:.cpp=.o)
EXES = $(SRC:.cpp=.out)

all: $(EXES)
	@echo "Build complete."
	@$(RM) $(OBJS)

%.out: %.o
	$(CC) $(CFLAGS) -o $@ $^ $(INCFLAGS) $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(EXES)
