
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



//  CITS2002 Project 1 2023
//  Student1:   STUDENT-23846485   NAME-Kazi Md Imran


//  myscheduler (v1.0)
//  Compile with:  cc -std=c11 -Wall -Werror -o myscheduler myscheduler.c


//  THESE CONSTANTS DEFINE THE MAXIMUM SIZE OF sysconfig AND command DETAILS

#define MAX_DEVICES                     4
#define MAX_DEVICE_NAME                 20
#define MAX_COMMANDS                    10
#define MAX_COMMAND_NAME                20
#define MAX_SYSCALLS_PER_PROCESS        40
#define MAX_RUNNING_PROCESSES           50

//  NOTE THAT DEVICE DATA-TRANSFER-RATES ARE MEASURED IN BYTES/SECOND,
//  THAT ALL TIMES ARE MEASURED IN MICROSECONDS (usecs),
//  AND THAT THE TOTAL-PROCESS-COMPLETION-TIME WILL NOT EXCEED 2000 SECONDS
//  (SO WE CAN SAFELY USE 'STANDARD' 32-BIT ints TO STORE TIMES).

#define DEFAULT_TIME_QUANTUM            100
#define TIME_CONTEXT_SWITCH             5
#define TIME_CORE_STATE_TRANSITIONS     10
#define TIME_ACQUIRE_BUS                20


//  ----------------------------------------------------------------------

#define CHAR_COMMENT                    '#'

// Define 1-dimensional array named devices to hold device information
struct {
   char name[MAX_DEVICE_NAME];
   int  read_speed;
   int  write_speed;
} devices[MAX_DEVICES];

int time_quantum = DEFAULT_TIME_QUANTUM;
int device_counts = 0;  // To count device numbers found in sysconfig file

// Read configuration file
void read_sysconfig(char argv0[], char filename[])
{
   FILE *file = fopen(filename,"r");
   if (file == NULL) {
       perror("Error opening sysconfig file");
       exit(EXIT_FAILURE);
   }

   char line[100];
   while (fgets(line, sizeof(line),file) != NULL) {
      if (line[0] == CHAR_COMMENT) {
         continue; // Skip comment line
      }
      
      char name[MAX_DEVICE_NAME];
      int read_speed, write_speed;

      if (device_counts < MAX_DEVICES) {
         sscanf(line, "device %s %dBps %dBps", name, &read_speed, &write_speed);
         strcpy(devices[device_counts].name,name);
         devices[device_counts].read_speed = read_speed;
         devices[device_counts].write_speed = write_speed;
         device_counts++;
      } 
   }
   sscanf(line, "timequantum %dusec", &time_quantum);
   fclose(file);
 //  printf("%d, %d",device_counts,time_quantum);
}

// Function to dump sysconfig data
void dump_sysconfig()
{
   for (int i=0; i < device_counts; i++) { 
     printf("%-15s%-15d%-15d\n",devices[i].name,devices[i].read_speed,devices[i].write_speed);
   }
   printf("#\ntime_quantum\t%d\n",time_quantum);
}

// Function to remove extra spaces from a string
void remove_extra_spaces(char *input) 
{
    int i = 0; // Initialize i to 0
    int j = 0;
    int space_found = 0;

    // Skip leading spaces
    while (input[i] == ' ' || input[i] == '\t' || input[i] == '\n') {
        i++;
    }

    for (; input[i]; i++) {
        if (input[i] == ' ' || input[i] == '\t' || input[i] == '\n') {
            if (!space_found) {
                input[j++] = ' ';
                space_found = 1;
            }
        } else {
            input[j++] = input[i];
            space_found = 0;
        }
    }

    if (j > 0 && input[j - 1] == ' ') {
        j--;
    }

    input[j] = '\0';
}

// Data structure to store commands
struct
{
    char name[MAX_COMMAND_NAME];
    struct {
        int when;
        char sys_call_name[MAX_COMMAND_NAME];
        char device[MAX_DEVICE_NAME];
        int value1;
    } syscalls[MAX_SYSCALLS_PER_PROCESS];
    int num_syscalls;
} commands[MAX_COMMANDS];

int num_commands = 0;
int num_syscalls;


