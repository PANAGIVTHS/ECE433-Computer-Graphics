HOST_OS ?= detect
TARGET_OS ?= detect
CC = gcc

TIMESTAMP := $(shell date +%Y%m%d-%H%M%S)

# Detect macOS
ifeq ($(shell sw_vers 2>/dev/null | grep mac | awk '{ print $$2}'),macOS)
    HOST_OS = mac
    RM = rm
# Detect Windows
else ifeq ($(OS), Windows_NT)
    HOST_OS = win
    RM = del /Q
# Detect Linux
else 
    HOST_OS = linux
    RM = rm
endif

ifeq ($(TARGET_OS), detect)
    TARGET_OS = $(HOST_OS)
endif

# --- Windows Configuration (Cross-Compile) ---
ifeq ($(TARGET_OS), win)
    ifneq ($(filter linux mac,$(HOST_OS)),)
        CC = x86_64-w64-mingw32-g++
    endif
    EXT = .exe
    
    LIB_DIR = ../lib/$(TARGET_OS)/

    INCFLAGS = -I$(LIB_DIR)/freeglut/include -I$(LIB_DIR)/glew/include
    
    LIB_FREEGLUT = $(LIB_DIR)/freeglut/lib/x64
    LIB_GLEW     = $(LIB_DIR)/glew/lib/Release/x64
    
    CFLAGS = -static -static-libgcc -static-libstdc++
    
    LDFLAGS = -L$(LIB_FREEGLUT) -L$(LIB_GLEW) \
              -lfreeglut -lglew32 -lopengl32 -lglu32 \
              -Wl,--subsystem,windows

    DLL_FILES = $(LIB_DIR)/freeglut/bin/x64/freeglut.dll $(LIB_DIR)/glew/bin/Release/x64/glew32.dll
else
    EXT = 
    # Detect macOS
    ifeq ($(shell sw_vers 2>/dev/null | grep mac | awk '{ print $$2}'),macOS)
        CFLAGS = -g -Wno-deprecated-declarations
        INCFLAGS = -I/usr/X11/include
        LDFLAGS = -framework OpenGL -framework GLUT -framework Cocoa
    
    # Detect Linux
    else
        CFLAGS = -g
        INCFLAGS = -I/usr/X11R6/include -I/sw/include \
                   -I/usr/sww/include -I/usr/sww/pkg/Mesa/include
        LDFLAGS = -L/usr/X11R6/lib -L/sw/lib -L/usr/sww/lib \
                  -L/usr/sww/bin -L/usr/sww/pkg/Mesa/lib \
                  -lGLEW -lglut -lGLU -lGL -lX11 -lm -lstdc++
    endif
endif

# Add extension to targets
TARGET_DIR = ../target/$(TARGET_OS)
TARGET1 = $(TARGET_DIR)/visiting3Dhouse$(EXT)
TARGET2 = $(TARGET_DIR)/views$(EXT)
ZIP_NAME = $(TARGET_OS)-build-$(TIMESTAMP).zip

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
	@mkdir -p $(TARGET_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(TARGET2): $(OBJS2)
	@mkdir -p $(TARGET_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp %.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $@

visiting3Dhouse.o: visiting3Dhouse.cpp
	$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $@

views.o: views.cpp
	$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $@

package: all
	@echo "Packaging for $(TARGET_OS)..."
	@mkdir -p temp_pkg
	@# Copy Executables
	cp $(TARGET1) temp_pkg/
	cp $(TARGET2) temp_pkg/
	@# Copy Assets (Textures/Models)
	@if [ -d "../assets" ]; then cp -r ../assets temp_pkg/; fi
	@if [ -d "../textures" ]; then cp -r ../textures temp_pkg/; fi
	@# If Windows, Copy DLLs
ifeq ($(TARGET_OS), win)
	@echo "Copying DLLs..."
	cp $(DLL_FILES) temp_pkg/
endif
	@# Zip it up
	cd temp_pkg && zip -r ../$(TARGET_DIR)/$(ZIP_NAME) .
	@# Clean up temp folder
	rm -rf temp_pkg
	@echo "Created package: $(TARGET_DIR)/$(ZIP_NAME)"

clean:
	rm -f $(ALL) $(OBJS1) $(OBJS2) $(COMMON_OBJS) $(TARGET1) $(TARGET2)
	rm -rf $(TARGET_DIR)
 
