//script sync_dir.c
#include "comp_dir.h"
#include "sync_dir.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <unistd.h>


int file_exists(const char *path) {
    struct stat buffer;
    return (stat(path, &buffer) == 0);
}

/*

void sync_directories(char *dirname, LIST **my_file_list, int dir_tree) {
    DIR *dirp;
    struct dirent *dp;

    dirp = opendir(dirname);

    if (dirp == NULL) {
        perror(dirname);
        exit(EXIT_FAILURE); // Handle the error and continue with the next directory
    }

    //bool file_hidden = true;
    //char *destination_dir = realpath(dirname, NULL);
    char *destination_dir = dirname;
    LIST *current = *my_file_list;

    while (current != NULL) {
        rewinddir(dirp);
        char *source_file = current->file_name;
        //char *source_dir = current->directory_path;
        char *test_path = dir_path(destination_dir,source_file);
        if(!file_exists(test_path)) {
            printf("%d source_file %s missing in %s\n",__LINE__,source_file,destination_dir);
        }
        rewinddir(dirp); // Rewind the directory stream to the beginning
        while ((dp = readdir(dirp)) != NULL) {
            // Skip "." and ".." entries
            if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) {
                continue;
            }
            if (!file_hidden && dp->d_name[0] == '.') {
                continue; // Skip hidden files
            }
            //char *directory_file = dp->d_name;
            //printf("%d source_file: %s, source_dir: %s, directory_file: %s,destination_dir: %s\n",__LINE__,source_file,source_dir,directory_file,destination_dir);

            //if(file_exists())
        }
        current = current->next;
    }
}
*/



void sync_directories(char *dirname, LIST **my_file_list, int dir_tree) {
    DIR *dirp;
    struct dirent *dp;

    dirp = opendir(dirname);

    if (dirp == NULL) {
        perror(dirname);
        exit(EXIT_FAILURE); // Handle the error and continue with the next directory
    }

    //bool file_hidden = true;
    //char *destination_dir = realpath(dirname, NULL);
    char *destination_dir = dirname;
    // Create a list to store filenames found in the directory
    //LIST *directory_files = list_new();

    // Read and compare files from the directory and its subdirectories
    while ((dp = readdir(dirp)) != NULL) {
        // Skip "." and ".." entries
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) {
            continue;
        }
        if (!file_hidden && dp->d_name[0] == '.') {
            continue; // Skip hidden files
        }
        char *destination_file_path = dir_path(destination_dir, dp->d_name);
        //printf("%d -- %s\n",__LINE__,file_path);

        if(destination_file_path != NULL) {
            struct stat  stat_buffer;
            if (stat(destination_file_path, &stat_buffer) == 0) {
                if (S_ISDIR(stat_buffer.st_mode)) {
                    //printf("Directory: %s\n", destination_file_path);
                } else {
                    //printf("File: %s\n", file_path);
                    LIST *source_list = *my_file_list;
                    while (source_list != NULL) {
                        char *source_file = source_list->file_name;
                        char *source_file_path = dir_path(source_list->directory_path,source_file);
                        char *destination_file = dp->d_name;
                        //char *check_file_path = dir_path(destination_dir,source_file);

                        if(strcmp(source_file, destination_file) == 0) {
                            if (strcmp(source_file_path, destination_file_path) == 0) {
                                printf("File '%s' in directory '%s' is up to date.\n", destination_file, destination_dir);
                            } else {
                                printf("Copy file '%s' from '%s' to current directory %s.\n", source_file, source_file_path,destination_file_path);
                                // Implement the file copy operation here
                            }
                            break;
                        } /*else if(!file_exists(check_file_path)){
                            printf("%d source_file %s missing in %s\n",__LINE__,source_file,destination_dir);
                        }*/
                        source_list = source_list->next;
                    }
                    if(source_list == NULL) {
                        printf("Missing file '%s' found in directory '%s'.\n", dp->d_name, destination_dir);
                    }
                }
            }
        }
    }

    closedir(dirp);

}



/*
void create_not_found_contents(LIST *directory_files) {
    LIST *current = *my_file_list;
    while (current != NULL) {
        // Check if the file is not found in the directory
        if (!list_find_matching_element(directory_files, current->file_name, current->file_type, current->dir_tree)) {
            printf("%d current->directory_path: %s current->file_name: %s\n",__LINE__,current->directory_path,current->file_name);
            printf("%d des->directory_path: %s des->file_name: %s\n",__LINE__,destination_dir,current->file_name);
            
            char *source_file = dir_path(current->directory_path,current->file_name);
            char *destination_file = dir_path(destination_dir,current->file_name);
            printf("%d File %s is not found in the directory %s tree_level %d \n",__LINE__, current->file_name,dirname,current->dir_tree);
            printf("%d Copy file %s from %s to %s\n",__LINE__,current->file_name, source_file,destination_file);
        }

        current = current->next;
    }
}

*/