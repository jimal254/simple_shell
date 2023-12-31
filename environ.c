#include "shell.h"

/**
 * _myenv - prints the current environment
 * @info: Structure containing potential arguments.
 * Return: Always 0
 */
int _myenv(info_t *info)
{
    print_list_str(info->env);
    return (0);
}

/**
 * _getenv - gets the value of an environment variable
 * @info: Structure containing potential arguments.
 * @name: env var name
 * Return: the value
 */
char *_getenv(info_t *info, const char *name)
{
    list_t *node = info->env;
    char *p;

    while (node)
    {
        p = starts_with(node->str, name);

        if (p && *p)
            return (p);

        node = node->next;
    }

    return (NULL);
}

/**
 * _mysetenv - Initialize a new environment variable

