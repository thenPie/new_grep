#include "grep_func.h"

#include <stdio.h>
#include <unistd.h>

#include <stdlib.h>
#include <string.h>
// #include <cstring>

// Тебе необходимо разработать утилиту grep:
// - Поддержка следующих флагов: `-e`, `-i`, `-v`, `-c`, `-l`, `-n`
// - Для регулярных выражений можно использовать только библиотеки pcre или regex  
// - Исходные, заголовочные и make файлы должны располагаться в директории src/grep/
// - Итоговый исполняемый файл должен располагаться в директории src/grep/ и называться s21_grep

// ### Опции grep 

// | № | Опции | Описание |
// | ------ | ------ | ------ |
// | 1 | -e | Шаблон. |
// | 2 | -i | Игнорирует различия регистра.  |
// | 3 | -v | Инвертирует смысл поиска соответствий. |
// | 4 | -c | Выводит только количество совпадающих строк. |
// | 5 | -l | Выводит только совпадающие файлы.  |
// | 6 | -n | Предваряет каждую строку вывода номером строки из файла ввода. |

// ### Использование grep 

// `grep [options] template [file_name]`

void run_grep(int argc, char** argv) {

    // debug
    printf("Function: run_grep\n");

    struct Flags flags = {-1, -1, -1, -1, -1, -1};
    int count_files;
    int count_patterns;
    char** existing_files = list_of_existing_files(argc, argv, &count_files);
    char** patterns = list_of_patterns(argc, argv, &count_patterns, existing_files, count_files);

    // debug
    printf("Count patterns: %d\n", count_patterns);

    int valid = 0;
    
    valid = check_if_there_are_any_arguments(argc);

    if (valid == 0) {
        opter(&flags, argc, argv);

        // delete later
        // print_existing_file_names(existing_files, count_files);
        // print_patterns(patterns, count_patterns);
        print_existing_file_names(patterns, count_patterns);
    }

    free_list(patterns, count_patterns);
    free_list(existing_files, count_files);

    // delete this line later
    if (valid == -1) {
        printf("Program didn't start");
    } else {
        printf("Program worked");
    }
}

void free_list(char** list, int count) {
    if (list != NULL) {
        for (int i = 0; i < count; i++) {
            if (list[i] != NULL) {
                free(list[i]);
            }
        }
        free(list);
    }
}

// TODO: make list_functions into different functions, code duplication!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

char** list_of_patterns(int argc, char** argv, int* count_patterns, char** file_list, int count_files) {

    // debug
    printf("Function: list_of_patterns\n");

    char** pattern_list = (char**)malloc((argc - 1) * sizeof(char*));
    if (pattern_list == NULL) {
        perror("Cannot see patterns");
        exit(EXIT_FAILURE);
    } else {
        *count_patterns = 0;
        char exclude_list[MAX_ARRAY_SIZE][MAX_SINGLE_ARRAY_ENTRY_SYMBOLS] = {
            "-e",
            "-i",
            "-v",
            "-c",
            "-l",
            "-n"
        };
        int exclude_list_index = 6;

        for (int i = 0; i < count_files; i++) {
            strncpy(exclude_list[exclude_list_index], file_list[i], MAX_SINGLE_ARRAY_ENTRY_SYMBOLS - 1);
            exclude_list[exclude_list_index][MAX_SINGLE_ARRAY_ENTRY_SYMBOLS - 1] = '\0';
            exclude_list_index += 1;
        }

        // debug, shows exclusion list
        // for (int i = 0; i < exclude_list_index; i++) {
        //     printf("%s\n", exclude_list[i]);
        // }

        printf("ENTERING INTO LOOP\n");
        // elements in exclude list shall be ignored, anything else is used as a template for searching in a file
        for (int i = 1; i < argc; i++) {
            int res = is_in_exclude_list(argv[i], exclude_list, exclude_list_index);
            printf("%d\n", res);
            if (res == 0) {
                pattern_list[*count_patterns] = strdup(argv[i]);
                (*count_patterns)++;
            }
        }
        printf("DONE WITH THE LOOP\n");
        
        pattern_list = (char**)realloc(pattern_list, (*count_patterns) * sizeof(char*));
        if (pattern_list == NULL && *count_patterns > 0) {
            perror("Couldn't readjust memory to amount of patterns");
            exit(EXIT_FAILURE);
        }
    }
    return pattern_list;
}

