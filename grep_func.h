#ifndef GREP_FUNC_H
#define GREP_FUNC_H

#define MAX_ARRAY_SIZE 1000
#define MAX_SINGLE_ARRAY_ENTRY_SYMBOLS 1000

struct Flags {
    int e;
    int i;
    int v;
    int c;
    int l;
    int n;
};

void run_grep(int argc, char** argv);

int check_if_there_are_any_arguments(int argc);
char** list_of_existing_files(int argc, char** argv, int* count_files);
char** list_of_patterns(int argc, char** argv, int* count_patterns, char** file_list, int count_files);
int is_in_exclude_list(char* string, char exclude_list[MAX_SINGLE_ARRAY_ENTRY_SYMBOLS][MAX_ARRAY_SIZE], int exclude_list_index);
void grep_func(char** existing_files, char** patterns, int count_files, int count_patterns);
void print_existing_file_names(char** file_list, int count_files);
void print_patterns(char** pattern_list, int count_patterns);
void opter(struct Flags* flags, int argc, char** argv);
void print_all_argvs(int argc, char** argv);
void print_flags(struct Flags flags);

void free_list(char** list, int count);

#endif