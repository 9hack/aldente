MAIN_TARGET     := aldente

SRC_DIR         := src
INC_DIR         := src
DEP_DIR         := .dep
BUILD_DIR       := .build
PROTO_DIR       := $(SRC_DIR)/proto

CC              := clang++
CFLAGS          := -std=c++14 -O0 -g -pthread
INCS            := -I$(INC_DIR)
HEADERS         := $(shell find $(INC_DIR) -name '*.h' -type 'f' | sort)
MAIN_SOURCES    := $(shell find $(SRC_DIR) -name '*.cpp' -type 'f' | sort)
MAIN_OBJECTS    := $(MAIN_SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEPFILES        := $(MAIN_SOURCES:$(SRC_DIR)/%.cpp=$(DEP_DIR)/%.dep)
NODEPS          := clean proto

LIBS            := -lGLEW -lglfw -lassimp -lSOIL -lfreetype
LIBS            += -lboost_system -lboost_filesystem -lboost_thread
LIBS            += -lBulletSoftBody -lBulletDynamics -lBulletCollision -lLinearMath
LIBS            += -lprotobuf
LIBS            += -lsfml-system -lsfml-audio
INCS            += $(shell pkg-config bullet --cflags)

ifeq ($(shell uname),Darwin)
	LIBS += -framework OpenGL
	INCS += -I/usr/local/include/freetype2
	INCS += -I/usr/local/include/SOIL
else
	LIBS += -lGL -lGLU
endif

all: $(DEPFILES) $(MAIN_TARGET)

proto:
	$(MAKE) -C $(PROTO_DIR)

# Linking all objects with libs into executable.
$(MAIN_TARGET): $(MAIN_OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

# Compilation of source files into object files, with help of depedency files.
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEP_DIR)/%.dep
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

# Rule for creating dependency files.
$(DEP_DIR)/%.dep: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCS) -MM -MT '$(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$<)' $< -MF $@

clean:
	rm -rf $(BUILD_DIR) $(DEP_DIR) $(MAIN_TARGET)

ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
	-include $(DEPFILES)
endif
