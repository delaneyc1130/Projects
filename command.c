/*
* Description: Implementation of the Unix System Commands : ls, pwd, mkdir, cd, cp, mv,
*
* Author: Delaney Carleton
*
* Date: April 8, 2020
*
* Notes:
* 1. Implementation of Unix-like commands using system calls
  2. Comments throughout file
  3. In cp attempted to handle "." and ".." but errors continued to occur but have my code commented in my attempt.
     After further research using resources such as geeksforgeeks and stackoverflow found a greater understanding 
     of implementing this by using a string comparison and offsetting the path. 
     This is then implemented in the parse token function within main.
  4. Tried to account for all possible erros that could occur with each of the commands.
*/

/*-------------------------Preprocessor Directives---------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "command.h"
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
/*---------------------------------------------------------------------------*/ 

/*----------------------------Program Main-----------------------------------*/ 

void listDir() /* ls command */
{
    //Define Variables
    struct dirent *de;
    DIR *dir;

    //Open directory
    dir = opendir(".");

    //Check if Empty
    if (dir == NULL)
    {
        write(1, "Error! Unrecognized Directory Not Found\n", strlen("Error! Unrecognized Directory Not Found\n")); 
    }

    //Loop through directories and write
    while ((de = readdir(dir)) != NULL)
    {
        write(1, de->d_name, strlen(de->d_name));
        write(1, " ", 1);
    }
  
    //Add newline for spacing, otherwise format incorrect 
    write(1, "\n", 1);

    //Close directory
    closedir(dir);
}

void showCurrentDir() /* pwd command */
{
    //Define Variables
    int bufsize = 1024;
    char buffer[bufsize];

    //Check if Empty with getcwd system call
    if (getcwd(buffer, bufsize) == NULL)
    {
        write(1, "Error! Current Directory Not Found\n", strlen("Error! Current Directory Not Found\n"));
    }

    else {

    //System Call write current directory
        write(1, buffer, strlen(buffer));
        write(1, "\n", 1);

    }

}

void makeDir(char *dirname) /* mkdir command */
{
    //Check if Directory Name Given
    if (dirname == NULL)
    {

        write(1, "Error! Unable to Create Directory\n", strlen("Error! Unable to Create Directory\n"));

    }

    else {

        //System Call
        //Mode : 01000 - give directory permission to be searchable and executable
        mkdir(dirname, 1000);

    }
}

void changeDir(char *dirname) /* cd command */
{

    //Check if Directory Name Given
    if (dirname == NULL)
    {
        write(1, "Error! Unable to Change Directory\n", strlen("Error! Unable to Change Directory\n"));
    }

    else 
    { 

        //System Call
        chdir(dirname);

    }

}

void copyFile(char *sourcePath, char *destinationPath) /* cp command */
{

    //Define Variable
    int status;
    char *buffer = NULL;
    size_t bufsize = 1024; 

//    char *new_destinationPath = NULL;

    int f;
    int copy_f;

    //Allocate Buffer
    buffer = (char *)calloc(bufsize, sizeof(char));

    //Open file with access mode Read-Only
    f = open(sourcePath, O_RDONLY);

    //Check for File
    if (f  == -1)
    {
        write(1, "Error with Opening File\n", strlen("Error with Opening File\n"));
    }

    /*
    //Check for . and .. 
    //Was not copy file correctly -- moved this logic to parse token function in main
    if ((strncmp(sourcePath,".", 1)) || (strncmp(sourcePath, "..", 1)))
    {
        destinationPath = destinationPath + 3;
        new_destinationPath = destinationPath + 3;
    }
    */


 //   else
 //  {

    //Open and Create File with access mode Write-Only and flag to Create File with correct permissions
    copy_f = open(destinationPath, O_WRONLY | O_CREAT, 0777);        

    //Read File and Write to New File
    read(f, buffer, bufsize);
    write(copy_f, buffer, strlen(buffer));

//  }


    //Close Files
    close(f);
    close(copy_f);
    
    //Free allocated memory
    free(buffer);


}

void moveFile(char *sourcePath, char *destinationPath) /* mv command */
{

    /*
    //System Call
    rename(sourcePath, destinationPath);
    */

    //Check for Error otherwise command runs
    if ((rename(sourcePath, destinationPath)) == -1)
    {

        write(1, "Error. No file exists.\n", strlen("Error. No file exists.\n"));

    }
}

void deleteFile(char *filename) /* rm command */
{

/*
    //System Call
    unlink(filename);
*/

    //Check for Error otherwise command runs
    if((unlink(filename)) == -1)
    {

        write(1, "Error. No file exists.\n", strlen("Error. No file exists.\n"));

    }
}


void displayFile(char *filename) /* cat command */
{
    //Define Variable
    int f;
    char *buffer;
    size_t bufsize = 1024;
    int nread;

    //Open File with flag for read-only access mode
    f = open(filename, O_RDONLY);

    //Allocate Buffer
    buffer = (char *)calloc(bufsize, sizeof(char));
    
    //Check for File
    if(f == -1)
    {

        write(1, "Error with Opening File.\n", strlen("Error with Opening File.\n"));

    }

    //Read through file
    while((nread = read(f, buffer, bufsize)) > 0)

    {
        //Write lines of file to buffer
        write(1, buffer, bufsize);
    }

    //Close File
    close(f);

    //Free Variable
    free(buffer);

}

/*-----------------------------Program End-----------------------------------*/