// Read command files
void read_commands(char argv0[], char filename[]) 
{
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening commands file");
        exit(EXIT_FAILURE);
    }
    char line[100];
    while (fgets(line, sizeof(line), file) != NULL) {
        char *split_line = strtok(line, "\r\n\t");
        // Remove leading and trailing whitespace from the line
        if (split_line == NULL || split_line[0] == '\0') {
            continue; // Skip empty or whitespace lines
        }

        // Check if the line starts with a comment character '#'
        if (split_line[0] == CHAR_COMMENT) {
            continue; // Skip comment lines
        }
        // Check if the line starts with a command name
        if (isalpha(split_line[0])) {
            // Initialize a new command
            if (num_syscalls > 0) {
                commands[num_commands-1].num_syscalls = num_syscalls;
                //printf("num_syscals: %d\n",commands[num_commands].num_syscalls);
            }
            num_syscalls = 0;
            strcpy(commands[num_commands].name,split_line);
            //printf("\n");
            //printf("%s\n",commands[num_commands].name);
            num_commands++;
            
        } else {
            // Parse a syscall from the command list
            remove_extra_spaces(split_line);
            //printf("%s\n",split_line);
            
            //num_syscalls++;
            int when;
            char sys_call_name[MAX_COMMAND_NAME];
            char device[MAX_DEVICE_NAME];
            int value1;

            if (sscanf(split_line, "%dusecs %s %s %d", &when, sys_call_name, device, &value1) == 4) {
                    strcpy(commands[num_commands-1].syscalls[num_syscalls].sys_call_name, sys_call_name);
                    strcpy(commands[num_commands-1].syscalls[num_syscalls].device, device); // Store the device name
                    commands[num_commands-1].syscalls[num_syscalls].when = when;
                    commands[num_commands-1].syscalls[num_syscalls].value1 = value1;
                
            } else if (sscanf(split_line, "%dusecs %s %d", &when, sys_call_name, &value1) == 3) {
                    strcpy(commands[num_commands-1].syscalls[num_syscalls].sys_call_name, sys_call_name);
                    strcpy(commands[num_commands-1].syscalls[num_syscalls].device, ""); // Store an empty string for device name
                    commands[num_commands-1].syscalls[num_syscalls].when = when;
                    commands[num_commands-1].syscalls[num_syscalls].value1 = value1;

            } else if (sscanf(split_line, "%dusecs %s %s", &when, sys_call_name, device) == 3) {
                    strcpy(commands[num_commands-1].syscalls[num_syscalls].sys_call_name, sys_call_name);
                    strcpy(commands[num_commands-1].syscalls[num_syscalls].device, device); // Store an empty string for device name
                    commands[num_commands-1].syscalls[num_syscalls].when = when;
                    commands[num_commands-1].syscalls[num_syscalls].value1 = value1;

            } else {
                    sscanf(split_line, "%dusecs %s", &when, sys_call_name);
                    strcpy(commands[num_commands-1].syscalls[num_syscalls].sys_call_name, sys_call_name);
                    strcpy(commands[num_commands-1].syscalls[num_syscalls].device, ""); // Store an empty string for device name
                    commands[num_commands-1].syscalls[num_syscalls].when = when;
            }
            num_syscalls++;

        }
    }
    if (num_syscalls > 0) {
        commands[num_commands-1].num_syscalls = num_syscalls;
    }
    fclose(file);
}

// Dump command files so it is easy to understand all commands added correctly
void dump_commands() 
{
    for (int i = 0; i < num_commands; i++) {
        printf("%s\n", commands[i].name); // Print command name
        for (int j = 0; j < commands[i].num_syscalls; j++) {
            printf("\t%dusecs %s %s %d\n", 
                commands[i].syscalls[j].when, 
                commands[i].syscalls[j].sys_call_name, 
                commands[i].syscalls[j].device, 
                commands[i].syscalls[j].value1
            );
        }
        printf("Number of Syscalls: %d\n\n", commands[i].num_syscalls);
    }
}
/*

struct {
    int pid;
    char name[MAX_COMMAND_NAME];
    int remaining_runtime;
    int time_on_cpu;
   // int time_blocked;
} processes[MAX_RUNNING_PROCESSES];
*/

int total_run_time = 0;
int time_on_cpu = 0;
int status = 0;

// Function to handle transition calculations
int transition_switch(int status) {
    // 0 - running, 1 - ready, 2 - blocked, 3 - sleeping, -1 - new
    if (status == 0) {
        total_run_time += 5;
    } else {
        total_run_time += 10;
    }
    return total_run_time;
}


// Get device's read or write data transfer speed
double get_read_write_speed(int read_or_write, char *name) {
    long speed = 0;
    // 0 - read | 1 - write
    if (read_or_write == 0) {
        for (int d = 0; d < device_counts; d++) {
            if (strcmp(devices[d].name, name) == 0) {
                speed = devices[d].read_speed;
                break;
            }
        }
    } else {
        for (int d = 0; d < device_counts; d++) {
            if (strcmp(devices[d].name, name) == 0) {
                speed = devices[d].write_speed;
                break;
            }
        }
    }
    return speed/1000000; //converting in bytes-per-microseconds
}

