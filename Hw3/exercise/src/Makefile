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

TARGET1 = visiting3Dhouse
TARGET2 = views

HOUSE_SRCS = house/Garage.cpp house/House.cpp house/Lantern.cpp house/Porch.cpp \
            house/BuildingBlocks.cpp house/Bathroom.cpp house/Bedroom1.cpp \
            house/Bedroom2.cpp house/DiningRoom.cpp house/FamilyRoom.cpp house/Hallway.cpp

COMMON_SRCS = Object.cpp Camera.cpp Environment.cpp GameManager.cpp WindowManager.cpp \
            InputManager.cpp Celestial.cpp TextureManager.cpp bmp.cpp \
            LightingManager.cpp MaterialManager.cpp Model.cpp Spline.cpp \
            $(HOUSE_SRCS)
			
COMMON_OBJS = $(COMMON_SRCS:.cpp=.o)
OBJS1       = visiting3Dhouse.o $(COMMON_OBJS)
OBJS2       = views.o $(COMMON_OBJS)

USE_MOUSE ?= 1
ifeq ($(USE_MOUSE), 1)
    CFLAGS += -DMOUSE_ROTATION
endif

VSYNC ?= 1
ifeq ($(VSYNC), 1)
    CFLAGS += -DVSYNC
endif

MODEL ?= 0
ifeq ($(MODEL), 1)
    CFLAGS += -DMODEL
endif

.PHONY: all clean

all: $(TARGET1) $(TARGET2)

$(TARGET1): $(OBJS1)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(TARGET2): $(OBJS2)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp %.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $@

visiting3Dhouse.o: visiting3Dhouse.cpp
	$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $@

views.o: views.cpp
	$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $@

clean:
	rm -f $(ALL) $(OBJS1) $(OBJS2) $(COMMON_OBJS) $(TARGET1) $(TARGET2)
 
