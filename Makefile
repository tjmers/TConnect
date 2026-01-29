.PHONY: all local remote local_test remote_test

CXX := g++
CXX_LOCAL_FLAGS := -O3 -Werror -Wall
CXX_REMOTE_FLAGS := -O3 -Werror -Wall
LD_LOCAL_FLAGS :=
LD_REMOTE_FLAGS :=
GRAPHICS_CPP_SRC := 
ENTRY_CPP_SRC :=

LOCAL_DIR := local
LOCAL_SRC_DIR := $(LOCAL_DIR)/src
LOCAL_SRC_FILES := $(wildcard $(LOCAL_SRC_DIR)/*.cpp)

REMOTE_DIR := remote
REMOTE_SRC_DIR := $(REMOTE_DIR)/src
REMOTE_SRC_FILES := $(wildcard $(REMOTE_SRC_DIR)/*.cpp)
LOCAL_GRAPHICS_DIR := $(LOCAL_SRC_DIR)/graphics
LOCAL_ENTRY_DIR := $(LOCAL_SRC_DIR)/entry

BUILD_DIR := build
LOCAL_BUILD_DIR := $(BUILD_DIR)/local
REMOTE_BUILD_DIR := $(BUILD_DIR)/remote
# Local must also include the platform-specific graphics object file
LOCAL_OBJECT_FILES := $(patsubst $(LOCAL_SRC_DIR)/%.cpp, $(LOCAL_BUILD_DIR)/%.o, $(LOCAL_SRC_FILES)) $(LOCAL_BUILD_DIR)/graphics.o $(LOCAL_BUILD_DIR)/entry.o
REMOTE_OBJECT_FILES := $(patsubst $(REMOTE_SRC_DIR)/%.cpp, $(REMOTE_BUILD_DIR)/%.o, $(REMOTE_SRC_FILES))

# Add os specific flags and graphics file
ifeq ($(OS),Windows_NT)
	LD_LOCAL_FLAGS += -ld2d1 -ldwrite
	GRAPHICS_CPP_SRC += $(LOCAL_GRAPHICS_DIR)/win_graphics.cpp
	ENTRY_CPP_SRC += $(LOCAL_ENTRY_DIR)/win_entry.cpp
endif

all: local remote local_test remote_test

local: $(LOCAL_OBJECT_FILES)
	$(CXX) $^ $(LD_LOCAL_FLAGS) -o tconnect

remote: $(REMOTE_OBJECT_FILES)
	$(CXX) $^ $(LD_REMOTE_FLAGS) -o tinit

$(LOCAL_BUILD_DIR)/graphics.o: $(GRAPHICS_CPP_SRC)
	$(CXX) -o $@ $^ -c $(CXX_LOCAL_FLAGS)

$(LOCAL_BUILD_DIR)/entry.o: $(ENTRY_CPP_SRC)
	$(CXX) -o $@ $^ -c $(CXX_LOCAL_FLAGS)

$(REMOTE_BUILD_DIR)/%.o: $(REMOTE_SRC_DIR)/%.cpp
	$(CXX) -o $@ $< -c $(CXX_REMOTE_FLAGS)

$(LOCAL_BUILD_DIR)/%.o: $(LOCAL_SRC_DIR)/%.cpp
	$(CXX) -o $@ $< -c $(CXX_LOCAL_FLAGS)

# Test files

LOCAL_TEST_DIR := $(LOCAL_DIR)/test
LOCAL_TEST_SRC := $(wildcard $(LOCAL_TEST_DIR)/*.cpp)
LOCAL_TEST_EXE := $(patsubst %.cpp, %, $(LOCAL_TEST_SRC))
REMOTE_TEST_DIR := $(REMOTE_DIR)/test
REMOTE_TEST_SRC := $(wildcard $(REMOTE_TEST_DIR)/*.cpp)
REMOTE_TEST_EXE := $(patsubst %.cpp, %, $(REMOTE_TEST_SRC))

local_test: $(LOCAL_TEST_EXE)

remote_test: $(REMOTE_TEST_EXE)

$(LOCAL_TEST_DIR)/%: $(LOCAL_TEST_DIR)/%.cpp $(LOCAL_OBJECT_FILES)
	$(CXX) -o $@ $^ $(CXX_LOCAL_FLAGS) $(LD_LOCAL_FLAGS)

$(REMOTE_TEST_DIR)/%: $(REMOTE_TEST_DIR)/%.cpp $(REMOTE_OBJECT_FILES)
	$(CXX) -o $@ $^ $(CXX_REMOTE_FLAGS) $(LD_REMOTE_FLAGS)