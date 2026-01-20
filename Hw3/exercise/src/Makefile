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

TARGET = visiting3Dhouse
SRCS   = visiting3Dhouse.cpp Object.cpp Camera.cpp Environment.cpp GameManager.cpp WindowManager.cpp InputManager.cpp Celestial.cpp TextureManager.cpp bmp.cpp LightingManager.cpp MaterialManager.cpp Model.cpp Spline.cpp
HOUSE_SRCS = house/Garage.cpp house/House.cpp house/Lantern.cpp
SRCS += $(HOUSE_SRCS)
OBJS   = $(SRCS:.cpp=.o)

USE_MOUSE ?= 1
ifeq ($(USE_MOUSE), 1)
    CFLAGS += -DMOUSE_ROTATION
endif

VSYNC ?= 0
ifeq ($(VSYNC), 1)
    CFLAGS += -DVSYNC
endif

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp %.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $@

visiting3Dhouse.o: visiting3Dhouse.cpp
	$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
 
