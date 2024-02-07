#include <sys/stat.h>
#include "comp_dir.h"

time_t get_file_modification_time(char *file) {
    struct stat st;
    if (stat(file, &st) == 0) {
        return st.st_mtime;
    }
    return -1; // Return -1 on error
}