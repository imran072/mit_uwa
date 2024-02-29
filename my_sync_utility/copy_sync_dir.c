#include "mysync.h"
#include "comp_dir.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>


void update_file(char destination[], char source[], size_t buffer_size) {
    FILE *fp_in = fopen(source, "rb");
    FILE *fp_out = fopen(destination, "wb");
    char *buffer;

    // Ensure that opening both files has been successful
    if (fp_in != NULL && fp_out != NULL) {
        buffer = (char *)malloc(buffer_size);
        size_t got, wrote;

        while ((got = fread(buffer, 1, sizeof buffer, fp_in)) > 0) {
            wrote = fwrite(buffer, 1, got, fp_out);
            if (wrote != got) {
                printf("Error copying files\n");
                exit(EXIT_FAILURE);
            }
        }
    } else {
        printf("Error opening files for copying\n");
        exit(EXIT_FAILURE);
    }

    // Ensure that we only close files that are open
    if (fp_in != NULL) {
        fclose(fp_in);
        free(buffer);
    }
    if (fp_out != NULL) {
        fclose(fp_out);
    }
}

void copy_file(char destination_dir[], char source_file[], size_t buffer_size) {
    FILE *fp_in = fopen(source_file, "rb");
    char *buffer;

    if (fp_in != NULL) {
        // Extract the base name of the source file
        char *source_base_name = strrchr(source_file, '/');
        if (source_base_name != NULL) {
            source_base_name++; // Move past the last '/'
        } else {
            source_base_name = source_file; // No '/' found, use the full path
        }

        // Create the destination path by combining the destination directory and the base name
        char *destination_path = dir_path(destination_dir,source_base_name);

        FILE *fp_out = fopen(destination_path, "wb");

        if (fp_out != NULL) {
            buffer = (char *)malloc(buffer_size);
            size_t got, wrote;

            while ((got = fread(buffer, 1, buffer_size, fp_in)) > 0) {
                wrote = fwrite(buffer, 1, got, fp_out);
                if (wrote != got) {
                    printf("Error copying files\n");
                    exit(EXIT_FAILURE);
                }
            }

            fclose(fp_out);
        } else {
            printf("Error opening destination file\n");
            exit(EXIT_FAILURE);
        }

        fclose(fp_in);
        free(buffer);
    } else {
        printf("Error opening source file\n");
        exit(EXIT_FAILURE);
    }
}

int create_directory(const char *path) {
    // Use the mkdir function to create the directory with read/write/execute permissions for the owner
    // and read/execute permissions for others.
    int result = mkdir(path, 0777);

    if (result != 0) {
        perror("mkdir");
        return 0;  // Return 0 to indicate failure
    }

    return 1;  // Return 1 to indicate success
}

