//comp_dir.c
//#include "get_file_modification_time.c"
#include "comp_dir.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>


#if	defined(__linux__)
extern	char	*strdup(char *string);
#endif





time_t get_file_modification_time(char *file) {
    struct stat st;
    if (stat(file, &st) == 0) {
        return st.st_mtime;
    }
    return -1; // Return -1 on error
}

time_t get_file_size(char *file) {
    struct stat st;
    if (stat(file, &st) == 0) {
        return st.st_size;
    }
    return -1; // Return -1 on error
}

bool is_dir(char *dirname) {
  struct stat  st;

  if(stat(dirname, &st) == 0) {
    if (S_ISDIR(st.st_mode)) {
        return true;
    } else {
        return false;
    }
  } else {
    return false;
  }
}


// Function to construct a full path by concatenating a directory path and a file name
char *dir_path(char *dir, char *file) {
    size_t len_path = strlen(dir) + 1 + strlen(file); // +1 for '/'
    char *path = (char *)malloc(len_path + 1); // +1 for null terminator

    if (path != NULL) {
        snprintf(path, len_path + 1, "%s/%s", dir, file);
    }

    return path;
}

//LIST *my_file_list;

// Function to update the linked list with file information
void updateList(LIST *list, char *file_name, int file_type, time_t modification_time, char *directory_path, int dir_tree) {
    while (list != NULL) {
        if (strcmp(list->file_name, file_name) == 0 && list->file_type == file_type) {
            // If the file exists in the current directory being processed
            if (strcmp(list->directory_path, directory_path) != 0) {
                //printf("%d %s %ld %s\n",__LINE__,list->file_name,list->last_mod_time,list->directory_path);
                //printf("%d %ld\n",__LINE__,modification_time);
                if (list->last_mod_time < modification_time) {
                    //printf("True\n");
                    list->file_name = strdup(file_name);
                    list->last_mod_time = modification_time;
                    //free(list->directory_path); // Free the old directory path
                    list->directory_path = strdup(directory_path); // Update directory path
                    //printf("%d %s %ld %s\n",__LINE__,list->file_name,list->last_mod_time,list->directory_path);
                    //printf("%d %s\n",__LINE__,list->directory_path);
            } 

            //printf("***%d %s %ld %s\n",__LINE__,list->file_name,list->last_mod_time,list->directory_path);
            return; // No need to continue searching
            }
            
        }
        list = list->next;
    }
}

// Function to compare directories and update the linked list
void compare_directories(char *dirname, LIST **my_file_list, int dir_tree) {
    DIR *dirp;
    struct dirent *dp;

    dirp = opendir(dirname);

    if (dirp == NULL) {
        perror(dirname);
        exit(EXIT_FAILURE); // Handle the error and continue with the next directory
    }

    //bool file_hidden = true;
    //char *dir_full_path = realpath(dirname, NULL);
    char *dir_full_path = dirname;

    // Read and compare files from the directory and its subdirectories
    while ((dp = readdir(dirp)) != NULL) {
        // Skip "." and ".." entries
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) {
            continue;
        }

        //char *relative_path = realpath(dir_path(dirname, dp->d_name), NULL);
        char *relative_path = dir_path(dirname, dp->d_name);
    
        time_t modification_time = get_file_modification_time(relative_path);
        //printf("%d %s %ld %s\n\n",__LINE__,dp->d_name,modification_time,relative_path);

        if (!file_hidden && dp->d_name[0] == '.') {
            continue; // Skip hidden files
        }
        char *dir_dp_name = dir_path(dirname,dp->d_name);        

        if (!is_dir(dir_dp_name)) {
            if (!list_find(*my_file_list, dp->d_name, dp->d_type, dir_tree)) {
                *my_file_list = list_add(*my_file_list, dp->d_name, dp->d_type, modification_time, dir_full_path, dir_tree);
                //printf("%d\n",__LINE__);
                //printf("dir_tree %d file_name: %s file_type: %d last_mod_time: %ld directory_path: %s\n", dir_tree, dp->d_name, dp->d_type, modification_time, dir_full_path);
            } else if (list_find(*my_file_list, dp->d_name, dp->d_type, dir_tree)) {
                //printf("%d\n",__LINE__);
                //printf("dir_tree %d file_name: %s file_type: %d last_mod_time: %ld directory_path: %s\n", dir_tree,dp->d_name, dp->d_type, modification_time, dir_full_path);
                updateList(*my_file_list, dp->d_name, dp->d_type, modification_time, dir_full_path, dir_tree);
                //printf("%d file_name: %s file_type: %d last_mod_time: %ld directory_path: %s\n",__LINE__,dp->d_name, dp->d_type, modification_time, dir_full_path);
            }
        }
           
        if (is_dir(dir_dp_name) && recursive) {
            //char *sub_dir_full_path = realpath(relative_path, NULL);
            char *sub_dir_full_path = relative_path;
            //printf("%d %s\n",__LINE__, sub_dir_full_path);
            //printf("%d dir_tree %d file_name: %s file_type: %d last_mod_time: %ld directory_path: %s\n",__LINE__, dir_tree,dp->d_name, dp->d_type, modification_time, dir_full_path);
            compare_directories(sub_dir_full_path, my_file_list, dir_tree + 1); // Increment dir_tree for subdirectories
            printf("%d sub-dir_tree %d file_name: %s file_type: %d last_mod_time: %ld directory_path: %s\n",__LINE__, dir_tree,dp->d_name, dp->d_type, modification_time, dir_full_path);
            //free(sub_dir_full_path); // Free the subdirectory path
        }
        
        //printf("%d\n",__LINE__);
        //free(dir_full_path); // Free the relative path
        //printf("%d\n",__LINE__);
    }

    //free(dir_full_path); // Free the directory path
    //printf("%d\n",__LINE__);
    closedir(dirp); // Close the directory
    //printf("%d\n",__LINE__);
    //list_print(my_file_list);
}

void usage(int exit_code) {
    printf("Usage: mysync [options] dir1 dir2 [dir3 ....]\n");
    printf("Where options are:\n");
    printf("  -a            synchronize all files.\n");
    printf("  -i pattern    ignore files matching the pattern.\n");
    printf("  -n           report actions that would be performed, but do not synchronize\n");
    printf("  -o pattern    only synchronize files matching the pattern.\n");
    printf("  -p            preserve modification times and protection modes.\n");
    printf("  -r            recursively synchronize directories.\n");
    printf("  -v		    be verbose and report actions.\n");
    exit(exit_code);
}


void debug_print(char *format, ...) {
    if (verbose) {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }
}