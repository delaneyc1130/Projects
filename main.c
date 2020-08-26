/*
* Description: The main function for Pseudo-Shell that is able to start in one of two modes: File mode or Interactive Mode.
               If in Interactive mode, the shell will accept user input for Unix commands and write output to the console.
               If in File mode, the shell will accept a file name as input for Unix commands and all output will be written to a output.txt file.
               The parse_token function is called within main in order to parse the input string and divide them into tokens
               to identify Unix Commands being called and the arguments they take. 
*
* Author: Delaney Carleton
*
* Date: April 8, 2020
*
* Notes: 
* 1. Parse token function written to check tokens for system command calls
  2. Parse token function implemented in main function for interactive and file mode
  3. Comments written throughout file
  4. Could not get error handling correct  for when a line begins with the control code. 
     My attempts at implementation are commented 
  5. Although the other errors are handled, there is possibly a different error case or an
     additional error case that outputs when an error occurs.
  6. Attempt to account for all errors and segmentation faults that could occur.
*/

/*-------------------------Preprocessor Directives---------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include "command.h"

/*---------------------------------------------------------------------------*/

void parse_token(char *token)
{

    //Define Variables
    char *parse_arg = NULL;
    char *args = NULL;
    char *arg1 = NULL;
    char *arg2 = NULL;
    char *arg3 = NULL;

    //Parse token by space
    parse_arg = strtok(token, " ");

    //Get Arguments from the line - parse by newline
    arg1 = strtok(NULL, " \n");
    arg2 = strtok(NULL, " \n");
    arg3 = strtok(NULL, " \n");

    /*
    Check for Incorrect Syntax if begin with control code
    I had issues with handling this error. I through it might possibly catch the error of beginning with the control code by checking
    before the while loop.
    While I was able to have it catch the syntax error if the line starts with the the control code, errors throughout the rest of 
    the commands occurred.
    
    if (strcmp(parse_arg, ";" ) == 0)
    {
        printf(1, "Error! Incorrect syntax with control code.\n", strlen("Error! Incorrect syntax with control code.\n"));
    }
    */
/*
    if (arg1 == NULL)
    {
        printf(1, "Error! Incorrect syntax with control code.\n", strlen("Error! Incorrect syntax with control code.\n"));
    }
    */

    //Check for Commands while parsing tokens
    while (parse_arg != NULL)
    {

        //ls command
        if ((strcmp(parse_arg, "ls") == 0) || (strstr(parse_arg, "ls") != NULL))
        {
            //Error Check if arguments 
            if (arg1 != NULL)
            {
                //Test to check Control Code Error
                //If took in too many arguments without a control code or if took in same command without control code
                if (arg3 != NULL || (strcmp(arg1, "ls") == 0))
                {
                    write(1, "Error! Incorrect syntax. No control code found.\n", strlen("Error! Incorrect syntax. No control code found.\n"));
//                    break;
                }

                else 
                {
                    write(1, "Error! Unsupported parameters for command: ls\n", strlen("Error! Unsupported parameters for command: ls\n"));

                }
     
            }

            else 
            {
                listDir();
            }

        }

        //pwd command
        else if ((strcmp(parse_arg, "pwd") == 0) || (strstr(parse_arg, "pwd") != NULL))
        {
            //Error Check if arguments
            if (arg1 != 0)
            {

                //Check Control Code Error
                if (arg3 != NULL || strcmp(arg1, "pwd") == 0)
                {
                    write(1, "Error! Incorrect syntax. No control code found.\n", strlen("Error! Incorrect syntax. No control code found.\n"));
 //                   break;
                }

                else 
                {
                    write(1, "Error! Unsupported parameters for command: pwd\n", strlen("Error! Unsupported parameters for command: pwd\n"));
                }
            }

            else
            {
                showCurrentDir();                
            }
        }

        //mkdir command
        else if ((strcmp(parse_arg, "mkdir") == 0) || (strstr(parse_arg, "mkdir") != NULL))
        {
            //arg1 = strtok(NULL, " \n");

            //Error Check if no arguments or too many arguments
            if (arg1 == 0 || arg2 != NULL)
            {

                write(1, "Error! Unsupported parameters for command: mkdir\n", strlen("Error! Unsupported parameters for command: mkdir\n"));

            }

            //Check control code error
            else if (arg3 != NULL || strcmp(arg1, "mkdir") == 0)
            {
                write(1, "Error! Incorrect syntax. No control code found.\n", strlen("Error! Incorrect syntax. No control code found.\n"));
            }

            else {

                makeDir(arg1);
            }

        }

        //cd command
        else if ((strcmp(parse_arg, "cd") == 0) || (strstr(parse_arg, "cd") != NULL))
        {

            //Error Check if no arguments or too many arguments
            if (arg1 == 0 || arg2 != NULL)
            {   

                write(1, "Error! Unsupported parameters for command: cd\n", strlen("Error! Unsupported parameters for command: cd\n"));

            }  

            //Error check syntax
            else if (arg3 != NULL || strcmp(arg1, "cd") == 0)
            {
                write(1, "Error! Incorrect syntax. No control code found.\n", strlen("Error! Incorrect syntax. No control code found.\n"));
            }

            else{

                changeDir(arg1);
            }
        }

        //cp command
        else if ((strcmp(parse_arg, "cp") == 0) || (strstr(parse_arg, "cp") != NULL))
        {
        
            //Error Check if 2 arguments not given or given too many arguments
            if (arg1 == 0 || arg2 == 0 || arg3 != NULL)
            {

                write(1, "Error! Unsupported parameters for command: cp\n", strlen("Error! Unsupported parameters for command: cp\n"));
//                break;

            }

            //Error check syntax
            else if (strcmp(arg1, "cp") == 0)
            {
                write(1, "Error! Incorrect syntax. No control code found.\n", strlen("Error! Incorrect syntax. No control code found.\n"));
            }

            //Account for .. and .
            //Attempt to originally account for this in command file but errors occurred
            //After research found success in implementing this within the parse token function 
            else if (strncmp(arg1, ".", 1) == 0 || strncmp(arg1, "..", 1) == 0)
            {

                //Call function with new destination path so offset correctly
                copyFile(arg1, (arg1 + 3));

            }

            else
            {
                copyFile(arg1, arg2);
            }
        }


        //mv command
        else if ((strcmp(parse_arg, "mv") == 0) || (strstr(parse_arg, "mv") != NULL))
        {

            //Error Check if 2 arguments not given
            if (arg1 == 0 || arg2 == 0 || arg3 != NULL)
            {

                write(1, "Error! Unsupported parameters for command: mv\n", strlen("Error! Unsupported parameters for command: mv\n"));

            }

            else if (strcmp(arg1, "mv") == 0)
            {
                write(1, "Error! Incorrect syntax. No control code found.\n", strlen("Error! Incorrect syntax. No control code found.\n"));
            }

            else 
            {
                moveFile(arg1, arg2);
            }
        }

        //rm command
        else if ((strcmp(parse_arg, "rm") == 0) || (strstr(parse_arg, "rm") != NULL))
        {

            //Error check if no argument given or given too many arguments
            if (arg1 == 0 || arg2 != NULL)
            {

                write(1, "Error! Unsupported parameters for command: rm\n", strlen("Error! Unsupported parameters for command: rm\n"));

            } 

            //Error check syntax
            else if (arg3 != NULL || strcmp(arg1, "rm") == 0)
            {   
                write(1, "Error! Incorrect syntax. No control code found.\n", strlen("Error! Incorrect syntax. No control code found.\n"));
            }   


            else 
            {
                deleteFile(arg1);
            }
        }

        //cat command
        else if ((strcmp(parse_arg, "cat") == 0) || (strstr(parse_arg, "cat") != NULL))
        {

            //Error Check if no argument given or given too many arguments
            if (arg1 == 0 || arg2 != NULL)
            {
                write(1, "Error! Unsupported parameters for command: cat\n", strlen("Error! Unsupported parameters for command: cat\n"));
            }

            //Error check syntax
            else if (arg3 != NULL || strcmp(arg1, "cat") == 0)
            {
                write(1, "Error! Incorrect syntax. No control code found.\n", strlen("Error! Incorrect syntax. No control code found.\n"));
            }

            else {

                displayFile(arg1);

            }
        }

        else {

            //Error Check: Given an argument that's not a command
            write(1, "Error! Unrecognized command: ", strlen("Error! Unrecognized command: "));
            write(1, token, strlen(token));
            write(1, "\n",1);

        }

        //Tokenize on " "
        parse_arg = strtok(NULL, " ");
    }
}

