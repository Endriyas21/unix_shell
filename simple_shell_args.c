#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT_SIZE 1024

int main(void)
{
    char input[MAX_INPUT_SIZE];
    char *args[MAX_INPUT_SIZE / 2]; // Assuming each argument is a single word

    while (1)
    {
        printf("#cisfun$ "); /* Display the prompt */
        fflush(stdout);     /* Flush the output buffer to ensure the prompt is displayed */

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            /* Handle end of file (Ctrl+D) */
            printf("\n");
            break;
        }

        /* Remove the trailing newline character */
        if (input[strlen(input) - 1] == '\n')
        {
            input[strlen(input) - 1] = '\0';
        }

        /* Check if the input is not empty */
        if (strlen(input) > 0)
        {
            /* Tokenize the input into arguments */
            int num_args = 0;
            char *token = strtok(input, " ");
            while (token != NULL)
            {
                args[num_args++] = token;
                token = strtok(NULL, " ");
            }
            args[num_args] = NULL; // Null-terminate the argument list

            /* Use the fork and execve functions to execute the command */
            pid_t pid = fork();

            if (pid == -1)
            {
                perror("fork");
            }
            else if (pid == 0)
            {
                /* Child process */
                if (execve(args[0], args, NULL) == -1)
                {
                    /* Handle execution error */
                    perror(args[0]);
                    exit(EXIT_FAILURE);
                }
            }
            else
            {
                /* Parent process */
                int status;
                if (waitpid(pid, &status, 0) == -1)
                {
                    perror("waitpid");
                }
            }
        }
    }

    return (0);
}