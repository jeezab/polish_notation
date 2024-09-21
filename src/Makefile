CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
#-fsanitize=leak -fsanitize=address -fsanitize=unreachable -fsanitize=undefined

SRC = ./main.c ./input/input_expr.c ./stack/stack.c ./whotype/whotype.c \
      ./dejkstra_alg/dejkstra.c ./dejkstra_alg/exprLogicCheck.c
HDR = ./input/input_expr.h ./stack/stack.h ./whotype/whotype.h \
      ./dejkstra_alg/dejkstra.h ./dejkstra_alg/exprLogicCheck.h
OBJ_DIR = ./objs

OBJ = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC))

BUILD = ../build

.PHONY: all clean rebuild cpplang clang_fix vlgr run

.SILENT:
all: polish_notation

polish_notation: $(BUILD)/polish_notation.out
	@echo "polish_notation: build complete"

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: ./%.c
	@mkdir -p $(OBJ_DIR)/$(dir $<)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/polish_notation.out: $(OBJ)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(BUILD)/polish_notation.out

rebuild: clean all

cpplang:
	clang-format -n $(SRC) $(HDR)
	cppcheck --enable=all --suppress=missingIncludeSystem --checkers-report=checkers.txt $(SRC)

clang_fix:
	clang-format -i $(SRC) $(HDR)

vlgr: $(BUILD)/polish_notation.out
	valgrind --tool=memcheck --leak-check=yes $(BUILD)/polish_notation.out

run: $(BUILD)/polish_notation.out
	$(BUILD)/polish_notation.out
