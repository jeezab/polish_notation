CC      = gcc
CFLAGS  = -Wall -Werror -Wextra -std=c11
# CFLAGS += -fsanitize=leak -fsanitize=address -fsanitize=unreachable -fsanitize=undefined
LFLAGS  = -lm

CC_WIN  = x86_64-w64-mingw32-gcc
CFLAGS_WIN = $(CFLAGS)
LFLAGS_WIN = -lm

SRC     = main.c \
          input/input_expr.c \
          stack/stack.c \
          whotype/whotype.c \
          dejkstra_alg/dejkstra.c \
          dejkstra_alg/expr_logic_check.c \
          polish_calc/polish_calc.c \
          graph/plot_graph.c

HDR     = input/input_expr.h \
          stack/stack.h \
          whotype/whotype.h \
          dejkstra_alg/dejkstra.h \
          dejkstra_alg/expr_logic_check.h \
          polish_calc/polish_calc.h \
          graph/plot_graph.h

OBJ_DIR = ./objs
BUILD   = ../build

OBJ     = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC))
OBJ_WIN = $(patsubst %.c,$(OBJ_DIR)/windows/%.o,$(SRC))

VPATH = ./input ./stack ./whotype ./dejkstra_alg ./polish_calc ./graph

.PHONY: all win_build clean rebuild cpplang clang_fix vlgr run

.SILENT:

all: polish_notation

polish_notation: $(BUILD)/polish_notation.out
	@echo "polish_notation.out: build complete"

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/polish_notation.out: $(OBJ)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ $(LFLAGS) -o $@

win_build: $(BUILD)/polish_notation.exe
	@echo "polish_notation.exe: build complete"

$(OBJ_DIR)/windows/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC_WIN) $(CFLAGS_WIN) -c $< -o $@

$(BUILD)/polish_notation.exe: $(OBJ_WIN)
	@mkdir -p $(dir $@)
	$(CC_WIN) $(CFLAGS_WIN) $^ $(LFLAGS_WIN) -o $@

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(BUILD)/polish_notation.* $(BUILD)/polish_notation.exe

rebuild: clean all win_build

cpplang:
	clang-format -n $(SRC) $(HDR)
	cppcheck --enable=all --suppress=missingIncludeSystem --checkers-report=checkers.txt $(SRC)

clang_fix:
	clang-format -i $(SRC) $(HDR)

vlgr: $(BUILD)/polish_notation.out
	valgrind --tool=memcheck --leak-check=yes $(BUILD)/polish_notation.out

run: $(BUILD)/polish_notation.out
	$(BUILD)/polish_notation.out
