//read_dir.c
#include  <stdio.h>
#include <stdlib.h>
#include  <sys/types.h>
#include  <dirent.h>
#include "read_dir.h"

// function to get the modification timestamp of a file
time_t get_file_modification_time(const char *file) {
    struct stat st;
    if (stat(file, &st) == 0) {
        return st.st_mtime;
    }
    return -1; // Return -1 on error
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

    // Read and compare files from directory1 and directory2 and store their names in a list
    while((dp1 = readdir(dirp1)) != NULL) {
        // Check if the file is a regular file or not
        if(dp1->d_type == DT_REG) {
            rewinddir(dirp2); // Reset the directory stream position
            while ((dp2 = readdir(dirp2)) != NULL) {
                if (dp2->d_type == DT_REG && strcmp(dp1->d_name, dp2->d_name) == 0) {
                    //printf("Common: %s\n", entry1->d_name);
                    if(dp1.)
                    break;
                }
            }    
        } 
    }
    
}

void list_directory(char *dirname)
{
    DIR             *dirp;
    struct dirent   *dp;

    dirp       = opendir(dirname);
    if(dirp == NULL) {
        perror(dirname);
        exit(EXIT_FAILURE);
    }

    while((dp = readdir(dirp)) != NULL) {  
        printf( "%s\n", dp->d_name );
    }
    closedir(dirp);
}


void read_file(char *filename)
{
    FILE  *fp;
    char  line[BUFSIZ];

    fp       = fopen(filename, "r");
    if(fp == NULL) {
        perror(filename);
        exit(EXIT_FAILURE);
    }

    while(fgets(line, sizeof(line), fp) != NULL) {
        printf( "%s", line);
    }
    fclose(fp);
}

void copy_file (char *sourceFile, char *destinationFile) {
    FILE *sf, *df;
    char ch;

    sf = fopen(sourceFile,"rb");
    if (sf == NULL) {
        perror("Failed to open the source file");
        exit(EXIT_FAILURE);
    }

    df = fopen(destinationFile,"wb");
    if (df == NULL) {
        perror("Failed to open the destination file");
        fclose(df); // Close the source file
        exit(EXIT_FAILURE);
    }

    while ((ch = fgetc(sf)) != EOF) {
        fputc(ch, df);
    }

    // Close both files
    fclose(sf);
    fclose(df);

    printf("File copied successfully.\n");
    //return 0;
}

int main(int argc, char *argv[]) {
    char *directory1;
    char *directory2;
    
    for (int i = 1; i < argc; i++) {
        if (i == 1) {
            directory1 = argv[1];
            directory2 = argv[2];
        }
    }
}