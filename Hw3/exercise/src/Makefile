# --- Global Settings ---
HOST_OS ?= detect
TARGET_OS ?= detect
INCLUDE_LIBS ?= 1
# Default to g++ for C++ projects to avoid linking issues
CC = gcc

# --- OS Detection ---
ifeq ($(OS), Windows_NT)
    HOST_OS = win
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Darwin)
        HOST_OS = mac
    else
        HOST_OS = linux
    endif
endif

ifeq ($(TARGET_OS), detect)
    TARGET_OS = $(HOST_OS)
else ifeq ($(HOST_OS), win)
    TARGET_OS = win
else ifeq ($(HOST_OS), mac)
    TARGET_OS = mac
endif

# --- Platform Specific Commands ---
ifneq ($(filter linux mac,$(HOST_OS)),)
    # LINUX / MAC TOOLS
    TIMESTAMP := $(shell date +%Y%m%d-%H%M%S)
    RM := rm -f
    RM_RF := rm -rf
    CP := cp
    CP_R := cp -r
    MKDIR_P := mkdir -p
    ZIP := zip -r
    SEP := /
    
    # Asset copy logic for Linux/Mac
    COPY_ASSETS = \
        if [ -d "../assets" ]; then $(CP_R) ../assets temp_pkg/; fi; \
        if [ -d "../textures" ]; then $(CP_R) ../textures temp_pkg/; fi
else
    # WINDOWS TOOLS
    TIMESTAMP := $(shell powershell -NoProfile -Command "Get-Date -Format 'yyyyMMdd-HHmmss'")
    RM := del /Q /F
    RM_RF := rmdir /S /Q
    CP := copy /Y
    # xcopy /E (recursive) /I (assume dir) /Y (suppress prompt) /Q (quiet)
    CP_R := xcopy /E /I /Y /Q 
    MKDIR_P := powershell -Command "New-Item -ItemType Directory -Force -Path"
    # Still use Powershell for zipping as Windows has no native command-line zip tool
    ZIP := powershell -Command "Compress-Archive -Force -Path"
    SEP := \\
    
    # Asset copy logic for Windows (using & to chain commands)
    # We use xcopy's ability to handle path creation
    COPY_ASSETS = \
        if exist "..\assets" $(CP_R) "..\assets" "temp_pkg\assets" & \
        if exist "..\textures" $(CP_R) "..\textures" "temp_pkg\textures"
endif

# --- Configuration & Flags ---
TARGET_DIR = ..$(SEP)target$(SEP)$(TARGET_OS)

ifeq ($(TARGET_OS), win)
    # Cross-compilation from Linux/Mac
    ifneq ($(filter linux mac,$(HOST_OS)),)
        CC = x86_64-w64-mingw32-g++
    endif

    EXT = .exe
    LIB_DIR = ..$(SEP)lib$(SEP)$(TARGET_OS)
    
    # Note: Using relative paths with forward slashes usually works for MinGW GCC
    INCFLAGS = -I$(LIB_DIR)/freeglut/include -I$(LIB_DIR)/glew/include
    
    # Ensure strict ordering for Windows libs
    LDFLAGS = -L$(LIB_DIR)/freeglut/lib/x64 -L$(LIB_DIR)/glew/lib/Release/x64 \
              -lfreeglut -lglew32 -lopengl32 -lglu32 \
              -static -static-libgcc -static-libstdc++ \
              -Wl,--subsystem,windows -lstdc++

    LIBS = $(LIB_DIR)$(SEP)freeglut$(SEP)bin$(SEP)x64$(SEP)freeglut.dll $(LIB_DIR)$(SEP)glew$(SEP)bin$(SEP)Release$(SEP)x64$(SEP)glew32.dll
else
    EXT = 
    ifeq ($(TARGET_OS), mac)
        CFLAGS = -g -Wno-deprecated-declarations
        INCFLAGS = -I/usr/X11/include
        LDFLAGS = -framework OpenGL -framework GLUT -framework Cocoa
    else
        # Linux
        LIB_DIR = ..$(SEP)lib$(SEP)linux

        # Libs for local opengl library
#         INCFLAGS = -I/usr/X11R6/include -I/sw/include \
#                    -I/usr/sww/include -I/usr/sww/pkg/Mesa/include

#         LDFLAGS = -L/usr/X11R6/lib -L/sw/lib -L/usr/sww/lib \
#                   -L/usr/sww/bin -L/usr/sww/pkg/Mesa/lib \
#                   -lGLEW -lglut -lGLU -lGL -lX11 -lm -lstdc++
        INCFLAGS = -I$(LIB_DIR)/glew/include \
                   -I$(LIB_DIR)/freeglut/include
        LDFLAGS = -L$(LIB_DIR)/glew/lib \
                        -L$(LIB_DIR)/freeglut/lib \
                        -Wl,-rpath,'$$ORIGIN' \
                        -lGLEW -lglut -lGLU -lGL -lX11 -lm -lstdc++
                  
        LIBS = $(LIB_DIR)/freeglut/lib/libglut.so* \
                $(LIB_DIR)/glew/lib/libGLEW.so*
    endif
endif

TARGET1 = $(TARGET_DIR)/visiting3Dhouse$(EXT)
TARGET2 = $(TARGET_DIR)/views$(EXT)
ZIP_NAME = $(TARGET_OS)-build-$(TIMESTAMP).zip

# --- Source Files ---
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

# --- Conditional Flags ---
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

.PHONY: all clean package

all: $(TARGET1) $(TARGET2)

$(TARGET1): $(OBJS1)
	@$(MKDIR_P) $(TARGET_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(TARGET2): $(OBJS2)
	@$(MKDIR_P) $(TARGET_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp %.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $@

# Explicit Rules
visiting3Dhouse.o: visiting3Dhouse.cpp
	$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $@

views.o: views.cpp
	$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $@

# --- Packaging ---
package: all
	@echo Packaging for $(TARGET_OS)...
ifeq ($(HOST_OS),win)
	-$(RM_RF) temp_pkg
	$(MKDIR_P) temp_pkg
	$(CP) "$(TARGET_DIR)$(SEP)*.exe" temp_pkg$(SEP)
	$(COPY_ASSETS)
ifeq ($(INCLUDE_LIBS), 1)
	$(foreach file,$(LIBS),$(CP) $(file) temp_pkg$(SEP);)
endif
	$(ZIP) "temp_pkg\*" "$(TARGET_DIR)\$(ZIP_NAME)"
	$(RM_RF) temp_pkg
else
	$(RM_RF) temp_pkg
	$(MKDIR_P) temp_pkg
	$(CP) $(TARGET1) temp_pkg/
	$(CP) $(TARGET2) temp_pkg/
	$(COPY_ASSETS)
ifeq ($(INCLUDE_LIBS), 1)
	$(foreach file,$(LIBS),$(CP) $(file) temp_pkg$(SEP);)
endif
	cd temp_pkg && $(ZIP) ../$(TARGET_DIR)/$(ZIP_NAME) .
	$(RM_RF) temp_pkg
endif
	@echo Created package: $(TARGET_DIR)/$(ZIP_NAME)

clean:
ifeq ($(HOST_OS),win)
	-$(RM) *.o house\*.o
	-$(RM) "$(subst /,\,$(TARGET1))" "$(subst /,\,$(TARGET2))"
    -$(RM_RF) $(TARGET_DIR)
else
	$(RM_RF) $(OBJS1) $(OBJS2) $(TARGET1) $(TARGET2) $(TARGET_DIR)
endif