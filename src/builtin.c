#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<limits.h>

#include "builtin.h"

int msh_cd(char **args);
int msh_pwd(char **args);
int msh_help(char **args);
int msh_exit(char **args);

char *builtin_str[] = {
    "cd",
    "pwd",
    "help",
    "exit"
};

int (*builtin_func[]) (char **) = {
    &msh_cd,
    &msh_pwd,
    &msh_help,
    &msh_exit,
};

int msh_num_builtins() {
    return sizeof(builtin_str)/sizeof(char *);
}

int msh_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "msh: excepted argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("msh");
        }
    }
    return 1;
}

int msh_pwd(char **args) {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("msh");
    } else {
        printf("%s\n", cwd);
    }
    return 1;
}

int msh_help(char **args) {
    int i;
    printf("Mohsen Ghasemi's MSH\n");
    printf("This Shell is mostly inspired by Stephen Brennan's LSH.(Check https://github.com/brenns10/lsh)");
    printf("Type program names and arguments, and hit enter.\n");
    printf("The following are built in:\n");

    for (i = 0; i < msh_num_builtins(); i++) {
        printf("  %s\n", builtin_str[i]);
    }

    printf("Use the man command for information on other programs.\n");
    return 1;
}

int msh_exit(char **args) {
    return 0; 
}


builtin_ptr get_builtin_func(int index) {
    return builtin_func[index];
}