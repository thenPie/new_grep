#include "grep_func.h"

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
  int count_patterns;
  char** existing_files = list_of_existing_files(argc, argv, &count_files);
  char** patterns = list_of_patterns(argc, argv, &count_patterns,
                                     existing_files, count_files);
  int valid = 0;

  valid = check_if_there_are_any_arguments(argc);

  if (valid == 0) {
    opter(&flags, argc, argv);
    grep_func(existing_files, patterns, count_files, count_patterns);
  }

  free_list(patterns, count_patterns);
  free_list(existing_files, count_files);
}

void grep_func(char** existing_files, char** patterns, int count_files,
               int count_patterns) {
  int reti;
  char msgbuf[100];
  for (int i = 0; i < count_files; i++) {
    FILE* file = fopen(existing_files[i], "r");
    if (file == NULL) {
      fprintf(stderr, "Could not open file: %s\n", existing_files[i]);
      continue;
    }
    regex_t regex[count_patterns];
    int compile_success = 1;
    for (int j = 0; j < count_patterns; j++) {
      reti = regcomp(&regex[j], patterns[j], 0);
      if (reti) {
        fprintf(stderr, "Could not compile regex for pattern %s\n",
                patterns[j]);
        compile_success = 0;
        break;
      }
    }
    if (!compile_success) {
      fclose(file);
      for (int k = 0; k < count_patterns; k++) {
        regfree(&regex[k]);
      }
      continue;
    }

    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, file)) != -1) {
      for (int j = 0; j < count_patterns; j++) {
        reti = regexec(&regex[j], line, 0, NULL, 0);
        if (!reti) {
          printf("%s", line);
        } else if (reti == REG_NOMATCH) {
          continue;
        } else {
          regerror(reti, &regex[j], msgbuf, sizeof(msgbuf));
          fprintf(stderr, "Regex match failed: %s\n", msgbuf);
        }
      }
    }
    fclose(file);
    if (line != NULL) {
      free(line);
      line = NULL;
    }
    for (int k = 0; k < count_patterns; k++) {
      regfree(&regex[k]);
    }
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

int is_in_exclude_list(
    char* string,
    char exclude_list[MAX_SINGLE_ARRAY_ENTRY_SYMBOLS][MAX_ARRAY_SIZE],
    int exclude_list_index) {
  int result = 0;
  for (int i = 0; i < exclude_list_index; i++) {
    int check_res = strcmp(string, exclude_list[i]);
    if (check_res == 0) {
      result = -1;
    }
  }
  return result;
}

void print_existing_file_names(char** file_list, int count_files) {
  for (int i = 0; i < count_files; i++) {
    printf("%s\n", file_list[i]);
  }
}

void print_patterns(char** pattern_list, int count_patterns) {
  for (int i = 0; i < count_patterns; i++) {
    printf("%s\n", pattern_list[i]);
  }
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

// TODO: make list_functions into different functions, code
// duplication!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

char** list_of_patterns(int argc, char** argv, int* count_patterns,
                        char** file_list, int count_files) {
  char** pattern_list = (char**)malloc((argc - 1) * sizeof(char*));
  if (pattern_list == NULL) {
    perror("Cannot see patterns");
    exit(EXIT_FAILURE);
  } else {
    *count_patterns = 0;
    char exclude_list[MAX_ARRAY_SIZE][MAX_SINGLE_ARRAY_ENTRY_SYMBOLS] = {
        "-e", "-i", "-v", "-c", "-l", "-n"};
    int exclude_list_index = 6;

    for (int i = 0; i < count_files; i++) {
      strncpy(exclude_list[exclude_list_index], file_list[i],
              MAX_SINGLE_ARRAY_ENTRY_SYMBOLS - 1);
      exclude_list[exclude_list_index][MAX_SINGLE_ARRAY_ENTRY_SYMBOLS - 1] =
          '\0';
      exclude_list_index += 1;
    }

    // elements in exclude list shall be ignored, anything else is used as a
    // template for searching in a file
    for (int i = 1; i < argc; i++) {
      int res = is_in_exclude_list(argv[i], exclude_list, exclude_list_index);
      if (res == 0) {
        pattern_list[*count_patterns] = strdup(argv[i]);
        (*count_patterns)++;
      }
    }

    pattern_list =
        (char**)realloc(pattern_list, (*count_patterns) * sizeof(char*));
    if (pattern_list == NULL && *count_patterns > 0) {
      perror("Couldn't readjust memory to amount of patterns");
      exit(EXIT_FAILURE);
    }
  }
  return pattern_list;
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
        file_list[*count_files] =
            (char*)malloc((strlen(argv[i]) + 1) * sizeof(char));
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
