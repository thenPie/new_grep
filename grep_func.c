#include "grep_func.h"

#include <stdio.h>
#include <unistd.h>

#include <stdlib.h>
#include <string.h>

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
    struct Flags flags = {-1, -1, -1, -1, -1, -1};
    int count_files;
    char** existing_files = list_of_existing_files(argc, argv, &count_files);;
    int valid = 0;
    
    valid = check_if_there_are_any_arguments(argc);

    if (valid == 0) {
        opter(&flags, argc, argv);

        // delete later
        print_existing_file_names(existing_files, count_files);
    }

    // freeing file names
    if (existing_files != NULL) {
        for (int i = 0; i < count_files; i++) {
            if (existing_files[i] != NULL) {
                free(existing_files[i]);
            }
        }
        free(existing_files);
    }

    // delete this line later
    printf("%d is the final valid variable", valid);
}

void print_existing_file_names(char** file_list, int count_files) {
    for (int i = 0; i < count_files; i++) {
        printf("%s\n", file_list[i]);
    }
}

char** list_of_existing_files(int argc, char** argv, int* count_files) {
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

void opter(struct Flags* flags, int argc, char** argv) {
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
    for (int i = 0; i < argc; i++) {
        printf("%s\n", argv[i]);
    }
}

void print_flags(struct Flags flags) {
    printf("e is - %d\n", flags.e);
    printf("i is - %d\n", flags.i);
    printf("v is - %d\n", flags.v);
    printf("c is - %d\n", flags.c);
    printf("l is - %d\n", flags.l);
    printf("n is - %d\n", flags.n);
}

int check_if_there_are_any_arguments(int argc) {
    int result = -1;
    if (argc > 1) {
        result = 0;
    }
    return result;
}