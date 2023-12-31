#include "my_shell.h"

/**
 * my_shell_loop - main shell loop
 * @shell_info: the parameter & return shell_info struct
 * @shell_args: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int my_shell_loop(info_t *shell_info, char **shell_args)
{
    ssize_t r = 0;
    int builtin_ret = 0;

    while (r != -1 && builtin_ret != -2)
    {
        clear_info(shell_info);
        if (is_interactive(shell_info))
            _puts("$ ");
        _eputchar(BUF_FLUSH);
        r = get_input(shell_info);
        if (r != -1)
        {
            set_info(shell_info, shell_args);
            builtin_ret = find_builtin_command(shell_info);
            if (builtin_ret == -1)
                find_shell_command(shell_info);
        }
        else if (is_interactive(shell_info))
            _putchar('\n');
        free_info(shell_info, 0);
    }
    write_history(shell_info);
    free_info(shell_info, 1);
    if (!is_interactive(shell_info) && shell_info->status)
        exit(shell_info->status);
    if (builtin_ret == -2)
    {
        if (shell_info->err_num == -1)
            exit(shell_info->status);
        exit(shell_info->err_num);
    }
    return (builtin_ret);
}

/**
 * find_builtin_command - finds a builtin command
 * @shell_info: the parameter & return shell_info struct
 *
 * Return: -1 if builtin not found,
 *         0 if builtin executed successfully,
 *         1 if builtin found but not successful,
 *         -2 if builtin signals exit()
 */
int find_builtin_command(info_t *shell_info)
{
    int i, built_in_ret = -1;
    builtin_table builtintbl[] = {
        {"exit", my_exit},
        {"env", my_env},
        {"help", my_help},
        {"history", my_history},
        {"setenv", my_setenv},
        {"unsetenv", my_unsetenv},
        {"cd", my_cd},
        {"alias", my_alias},
        {NULL, NULL}
    };

    for (i = 0; builtintbl[i].type; i++)
        if (_strcmp(shell_info->argv[0], builtintbl[i].type) == 0)
        {
            shell_info->line_count++;
            built_in_ret = builtintbl[i].func(shell_info);
            break;
        }
    return (built_in_ret);
}

/**
 * find_shell_command - finds a command in PATH
 * @shell_info: the parameter & return shell_info struct
 *
 * Return: void
 */
void find_shell_command(info_t *shell_info)
{
    char *path = NULL;
    int i, k;

    shell_info->path = shell_info->argv[0];
    if (shell_info->linecount_flag == 1)
    {
        shell_info->line_count++;
        shell_info->linecount_flag = 0;
    }
    for (i = 0, k = 0; shell_info->arg[i]; i++)
        if (!is_delim(shell_info->arg[i], " \t\n"))
            k++;
    if (!k)
        return;

    path = find_path(shell_info, _getenv(shell_info, "PATH="), shell_info->argv[0]);
    if (path)
    {
        shell_info->path = path;
        fork_shell_command(shell_info);
    }
    else
    {
        if ((is_interactive(shell_info) || _getenv(shell_info, "PATH=")
             || shell_info->argv[0][0] == '/') && is_cmd(shell_info, shell_info->argv[0]))
            fork_shell_command(shell_info);
        else if (*(shell_info->arg) != '\n')
        {
            shell_info->status = 127;
            print_error(shell_info, "not found\n");
        }
    }
}

/**
 * fork_shell_command - forks an exec thread to run cmd
 * @shell_info: the parameter & return shell_info struct
 *
 * Return: void
 */
void fork_shell_command(info_t *shell_info)
{
    pid_t child_pid;

    child_pid = fork();
    if (child_pid == -1)
    {
        perror("Error:");
        return;
    }
    if (child_pid == 0)
    {
        if (execve(shell_info->path, shell_info->argv, get_environ(shell_info)) == -1)
        {
            free_info(shell_info, 1);
            if (errno == EACCES)
                exit(126);
            exit(1);
        }
    }
    else
    {
        wait(&(shell_info->status));
        if (WIFEXITED(shell_info->status))
        {
            shell_info->status = WEXITSTATUS(shell_info->status);
            if (shell_info->status == 126)
                print_error(shell_info, "Permission denied\n");
        }
    }
}
