.PHONY: all test clean

PROGRAM      := ./bin/gel
SOURCES      := $(wildcard src/*.cc)
DEPENDS      := $(addprefix build/, $(patsubst %.cc, %.d, $(SOURCES)))
OBJS         := $(addprefix build/, $(patsubst %.cc, %.o, $(SOURCES)))

TEST_PROGRAM := ./bin/test
TEST_SOURCES := $(wildcard test/*.cc)
TEST_DEPENDS := $(addprefix build/, $(patsubst %.cc, %.d, $(TEST_SOURCES)))
TEST_OBJS    := $(addprefix build/, $(patsubst %.cc, %.o, $(TEST_SOURCES)))

INCLUDE_DIR  := ./include ./src
LIB_DIR      := ./lib
LIBS         :=

CC           := g++
CCFLAGS      := -Wall $(addprefix -I ,$(INCLUDE_DIR)) $(addprefix -isystem ,$(INCLUDE_DIR)) $(addprefix -L ,$(LIB_DIR)) $(addprefix -l, $(LIBS)) -std=c++1y

all: $(DEPENDS) $(PROGRAM)

test: $(DEPENDS) $(TEST_DEPENDS) $(TEST_PROGRAM)
	@$(TEST_PROGRAM)

$(PROGRAM): $(OBJS)
	@mkdir -p `dirname $@`
	$(CC) $(CCFLAGS) -o $@ $^

build/%.d: %.cc
	@mkdir -p `dirname $@`
	$(CC) $(CCFLAGS) -MM $< > $@

build/%.o: %.cc
	@mkdir -p `dirname $@`
	$(CC) $(CCFLAGS) -o $@ -c $<

$(TEST_PROGRAM): $(filter-out build/src/main.o, $(OBJS)) $(TEST_OBJS)
	@mkdir -p `dirname $@`
	$(CC) $(CCFLAGS) -o $@ $^ 

ifneq "$(MAKECMDGOALS)" "clean"
-include $(DEPENDS)
endif

ifeq "$(MAKECMDGOALS)" "test"
-include $(TEST_DEPENDS)
endif

clean:
	rm -rf build bin
