//comp_dir.h
#ifndef COMP_DIR_H
#define COMP_DIR_H
#include <stdbool.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>   // For printf
#include <stdlib.h>  // For exit
#include <sys/stat.h> // For stat
#include <string.h>  // For strcmp and strdup
#include <stdbool.h>
#include <stdarg.h>


#include "linked_list.h"

// Function to get the last modification time of a file
extern time_t get_file_modification_time(char *file);

// Function to get the size time of a file
extern time_t get_file_size(char *file);

// Function to check if it is a dir or reg file
extern bool is_dir(char *dirname);

extern char *dir_path(char *dir, char *file);

extern void compare_directories(char *directory, LIST **my_file_list, int dir_tree);

extern void updateList(LIST *list, char *file_name, int file_type, time_t modification_time, char *directory_path, int dir_tree);

extern void sync_directories(char *dirname, LIST **my_file_list, int dir_tree);

extern void debug_print(char *format, ...);

extern bool file_hidden;

extern void usage(int exit_code);
extern bool copy_only;
extern bool ignore_patterns;
extern bool pattern_matching;
extern bool permission;
extern bool recursive;


//extern int dir_tree;
//extern LIST *my_file_list;

#endif // COMP_DIR_H