# MySync

MySync is a command-line tool for synchronizing directories efficiently. It compares the contents of directories and ensures they are identical by copying missing files and updating existing ones. It offers various options for customizing the synchronization process.

## Features

- **Directory Comparison**: Compare the contents of directories to identify differences.
- **File Synchronization**: Synchronize files between directories, ensuring that all directories have the same files.
- **Recursive Comparison**: Support for recursively comparing and synchronizing subdirectories.
- **Option Flags**: Customizable options for controlling the synchronization process, such as ignoring hidden files, copying only, verbose output, etc. (yet to be implemented)
- **Linked List Implementation**: Utilizes a linked list data structure for efficient storage and manipulation of file information.

## Installation

To install MySync, follow these steps:

1. Clone the repository from GitHub:
    `git clone https://github.com/imran072/mit_uwa.git`
2. Navigate to the project directory:
    `cd mit_uwa/my_sync_utility`
3. Compile the source code:
    `make`

## Usage

Run the executable with appropriate command-line arguments:
    `./mysync [options] directory1 directory2`

Options:
-  -a    Include hidden files in the comparison
-  -i    Ignore patterns specified in the command-line argument (yet to be implemented)
-  -n    Perform a trial run with no changes made
-  -r    Recursively compare and synchronize subdirectories
-  -v    Enable verbose output for detailed information
