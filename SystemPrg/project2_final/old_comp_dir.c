#include  <stdio.h>
#include <stdlib.h>
#include  <sys/types.h>
#include  <dirent.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
//#include "linked_list.c"

time_t get_file_modification_time(const char *file) {
    struct stat st;
    if (stat(file, &st) == 0) {
        return st.st_mtime;
    }
    return -1; // Return -1 on error
}

char *dir_path(char *dir, char *file) {
    char *path;
    size_t len_path = strlen(dir) + strlen(file);

    // Allocate memory for the parth
    path = (char *)malloc(len_path + 2); // +1 for '/' and +1 for the null terminator

    snprintf(path, len_path + 2, "%s/%s", dir, file);
    return path;
}

void compare_directories(char *dirname1, char *dirname2) {
    DIR             *dirp1, *dirp2;
    struct dirent   *dp1, *dp2;

    dirp1       = opendir(dirname1);
    dirp2       = opendir(dirname2);

    if(dirp1 == NULL) {
        perror(dirname1);
        exit(EXIT_FAILURE);
    } else if (dirp2 == NULL) {
        perror(dirname2);
        exit(EXIT_FAILURE);
    }
    bool file_hidden = true;

    char *dir_full_path1 = realpath(dirname1, NULL);
    char *dir_full_path2 = realpath(dirname2, NULL);
    char *dir_full_path;
    time_t modification_time;

    // Read and compare files from directory1 and directory2 and store their names in a list
    while((dp1 = readdir(dirp1)) != NULL) {
        // Skip "." and ".." entries
        if (strcmp(dp1->d_name, ".") == 0 || strcmp(dp1->d_name, "..") == 0) {
        continue;
        }

        if(!file_hidden) {
            if(dp1->d_name[0] == '.') {
                continue;
            }
        }
        
        rewinddir(dirp2); // Reset the directory stream position
        while ((dp2 = readdir(dirp2)) != NULL) {
            if (strcmp(dp2->d_name, ".") == 0 || strcmp(dp2->d_name, "..") == 0) {
            continue;
            }

            char *relative_path1 = dir_path(dirname1,dp1->d_name);
            char *relative_path2 = dir_path(dirname2,dp2->d_name);

            if(get_file_modification_time(relative_path1) < get_file_modification_time(relative_path2)) {
                dir_full_path = dir_full_path2;
                modification_time = get_file_modification_time(relative_path2);
            } else {
                dir_full_path = dir_full_path1;
                modification_time = get_file_modification_time(relative_path1);
            }
            printf("%ld\n\n\n",modification_time);

            if (strcmp(dp1->d_name, dp2->d_name) == 0) {
                if(dp1->d_type != DT_DIR) {
                    //char *file_path1 = dir_path(dirname1,dp1->d_name);
                    //char *file_path2 = dir_path(dirname2,dp2->d_name);
                    //my_file_list = list_add(my_file_list, dp1->d_name,dp1->d_type,modification_time,dir_full_path);
                    printf("%s - %s\n\n",dp1->d_name,dir_full_path);
                }
                
                if (dp1->d_type == DT_DIR) {
                    char *sub_dir_full_path1 = realpath(relative_path1, NULL);
                    char *sub_dir_full_path2 = realpath(relative_path2, NULL);
                    //printf("%s - %s",sub_dir_path1,sub_dir_path2);
                    compare_directories(sub_dir_full_path1,sub_dir_full_path2);
                }
                break;
            }
            //my_file_list = list_add(my_file_list, dp2->d_name,dp2->d_type,modification_time,dir_full_path);


        } 
    }
    
}

int main(int argc, char *argv[]) {
    //LIST *my_file_list = list_new();
    //char *directory1;
    //char *directory2;
    compare_directories(argv[1],argv[2]);
}
