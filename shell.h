#include "my_shell.h"

int my_shell(shell_info_t *shell_info, char **shell_args)
{
	ssize_t read_status = 0;
	int builtin_result = 0;

	while (read_status != -1 && builtin_result != -2)
	{
		clear_shell_info(shell_info);
		if (is_interactive(shell_info))
			_puts("$ ");
		_eputchar(BUF_FLUSH);
		read_status = get_input(shell_info);
		if (read_status != -1)
		{
			set_shell_info(shell_info, shell_args);
			builtin_result = find_builtin_command(shell_info);
			if (builtin_result == -1)
				find_shell_command(shell_info);
		}
		else if (is_interactive(shell_info))
			_putchar('\n');
		free_shell_info(shell_info, 0);
	}
	write_history(shell_info);
	free_shell_info(shell_info, 1);
	if (!is_interactive(shell_info) && shell_info->status)
		exit(shell_info->status);
	if (builtin_result == -2)
	{
		if (shell_info->err_num == -1)
			exit(shell_info->status);
		exit(shell_info->err_num);
	}
	return (builtin_result);
}

int find_builtin_command(shell_info_t *shell_info)
{
	int i, built_in_result = -1;
	builtin_table_t builtintbl[] = {
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

	for (i = 0; builtintbl[i].command; i++)
		if (_strcmp(shell_info->argv[0], builtintbl[i].command) == 0)
		{
			shell_info->line_count++;
			built_in_result = builtintbl[i].func(shell_info);
			break;
		}
	return (built_in_result);
}

void find_shell_command(shell_info_t *shell_info)
{
	char *path = NULL;
	int i, arg_count;

	shell_info->path = shell_info->argv[0];
	if (shell_info->linecount_flag == 1)
	{
		shell_info->line_count++;
		shell_info->linecount_flag = 0;
	}
	for (i = 0, arg_count = 0; shell_info->arg[i]; i++)
		if (!is_delimiter(shell_info->arg[i], " \t\n"))
			arg_count++;
	if (!arg_count)
		return;

	path = find_command_path(shell_info, _getenv(shell_info, "PATH="), shell_info->argv[0]);
	if (path)
	{
		shell_info->path = path;
		fork_shell_command(shell_info);
	}
	else
	{
		if ((is_interactive(shell_info) || _getenv(shell_info, "PATH=")
			|| shell_info->argv[0][0] == '/') && is_shell_command(shell_info, shell_info->argv[0]))
			fork_shell_command(shell_info);
		else if (*(shell_info->arg) != '\n')
		{
			shell_info->status = 127;
			print_shell_error(shell_info, "not found\n");
		}
	}
}

void fork_shell_command(shell_info_t *shell_info)
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
		if (execve(shell_info->path, shell_info->argv, get_shell_environment(shell_info)) == -1)
		{
			free_shell_info(shell_info, 1);
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
				print_shell_error(shell_info, "Permission denied\n");
		}
	}
}
