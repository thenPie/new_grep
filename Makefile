CC = gcc
CC_FLAGS_STD = $(CC) -Wall -Werror -Wextra -std=c11
NAME_IS = -o
GREP_SRC_CODE = grep.c grep_func.c

all: s21_grep

s21_grep: $(GREP_SRC_CODE)
	$(CC_FLAGS_STD) $(NAME_IS) s21_grep $(GREP_SRC_CODE)

clean:
	rm s21_grep

rebuild: clean all