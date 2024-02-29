//script mysync.c
#include "comp_dir.h"
#include "mysync.h"
#include <getopt.h>
#include <unistd.h>
#include <stdio.h>
#if	defined(__linux__)
extern	char	*strdup(char *string);
#endif

#define OPTLIST     "ai:noprv"

int main(int argc, char *argv[]) {
    
    int opt;

    while((opt = getopt(argc, argv, OPTLIST)) != -1) {
        switch(opt) {
            case 'a':
            file_hidden = true;
            break;
            case 'i':
            ignore_patterns = true;
            char *pattern = strdup(optarg);
            printf("%s\n",pattern);
            printf("%d",ignore_patterns);
            break;
            case 'n':
            copy_only = true;
            verbose = true;
            break;
            case 'r':
            recursive = true;
            //printf("%d\n",recursive);
            break;
            case 'v':
            verbose = true;
            break;
            default:
            // Handle invalid options
            fprintf(stderr, "Invalid option: -%c\n", optopt);
            usage(EXIT_FAILURE);
        
        }
    }
    printf("optind %d argc %d\n",optind,argc);
    if(argc - optind < 2) {  
        fprintf(stderr, "Error: You must provide at least two directory\n");  //  display mysync's usage/help   
        usage(EXIT_FAILURE);
    }
    
/*
    for (int i = optind; i < argc; i++) {
        printf("Non-option argument: %s\n", argv[i]);
    }
*/


    /*
    argc  -= optind;
    argv  += optind;
    */
    

    LIST *my_file_list = list_new(); // Initialize the list
    int dir_tree;

    for (int i = optind; i < argc; ++i) {
        dir_tree = 0;
        if(!is_dir(argv[i])) {
            fprintf(stderr, "Error: '%s' is not a valid directory.\n", argv[i]);
            usage(EXIT_FAILURE);
        }
        //printf("****************%s\n\n",argv[i]);
        compare_directories(argv[i], &my_file_list, dir_tree);
        //printf("****************\n\n");
        //list_print(my_file_list);
        //printf("xxxxx%s\n",argv[i]);
    }
    //printf(".............\n\n");
    list_print(my_file_list);
    //printf(".............\n\n");
    //printf("check");

    for (int i = optind; i < argc; ++i) {
        dir_tree = 0;
        if(i + 1 < argc && recursive) {
            compare_and_create(argv[i], argv[i+1]);
            //printf("got!!\n");
        }
        //printf("^^^^^^^^^^^^%s\n\n",argv[i]);
        sync_directories(argv[i], &my_file_list, dir_tree);
        //printf("****************\n\n");
        //list_print(my_file_list);
        //printf("xxxxx%s\n",argv[i]);
    }
    /**/
    argc -= optind;
    argv += optind;
    
    return EXIT_SUCCESS;
}
