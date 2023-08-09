#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include<unistd.h>

#include "builtin.h"

#define MSH_RL_BUFSIZE 1024
#define MSH_TOK_BUFSIZE 64
#define MSH_TOK_DELIM " \t\r\n\a"

void msh_loop(void);
char *msh_read_line();
char **msh_split_line(char *line);
int msh_launch(char **args);
int msh_execute(char **args);

int main(int argc, char **argv) {

    msh_loop();

    return EXIT_SUCCESS;
}

void msh_loop(void) {
    char *line;
    char **args;
    int status = 1;
    
    do {
        printf("> ");
        line = msh_read_line();
        args = msh_split_line(line);
        status = msh_execute(args);

        free(line);
        free(args);
    } while(status);

}

char *msh_read_line(void) {
    char *line = NULL;
    ssize_t bufsize = 0;

    if (getline(&line, &bufsize, stdin) == -1) {
        if (feof(stdin)) {
            exit(EXIT_SUCCESS);
        } else {
            perror("readline");
            exit(EXIT_FAILURE);
        }
    }

    return line;
}

// the following is replaced by msh_read_line() using getline()
char *msh_read_line_legacy(void) {
    int bufsize = MSH_RL_BUFSIZE;
    int position = 0;
    char *buffer = malloc(sizeof(char) * bufsize);
    int c;

    if (!buffer) {
        fprintf(stderr, "msh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        c = getchar();

        if (c == EOF || c == '\n') {
            buffer[position] = '\0';
            return buffer;
        } else {
            buffer[position] = c;
        }
        position++;

        if (position >= bufsize) {
            bufsize += MSH_RL_BUFSIZE;
            buffer = realloc(buffer, bufsize);
            if (!buffer) {
                fprintf(stderr, "msh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

char **msh_split_line(char *line) {
    int bufsize = MSH_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens) {
        fprintf(stderr, "msh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, MSH_TOK_DELIM);
    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += MSH_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));

            if (!tokens) {
                fprintf(stderr, "msh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, MSH_TOK_DELIM);
    }

    tokens[position] = NULL;
    return tokens;
}

int msh_launch(char **args) {
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) {
        printf("I'm child.\n");
        // Child process
        if (execvp(args[0], args) == -1) {
            perror("msh");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Error forking
        perror("msh");
    } else {
        printf("I'm parent.\n");
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int msh_execute(char **args) {
    printf("hi\n");
    // if (args[0] == NULL) {
    //     // An empty command entered
    //     return 1;
    // }

    // for (int i = 0; i < msh_num_builtins(); i++) {
    //     if (strcmp(args[0], builtin_str[i]) == 0) {
    //         return (*get_builtin_func(i))(args);
    //     }
    // }
    printf("salam\n");
    return msh_launch(args);
}