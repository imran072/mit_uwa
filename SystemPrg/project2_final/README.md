# Background
We increasingly use multiple computers, such as those at university, at places of employment, personal desktop computers, and laptops. Unfortunately, because cloud-based services such as Dropbox and OneDrive are not always available, we may have only a subset of our files on the computer we're using. We (should also) employ portable storage devices, such as USB thumb-drives to backup and transport copies of our files between computers but, over time, we need to manually identify the most up-to-date version of a file before we overwrite another file of the same name.
We'd like to synchronise files between our computers and our USB thumb-drive (so that both hold the most up-to-date copy of the same files), travel home, and then synchronise our USB thumb-drive with our home computer or laptop. Eventually, all three locations will hold identical copies of the same files.

The activity of ensuring that two sets of files are identical, is termed file synchronization, or just syncing.

For this project, two sets of files are deemed identical if each file exists in the same relative location below a 'top-level' directory, with the same filename, and with identical contents.

The goal of this project is to design and develop a command-line utility program, named mysync, to synchronise the contents of two or more directories.
 
Successful completion of the project will develop and enhance your understanding of advanced features of the C11 programming language, POSIX function calls, and the use of the make utility.

# Program invocation
The program will be invoked from the command-line with zero-or-more options (switches) and two-or-more directory names:
prompt> ./mysync  [options]  directory1  directory2  [directory3  ...]

The program's options are:

-a	By default, 'hidden' and configuration files (those beginning with a '.' character) are not synchronised because they do not change very frequently. The -a option requests that all files, regardless of whether they begin with a '.' or not, should be considered for synchronisation.

-i pattern	Filenames matching the indicated pattern should be ignored; all other (non-matching) filenames should be considered for synchronisation. The -i option may be provided multiple times.

-n	By default, mysync determines what files need to be synchronised and then silently performs the necessary copying. The -n option requests that any files to be copied are identified, but they are not actually synchronised. Setting the -n option also sets the -v option.

-o pattern	Only filenames matching the indicated pattern should be considered for synchronisation; all other (non-matching) filenames should be ignored. The -o option may be provided multiple times.

-p	By default, after a file is copied from one directory to another, the new file will have the current modification time and default file permissions. The -p option requests that the new copy of the file have the same modification time and the same file permissions as the old file.

-r	By default, only files found immediately within the named directories are synchronised. The -r option requests that any directories found within the named directories are recursively processed.

-v	By default, mysync performs its actions silently. No output appears on the stdout stream, although some errors may be reported on the stderr stream. The -v option requests that mysync be more verbose in its output, reporting its actions to stdout. There is no required format for the (your) debug printing (it will be ignored during marking).
File patterns
Globbing, also known as wildcard expansion, is a feature in Unix-like operating systems (including Linux and macOS) that allows you to use special characters, termed wildcards, to match and specify multiple files and directories in a concise way. The wildcard characters *, ?, [ ], and { } are widely supported. Normally, globbing is performed by the shell before it passes the expanded patterns as arguments to a new child process.
Within our own programs, such as mysync with its -i and -o options, we need to perform globbing ourselves. A simple way to determine if a glob-pattern matches a given filename, is to first convert the glob-pattern to a regular-expression, and to then see if the regular expression matches the filename. The provided function glob2regex performs the first part of this process, and the library functions regcomp and regexec, used in sequence, perform the second.

Note that, because the shell expands wildcards, that you'll need to enclose your file patterns within single-quotation characters. For example, the following command will (only) synchronise your C11 files:

prompt> ./mysync  -o  '*.[ch]'  ....

and the following command will synchronise all files except your laboratory exercises:

prompt> ./mysync  -i  'lab?-Q*.*'  ....
