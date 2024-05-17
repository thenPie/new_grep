#ifndef GREP_FUNC_H
#define GREP_FUNC_H

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
void print_existing_file_names(char** file_list, int count_files);
void opter(struct Flags* flags, int argc, char** argv);
void print_all_argvs(int argc, char** argv);
void print_flags(struct Flags flags);

#endif