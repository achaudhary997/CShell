# OS Assignment 0

#### Working of the shell
The way this shell works is that we have a while True loop which is the main loop for our shell. Inside that first the input is parsed using strtok using space as delimiter, then depending on the command which user typed in and based on that the respective command will be called. Depending on the input the commands can be:

* `echo`: arguments parsed and printed to stdout. (the text to be printed should not have any space)
* `history`: reads a file containing the history commands in the cuurent directory.
* `cd`: cd supports ~ and - (previous directory) along with ```cd directory_name```.
* `pwd`: prints the current working directory using getcwd() function.
* `exit`: calls exit

For External commands a child process is spawned using fork() and then the appropriate binary from the commandsCode folder. Once executed the control is returned back to the main program.

* `ls`: This function is implemented using dirent and opendir.s
* `cat`: reads a file and prints the values to stdout.
* `date`: prints the current date using struct time and strftime.
* `mkdir`: cretes a directory in the current directory.
* `rm`: used to remove files and directories.

---
## Commands

I implemented the following commands:
**Internal Commands**

1. Echo - Prints whatever you give to it in stdout.
   1. Flags
      * ```-n``` Won't print a newline in the end.
      * ```-E``` Won't process escape sequences. 
   1. Usage
      1. `echo -n "ABCD"`
      1. `echo -E "\x41\x42"`
      1. `echo -nE "\x41ABCD"`
1. History
   1. Flags
      * `-c` Clears the history file.
      * `10` Prints the last 10 commands in history.
   1. Usage
      1. `history`
      1. `history -c`
1. cd
      1. ```cd ..``` Go to previous directory
      1. ```cd ~``` Go to Home directory (Uses environment variables)
      1. ```cd -``` Goes to last accessed directory (Uses environment variables)
      1. ```cd``` Goes to Home directory
      1. ```cd validPath``` Goes to the directory validPath
1. pwd
   1. `pwd` Prints the current working directory (Uses environment variables)
   1. Usage
      1. `pwd aldskfjasf`
      1. `pwd -asdfkj`
1. exit
   1. `exit` Exits from the shell.

#### External Commands

1. ls
   1. ```ls``` Prints the current directory listing.
   1. Usage
      1. ```ls -a``` Prints the current directory listing along with the hidden files.
      1. ```ls -m``` Prints the current directory listing using , as the separator.
      1. ```ls path/to/Directory``` Prints the contents to the directory *path/to/directory*.
1. cat
   1. prints the contents of the file.
   1. Usage
      1. ```cat``` Will open a stdin stdout loopback to exit press CTRL+C.
      1. ```cat myshell.c``` Prints the contents of the myshell.c to stdout.
      1. ```cat -n myshell.c``` Prints the contents of myshell.c with line numbers in the beginning.
      1. ```cat -E myshell.c``` Prints the contents of myshell.c with *$* at the end of each line.
1. date
   1. ```date``` Prints todays date/time.
   1. ```date -R``` Prints the current date/time in RFC format.
   1. ```date -u``` Prints the date/time in the UTC region.
1. rm
   1. ```rm -v``` Verbose output
   1. ```rm -d``` Removed directories.
   1. Usage
      1. ```rm -d abcd``` Given abcd exists
      1. ```rm -v lsa``` Given ls exists
1. mkdir
   1. ```mkdir <name>``` Creates the specified directory
   1. Usage
      1. ```mkdir -v abcd``` Verbose output
      1. ```mkdir -p abcd/efgh/hi``` Creates given directory structure.

---
### Corner Cases
1. ls
   1. ls \<invalid-directory>
   1. ls \<invalid-argument>
1. mkdir
   1. mkdir \<already-existing-dir>
   1. mkdir
1. rm
   1. rm
   1. rm \<dir-name>
1. cat
   1. cat \<file-which-does-not-exist>
   1. cat
1. date
   1. date \<invalid-argument>
   1. date aldsjf
1. cd
   1. cd
   1. cd \<dir-which-does-not-exist>
1. echo
   1. echo "hello
   1. echo
1. history
   1. history \<number-greater-than-num-of-lines>
   1. history 0
1. pwd
   1. pwd \<random-string>
   1. pwd \<invalid-flag>

---

#### Handling bugs/errors
1. First the most basic one was stack overflow using fgets was the best option.
1. For myshell even a single enter press or a `\n` character was parsed as a command patched this.
1. For echo -E it was important to new take any \ as escape character.
1. Using any random file name with `rm`, `cat`, `mkdir`.
1. Any unknown command.

#### Extra:
Pipelining works for a single pipe only. Using inbuilt binaries for this and not the ones which I wrote. Syntax:

   1. ```cat myshell.c | wc -l```
   2. ```ls -l | wc -l```

Redirection also works. You can use >, <, >> to write/read/append to a file. Example

   1. ```cat myshell.c > test.txt```

   1. ```wc -l < test.txt```

   1. ```ls >> test.txt```


#### Note

1. There should not be any space in the directory name while doing cd.
1. The commandsCode folder should be present in the same directory and should contain all the commands .c and executable files.
1. For commands like `cat` and others the flags should be before the fileName or else it won't work.
1. Multiple command line arguments together in the same line won't work.


## Name: Anubhav Chaudhary

## Roll No: 2016013