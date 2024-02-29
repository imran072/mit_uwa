//script sync_dir.c

#include "comp_dir.h"
#include "mysync.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <unistd.h>


int file_exists(const char *path) {
    struct stat buffer;
    return (stat(path, &buffer) == 0);
}

void sync_directories(char *dirname, LIST **my_file_list, int dir_tree) {
    DIR *dirp;
    struct dirent *dp;
    
    dirp = opendir(dirname);

    if (dirp == NULL) {
        perror(dirname);
        exit(EXIT_FAILURE); // Handle the error and continue with the next directory
    }
    char *destination_dir = dirname;
    LIST *source_list = *my_file_list;

    while (source_list != NULL) {
        if(source_list->dir_tree == dir_tree) {
            //rewinddir(dirp);
            //printf("%d - %d\n",__LINE__,dir_tree);
            char *source_file_name = source_list->file_name;
            char *source_file = dir_path(source_list->directory_path, source_file_name);

            //printf("%d source_file_name %s source_file %s destination_dir %s\n",__LINE__,source_file_name,source_file,destination_dir);
        
            //char *source_dir = current->directory_path;
            char *check_file_path = dir_path(destination_dir, source_file_name);
            //printf("%d check_file_path %s source_list->directory_path %s destination_dir %s\n",__LINE__,check_file_path,source_list->directory_path,destination_dir);
            if (!file_exists(check_file_path)) {
                //debug_print("%d source_file %s missing in %s\n", __LINE__, source_file, destination_dir);
                if (!copy_only) {
                    size_t buffer_size = get_file_size(source_file);
                    copy_file(destination_dir, source_file, buffer_size);
                } else {
                    debug_print("File '%s' will be copied to '%s'\n" ,source_file, check_file_path);
                }
            }
        }
        source_list = source_list->next;
    }
    // Read and compare files from the directory and its subdirectories
    while ((dp = readdir(dirp)) != NULL) {
        // Skip "." and ".." entries
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) {
            continue;
        }
        if (!file_hidden && dp->d_name[0] == '.') {
            continue; // Skip hidden files
        }
        char *dir_dp_name = dir_path(dirname,dp->d_name); 
        
        if(is_dir(dir_dp_name)){
            //printf("directory %s name in dir %s\n",dp->d_name,destination_dir);
            char *sub_dir = dir_path(destination_dir, dp->d_name);
            //printf("@@@@@^^^^^^%s\n",sub_dir);
            int sub_dir_tree = dir_tree + 1;
            sync_directories(sub_dir, my_file_list, sub_dir_tree);
            //printf("sub-dir\n");
            //printf("%d %s %d\n",__LINE__,sub_dir,sub_dir_tree);
        } else {
            LIST *match = list_find_matching_element(*my_file_list, dp->d_name, dp->d_type, dir_tree);
            if (match != NULL && !is_dir(dir_dp_name)) {
                char *source_dir = match->directory_path;
                if (strcmp(source_dir, destination_dir) == 0) {
                    //debug_print("%d file: %s source_dir: %s destination_dir: %s\n",__LINE__,match->file_name,source_dir,destination_dir);
                    //printf("File '%s' in directory '%s' is up to date.\n", destination_dir, destination_dir);
                    debug_print("File %s in directory %s is up to date.\n", match->file_name, destination_dir);
                } else {
                    char *source_file = dir_path(source_dir, match->file_name);
                    char *destination_file = dir_path(destination_dir, match->file_name);
                    //printf("%d file: %s source_dir: %s destination_dir: %s\n",__LINE__,match->file_name,source_file,destination_file);
                    if (!copy_only) {
                        debug_print("Copy file '%s/%s' from %s to '%s'\n", match->directory_path,match->file_name, source_file, destination_file);
                        size_t buffer_size = get_file_size(source_file);
                        update_file(destination_file, source_file, buffer_size);
                    } else {
                        debug_print("File '%s/%s' will be copied to %s\n", match->directory_path,match->file_name, destination_dir);
                    }
                }
            }
        }
    }
    closedir(dirp);
}

void compare_and_create(char *dir1, char *dir2) {
    struct dirent *dp;
    DIR *dirp = opendir(dir1);
    DIR *dirp2 = opendir(dir2);

    if (dirp == NULL) {
        //perror("dir1");
        return;
    }
    
    while ((dp = readdir(dirp))) {
        char *dir_dp_name = dir_path(dir1,dp->d_name); 
        //printf("IMU dir_dp_name %s name %s\n",dir_dp_name,dp->d_name);
        if (is_dir(dir_dp_name)) {
            if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) {
                continue;
            }

            char *subfolder1 = dir_path(dir1, dp->d_name);
            char *subfolder2 = dir_path(dir2, dp->d_name);

            if (opendir(subfolder2) == NULL) {
                if (!copy_only) {
                    create_directory(subfolder2);
                } else {
                    printf("Folder '%s' will be created in '%s'\n", dp->d_name, dir2);
                }
            }

            free(subfolder1);
            free(subfolder2);

            if (!copy_only) {
                compare_and_create(subfolder1, subfolder2);
            }
        }
    }

    // Repeat the same process for subfolders found in dir2 but missing in dir1
    if (dirp2 != NULL) {
        while ((dp = readdir(dirp2))) {
            char *dir_dp_name = dir_path(dir2,dp->d_name); 
            //printf("IMU2 dir_dp_name %s name %s\n",dir_dp_name, dp->d_name);
            if (is_dir(dir_dp_name) && strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
                char *subfolder1 = dir_path(dir1, dp->d_name);
                char *subfolder2 = dir_path(dir2, dp->d_name);

                if (opendir(subfolder1) == NULL) {
                    if (!copy_only) {
                        create_directory(subfolder1);
                    } else {
                        printf("Folder '%s' will be created in '%s'\n", dp->d_name, dir1);
                    }
                }

                free(subfolder1);
                free(subfolder2);

                if (!copy_only) {
                    compare_and_create(subfolder1, subfolder2);
                }
            }
        }
        closedir(dirp2);
    }

    closedir(dirp);
}
