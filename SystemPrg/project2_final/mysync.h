//mysync.h
//  CITS2002 Project 2 2023
//  Student1:   23846485   Kazi Md Imran
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// function to update file between two directories
extern void update_file(char destination[], char source[], size_t buffer_size);

extern void copy_file(char destination_dir[], char source_file[], size_t buffer_size);

// create directory based on path
extern int create_directory(const char *path);

// Function to compare and create missing subfolders
extern void compare_and_create(char *dir1, char *dir2);