// Function to check process time with time quantum
void time_quantum_handler(int when, int *total_run_time, int *time_on_cpu) {
    int remaining_runtime;
    if (when < time_quantum) {
        *total_run_time += when;
        *time_on_cpu += when;
    } else {
        while (when >= time_quantum) {
            *total_run_time += time_quantum;
            *time_on_cpu += time_quantum;                
            // status changes as TQ expires
            status = 1;
            transition_switch(status); //RUNNING->READY as TQ expires
            status = 0;
            transition_switch(status); // READY->RUNNING
            remaining_runtime = when - time_quantum;
            when = remaining_runtime;
        }
        if (remaining_runtime > 0) {
            *total_run_time += when;
            *time_on_cpu += when;
            
            if (remaining_runtime == 100) {
                // status changes as TQ expires
                status = 1;
                transition_switch(status); //RUNNING->READY as TQ expires
                status = 0;
                transition_switch(status); // READY->RUNNING
            }
        }
    }
}

int find_command_index(char *name)
{
    for (int i=0; i < num_commands; i++ ) {
        if (strcmp(commands[i].name, name) == 0) {
            return i;
            break;
        }
    }
    return -1; // not found
}

// Function to round of double numbers, taken from web
int custom_ceil(double x) {
    int intX = (int)x;
    return x > intX ? intX + 1 : intX;
}

// Function to handle processes
void process_syscall(int i, int j)
{
    int when = 0;
    int spawn_index = 0;
    
    if (j == 0) {
        when = commands[i].syscalls[j].when;
    } else {
        when = commands[i].syscalls[j].when - commands[i].syscalls[j - 1].when;
    }
    status = 0;
    transition_switch(status);
    if (strcmp(commands[i].syscalls[j].sys_call_name, "exit") == 0) {
        total_run_time += 1;
        time_quantum_handler(when, &total_run_time, &time_on_cpu);
        if (spawn_index == 0) {
            return; // Exit the function
        }
    } else {
        time_quantum_handler(when, &total_run_time, &time_on_cpu);

        if (strcmp(commands[i].syscalls[j].sys_call_name, "sleep") == 0) {
            total_run_time += 1;
            status = 3;
            transition_switch(status);
            total_run_time += commands[i].syscalls[j].value1 - 10 + 1;
        } else if (strcmp(commands[i].syscalls[j].sys_call_name, "read") == 0) {
            char *name = commands[i].syscalls[j].device;
            double read_speed = get_read_write_speed(0, name);
            total_run_time += 1;
            status = 2;
            transition_switch(status);
            total_run_time += ((int)custom_ceil(commands[i].syscalls[j].value1 / read_speed) + 20);
        } else if (strcmp(commands[i].syscalls[j].sys_call_name, "write") == 0) {
            char *name = commands[i].syscalls[j].device;
            double write_speed = get_read_write_speed(1, name);
            total_run_time += 1;
            status = 2;
            transition_switch(status);
            total_run_time += ((int)custom_ceil(commands[i].syscalls[j].value1 / write_speed) + 20);
        } else if (strcmp(commands[i].syscalls[j].sys_call_name, "wait") == 0) {
            total_run_time += 1;
            status = 4;
            //transition_switch(status);
        } else if (strcmp(commands[i].syscalls[j].sys_call_name, "spawn") == 0) {
            char *com_name = commands[i].syscalls[j].device;
            spawn_index = find_command_index(com_name);
            for (int z = 0; z < commands[spawn_index].num_syscalls; z++) {
                process_syscall(spawn_index, z);
            }
        }
    }
    status = 1;
    transition_switch(status);
}


void execute_commands()
{
    for (int i = 0; i <num_commands; i++ ) {
        for (int j = 0; j < commands[i].num_syscalls; j++) {
            if(i == 0) {
                // for  the 1st syscall take the execution time as is for rest take the incremental time
                process_syscall(i, j);
            }   
        }
    }

}

int main(int argc, char *argv[])
{
//  ENSURE THAT WE HAVE THE CORRECT NUMBER OF COMMAND-LINE ARGUMENTS
    if(argc != 3) {
        printf("Usage: %s sysconfig-file command-file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

//  READ THE SYSTEM CONFIGURATION FILE
    read_sysconfig(argv[0], argv[1]);
    dump_sysconfig();
    printf("\n\n");

//  READ THE COMMAND FILE
    read_commands(argv[0], argv[2]);
    dump_commands();
    printf("\n\n");

    //int_processes();
    //int_READY_queue();
    //int_BLOCKED_queue();

//  EXECUTE COMMANDS, STARTING AT FIRST IN command-file, UNTIL NONE REMAIN
    execute_commands();
    printf("Number of devices: %d\n",device_counts);
    printf("Number of commands: %d\n",num_commands);
    printf("\n\n");

//  PRINT THE PROGRAM'S RESULTS
    printf("measurements  %i  %i\n", total_run_time, time_on_cpu);

    exit(EXIT_SUCCESS);
}