int is_in_exclude_list(char* string, char exclude_list[MAX_SINGLE_ARRAY_ENTRY_SYMBOLS][MAX_ARRAY_SIZE], int exclude_list_index) {
    
    printf("Function: is_in_exclude_list\n");

    int result = 0;
    for (int i = 0; i < exclude_list_index; i++) {
        int check_res = strcmp(string, exclude_list[i]);
        if (check_res == 0) {
            result = -1;
        }
    }
    printf("Returning %d\n", result);
    return result;
}

char** list_of_existing_files(int argc, char** argv, int* count_files) {

    // debug
    printf("Function: list_of_existing_files\n");

    char** file_list = (char**)malloc((argc - 1) * sizeof(char*));
    if (file_list == NULL) {
        perror("Cannot see files");
        exit(EXIT_FAILURE);
    } else {
        *count_files = 0;
        for (int i = 1; i < argc; i++) {
            FILE* file = fopen(argv[i], "r");
            if (file != NULL) {
                fclose(file);
                file_list[*count_files] = (char*)malloc((strlen(argv[i]) + 1) * sizeof(char));
                if (file_list[*count_files] == NULL) {
                    perror("Unable to allocate memory for file name");
                    exit(EXIT_FAILURE);
                }
                strcpy(file_list[*count_files], argv[i]);
                (*count_files)++;
            }
        }
        file_list = (char**)realloc(file_list, (*count_files) * sizeof(char*));
        if (file_list == NULL && *count_files > 0) {
            perror("Couldn't readjust memory to amount of readable files");
            exit(EXIT_FAILURE);
        }
    }
    return file_list;
}

void print_existing_file_names(char** file_list, int count_files) {

    printf("Function: print_existing_file_names\n");

    for (int i = 0; i < count_files; i++) {
        printf("%s\n", file_list[i]);
    }
}

void print_patterns(char** pattern_list, int count_patterns) {

    printf("Function: print_patterns\n");

    for (int i = 0; i < count_patterns; i++) {
        printf("%s\n", pattern_list[i]);
    }
}

void opter(struct Flags* flags, int argc, char** argv) {

    // debug
    printf("Function: opter\n");

    int opt;
    while ((opt = getopt(argc, argv, ":eivcln")) != -1) {
        switch (opt) {
            case 'e':
                flags->e = 0;
                break;
            case 'i':
                flags->i = 0;
                break;
            case 'v':
                flags->v = 0;
                break;
            case 'c':
                flags->c = 0;
                break;
            case 'l':
                flags->l = 0;
                break;
            case 'n':
                flags->n = 0;
                break;
            default:
                break;
        }
    }
}

void print_all_argvs(int argc, char** argv) {

    printf("Function: print_all_argvs\n");

    for (int i = 0; i < argc; i++) {
        printf("%s\n", argv[i]);
    }
}

void print_flags(struct Flags flags) {

    // debug
    printf("Function: print_flags\n");

    printf("e is - %d\n", flags.e);
    printf("i is - %d\n", flags.i);
    printf("v is - %d\n", flags.v);
    printf("c is - %d\n", flags.c);
    printf("l is - %d\n", flags.l);
    printf("n is - %d\n", flags.n);
}

int check_if_there_are_any_arguments(int argc) {

    // debug
    printf("Function: check_if_there_are_any_arguments\n");

    int result = -1;
    if (argc > 1) {
        result = 0;
    }
    return result;
}
