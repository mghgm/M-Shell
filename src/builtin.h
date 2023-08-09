#ifndef __BUILTIN_H__
# define __BUILTIN_H__

extern char *builtin_str[];

typedef int (*builtin_ptr)(char **);

/*
    Launches builtin function corresponding to builtin_str index
*/ 
builtin_ptr get_builtin_func(int index);

int msh_num_builtins();

#endif