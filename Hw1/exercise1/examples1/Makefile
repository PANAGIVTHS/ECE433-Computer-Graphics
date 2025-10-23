CC = gcc

ifeq ($(shell sw_vers 2>/dev/null | grep mac | awk '{ print $$2}'),macOS)
CFLAGS = -g -Wno-deprecated-declarations
INCFLAGS = -I/usr/X11/include
LDFLAGS = -framework OpenGL -framework GLUT -framework Cocoa
else
CFLAGS = -g
INCFLAGS = -I/usr/X11R6/include -I/sw/include \
            -I/usr/sww/include -I/usr/sww/pkg/Mesa/include
LDFLAGS = -L/usr/X11R6/lib -L/sw/lib -L/usr/sww/lib \
           -L/usr/sww/bin -L/usr/sww/pkg/Mesa/lib \
           -lglut -lGLU -lGL -lX11
endif

RM = /bin/rm -f
OUT_DIR = .out_files
OBJS = $(OUT_DIR)/my_triangles.o $(OUT_DIR)/my_white.o $(OUT_DIR)/my_FirstOpenGL.o

all: my_triangles my_white my_FirstOpenGL

# Create output folder automatically if missing
$(OUT_DIR):
	mkdir -p $(OUT_DIR)

# Targets
my_triangles: $(OUT_DIR)/my_triangles.o | $(OUT_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(INCFLAGS) $(LDFLAGS)

$(OUT_DIR)/my_triangles.o: my_triangles.cpp | $(OUT_DIR)
	$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $@

my_white: $(OUT_DIR)/my_white.o | $(OUT_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(INCFLAGS) $(LDFLAGS)

$(OUT_DIR)/my_white.o: my_white.cpp | $(OUT_DIR)
	$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $@

my_FirstOpenGL: $(OUT_DIR)/my_FirstOpenGL.o | $(OUT_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(INCFLAGS) $(LDFLAGS)

$(OUT_DIR)/my_FirstOpenGL.o: my_FirstOpenGL.cpp | $(OUT_DIR)
	$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) my_triangles my_white my_FirstOpenGL
	rmdir $(OUT_DIR) 2>/dev/null || true
