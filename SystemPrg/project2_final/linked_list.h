//linked_list.h
#include <sys/stat.h>
#include <stdbool.h>
#include <stdio.h>  // For perror
#include <stdlib.h> // For exit
#include <time.h>


//  A HELPFUL PREPROCESSOR MACRO TO CHECK IF ALLOCATIONS WERE SUCCESSFUL
#define CHECK_ALLOC(p) if(p == NULL) { perror(__func__); exit(EXIT_FAILURE); }

typedef struct file_info
{
    char *file_name;              // File name
    int file_type;                
    time_t last_mod_time;         // Last modified time
    char *directory_path;         // Directory path
    int dir_tree;                 // 0 - base dir, 1 - sub, 2 sub-sub dir and so on
    struct file_info   *next;      // Pointer to the next file node
} LIST;

extern LIST *list_new(void);

extern LIST *list_add(LIST *oldlist, char *file_name, int file_type, time_t last_mod_time, char *directory_path, int dir_tree);

extern bool list_find(LIST *list, char *file_name, int file_type, int dir_tree);

extern LIST *list_find_matching_element(LIST *list, char *file_name, int file_type, int dir_tree);

extern void list_print(LIST *list);

extern bool verbose;


