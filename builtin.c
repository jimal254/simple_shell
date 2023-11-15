#include "shell.h"

/**
 * _myexit - Exits the shell.
 * @info: Structure containing potential arguments.
 * Return: Exits with a given exit status (0 if info->argv[0] != "exit").
 */
int _myexit(info_t *info)
{
    int exitcheck;

    if (info->argv[1])
    {
        exitcheck = _erratoi(info->argv[1]);

        if (exitcheck == -1)
        {
            info->status = 2;
            print_error(info, "Illegal number: ");
            _eputs(info->argv[1]);
            _eputchar('\n');
            return (1);
        }

        info->err_num = _erratoi(info->argv[1]);
        return (-2);
    }

    info->err_num = -1;
    return (-2);
}

/**
 * _mycd - Changes the current directory of the process.
 * @info: Structure containing potential arguments.
 * Return: Always 0.
 */
int _mycd(info_t *info)
{
    char *s, *dir, buffer[1024];
    int chdir_ret;

    s = getcwd(buffer, sizeof(buffer));

    if (!s)
        _puts("TODO: >>getcwd failure emsg here<<\n");

    if (!info->argv[1])
    {
        dir = _getenv(info, "HOME=");
        chdir_ret = chdir((dir) ? dir : "/");
    }
    else if (_strcmp(info->argv[1], "-") == 0)
    {
        dir = _getenv(info, "OLDPWD=");
        if (!dir)
        {
            _puts(s);
            _putchar('\n');
            return (1);
        }

        _puts(dir), _putchar('\n');
        chdir_ret = chdir(dir);
    }
    else
    {
        chdir_ret = chdir(info->argv[1]);
    }

    if (chdir_ret == -1)
    {
        print_error(info, "can't cd to ");
        _eputs(info->argv[1]), _eputchar('\n');
    }
    else
    {
        _setenv(info, "OLDPWD", _getenv(info, "PWD="));
        _setenv(info, "PWD", getcwd(buffer, sizeof(buffer)));
    }

    return (0);
}

/**
 * _myhelp - Prints help message.
 * @info: Structure containing potential arguments.
 * Return: Always 0.
 */
int _myhelp(info_t *info)
{
    (void)info; // To silence unused variable warning
    _puts("Help message. Function not yet implemented.\n");
    return (0);
}
