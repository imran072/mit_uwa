/*this script name is linked_list.c*/

#include <stdio.h>  // For printf
#include <stdlib.h> // For calloc, exit, and NULL
#include <string.h> // For strcmp and strdup
#include "linked_list.h"

#if	defined(__linux__)
extern	char	*strdup(char *string);
#endif

LIST *list_new(void)
{
    return NULL;
}
/*
typedef struct file_info
{
    char *file_name;              // File name
    time_t last_mod_time;         // Last modified time
    char *directory_name;         // Directory name
    struct file_info   *next;      // Pointer to the next file node
} LIST;
*/

bool list_find(LIST *list, char *file_name, int file_type, int dir_tree)
{
    while(list != NULL) {
        if (strcmp (list->file_name, file_name) == 0 &&  list->file_type == file_type && list->dir_tree == dir_tree) {
            return true;
        }
        list = list->next;
    }
    return false;
}

// Function to find a matching element in the linked list
LIST *list_find_matching_element(LIST *list, char *file_name, int file_type, int dir_tree) {
    while (list != NULL) {
        if (strcmp(list->file_name, file_name) == 0 && list->file_type == file_type && list->dir_tree == dir_tree) {
            return list; // Return the matching element
        }
        list = list->next;
    }
    return NULL; // Return NULL if no matching element is found
}

//  ALLOCATE SPACE FOR A NEW LIST ITEM, TESTING THAT ALLOCATION SUCCEEDS
LIST *list_new_item(char *file_name, int file_type, time_t last_mod_time, char *directory_path, int dir_tree)
{
    LIST *new       = calloc(1, sizeof(LIST) );
    CHECK_ALLOC(new);
    new->file_name     =  strdup(file_name);
    CHECK_ALLOC(new->file_name);
    new->file_type = file_type;
    new->last_mod_time = last_mod_time;
    new->directory_path = directory_path;
    CHECK_ALLOC(new->directory_path);
    new->dir_tree = dir_tree;
    new->next       =  NULL;
    return new;
}

//  ADD A NEW (STRING) ITEM TO AN EXISTING LIST
LIST *list_add(LIST *list, char *file_name, int file_type, time_t last_mod_time, char *directory_path, int dir_tree)
{
    if(list_find(list, file_name, file_type, dir_tree)) {            // only add each item once
        return list;
    }
    else {                                      // add new item to head of list
        LIST *new   = list_new_item(file_name, file_type, last_mod_time, directory_path, dir_tree);
        new->next   = list;
        return new;
    }
}

/*
LIST *list_add(LIST *oldlist, char *file_name, int file_type, time_t last_mod_time, char *directory_path)
{
    if(list_find(oldlist, file_name, file_type)) {
        return oldlist;
    }
    LIST *new = malloc(sizeof(LIST));
    if (new == NULL) {
        perror("Memory allocation failed for a file node.\n");
        return NULL; // Return NULL to indicate an error
    }

    new->file_name = strdup(file_name);
    if (new->file_name == NULL) {
        fprintf(stderr, "String duplication failed.\n");
        free(new); // Free the allocated memory before returning
        return NULL;
    }
    new->file_type = file_type;
    new->last_mod_time = last_mod_time;
    new->directory_path = directory_path;
    new->next = oldlist;
    return new;
}
*/

/*
void list_print(LIST *list) {

    LIST *current = list; // Create a temporary pointer to traverse the list
    while (current != NULL) {
        printf("file_name: %s file_type: %d last_mod_time: %ld directory_path: %s\n", current->file_name, current->file_type, current->last_mod_time, current->directory_path);
        current = current->next; // Move to the next node using the temporary pointer
    }
    free(current);
}
*/

//  PRINT EACH ITEM (A STRING) IN A GIVEN LIST TO stdout
void list_print(LIST *list)
{
    if (verbose) {
        if (list != NULL) {
            while (list != NULL) {
                printf("dir_tree: %d file_name: %s file_type: %d last_mod_time: %ld directory_path: %s\n", list->dir_tree, list->file_name, list->file_type, list->last_mod_time, list->directory_path);
                if (list->next != NULL) {
                    printf(" -> ");
                }
                list = list->next;
            }
            printf("\n");
        }
    }
}