/*-----------------------------Program Main----------------------------------*/
int main(int argc, char *argv[]) {
    setbuf(stdout, NULL);
    
    /* Main Function Variables */

    FILE *stream;

    char *token = NULL;

    char filemodeStr[5];
    strcpy(filemodeStr, "-f");

    char *buffer;
    ssize_t nread;
    size_t bufsize = 1024;
    char *save_buffer = NULL;

    int condition = 1;

    
    /* Allocate memory for the input buffer. */
    buffer = (char *)malloc(bufsize * sizeof(char));

    /*Mode: File mode or Interative Mode*/

    //If User Mode ./pseudo-shell
    if (argc < 2) 
    {

     // Check if in Interactive Mode
    //  write(1, "In Interactive Mode\n", strlen("In Interactive Mode\n"));

        //Take standard input from console
        stream = stdin;

    }
    
    //If File Mode : -f flag with text file
    else if ((argc == 3) && ((strcmp(filemodeStr, argv[1])) == 0))
    {

        //Check if in File Mode
    //  write(1, "In File Mode\n", strlen("In File Mode\n"));

        //Set input to file to read
        stream = fopen(argv[2], "r");

        //Create file to write all output
        freopen("output.txt", "w", stdout);
    }

    else {

        //Error Check if not Interactive or File Mode
        write(1, "Error! Not a valid Mode for Pseudo Shell\n", strlen("Error! Not a valid Mode for Pseudo Shell\n"));

        //Free buffer to avoid memory leaks
        free(buffer);
        //Exit Program
        exit(1);
    }


    //do-while loop 
    do {

        //Interactive Mode
        if (argc < 2) 
        {
            //Prompt user
            printf(">>> ");

            //Get user input string
            nread = getline(&buffer, &bufsize, stream);

            save_buffer = buffer;

            //Tokenize on ;
//            while((token = strtok_r(buffer, ";", &buffer)) != NULL) {
//            Issues with implementing strtok_r buffer after viewing manual pages
//            and tutorials found to have a save pointer as an argument for strtok_r 
//            in order for it to be invoked properly
            while((token = strtok_r(save_buffer, ";", &save_buffer)) != NULL)
            {

                //Check for exit command 
                if (strcmp(token, "exit") == 0 || strcmp(token, "exit\n") == 0)
                {
                    free(buffer);
                    exit(1);
                }

                /*
                //Check if line starts with control code of ;
                //Problems with this implementation. I've tried implementing it within the parse token function which is
                //where I think the check for this should be but the error does not seem to catch
                if(strcmp(token, ";") == 0)
                {
                    write(1, "Error! Unrecognized command: \n", strlen("Error! Unrecognized command: \n"));
                }
                */

                //Call function to parse user input token for commands
                parse_token(token);

            }
        }


        //File Mode
        else if ((strcmp(filemodeStr, argv[1]) == 0)) 
        {

            //Get lines of input file
            while ((nread = getline(&buffer, &bufsize, stream)) != -1)
            {

                save_buffer = buffer;

                /* Inititally implemented strtok_r with buffer but errors occurred.
                   Through reading manual page of strtok_r and viewing examples 
                   discovered I needed to set a save pointer for strtok_r in order
                   to tokenize
                   while ((token = strtok_r(buffer, ";\n\r", &buffer)) != NULL)
                */
                /*Delimeter set to ";\n\r" to account for newline and carriage return */

                while ((token = strtok_r(save_buffer, ";\n\r", &save_buffer)) != NULL)
                {
                    //Call function to parse file input token for commands
                    parse_token(token);

                }
            }

            //Break out of loop when done reading File
            condition = 0;

        }


    } while(condition);

    
    //If File Mode, close the file
    if (strcmp(filemodeStr, argv[1]) == 0) {
        fclose(stream);
    }

    //Free the allocated memory
    free(buffer);

    //Return
    return 1;
}
/*-----------------------------Program End-----------------------------------*/

