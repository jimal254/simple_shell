#include "my_shell.h"

/**
 * is_command_chain_delimiter - test if current char in buffer is a command chain delimiter
 * @info: the parameter struct
 * @buffer: the char buffer
 * @position: address of current position in buffer
 *
 * Return: 1 if chain delimiter, 0 otherwise
 */
int is_command_chain_delimiter(info_t *info, char *buffer, size_t *position)
{
	size_t j = *position;

	if (buffer[j] == '|' && buffer[j + 1] == '|')
	{
		buffer[j] = 0;
		j++;
		info->command_buffer_type = COMMAND_OR;
	}
	else if (buffer[j] == '&' && buffer[j + 1] == '&')
	{
		buffer[j] = 0;
		j++;
		info->command_buffer_type = COMMAND_AND;
	}
	else if (buffer[j] == ';') /* found end of this command */
	{
		buffer[j] = 0; /* replace semicolon with null */
		info->command_buffer_type = COMMAND_CHAIN;
	}
	else
		return (0);
	*position = j;
	return (1);
}

/**
 * check_command_chain - checks if we should continue chaining based on last status
 * @info: the parameter struct
 * @buffer: the char buffer
 * @position: address of current position in buffer
 * @start_position: starting position in buffer
 * @length: length of buffer
 *
 * Return: Void
 */
void check_command_chain(info_t *info, char *buffer, size_t *position, size_t start_position, size_t length)
{
	size_t j = *position;

	if (info->command_buffer_type == COMMAND_AND)
	{
		if (info->status)
		{
			buffer[start_position] = 0;
			j = length;
		}
	}
	if (info->command_buffer_type == COMMAND_OR)
	{
		if (!info->status)
		{
			buffer[start_position] = 0;
			j = length;
		}
	}

	*position = j;
}

/**
 * replace_command_alias - replaces an aliases in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_command_alias(info_t *info)
{
	int i;
	list_t *node;
	char *p;

	for (i = 0; i < 10; i++)
	{
		node = node_starts_with(info->alias, info->command_argv[0], '=');
		if (!node)
			return (0);
		free(info->command_argv[0]);
		p = _strchr(node->str, '=');
		if (!p)
			return (0);
		p = str_duplicate(p + 1);
		if (!p)
			return (0);
		info->command_argv[0] = p;
	}
	return (1);
}

/**
 * replace_command_vars - replaces vars in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_command_vars(info_t *info)
{
	int i = 0;
	list_t *node;

	for (i = 0; info->command_argv[i]; i++)
	{
		if (info->command_argv[i][0] != '$' || !info->command_argv[i][1])
			continue;

		if (!_strcmp(info->command_argv[i], "$?"))
		{
			replace_string(&(info->command_argv[i]),
				str_duplicate(convert_number(info->status, 10, 0)));
			continue;
		}
		if (!_strcmp(info->command_argv[i], "$$"))
		{
			replace_string(&(info->command_argv[i]),
				str_duplicate(convert_number(getpid(), 10, 0)));
			continue;
		}
		node = node_starts_with(info->env, &info->command_argv[i][1], '=');
		if (node)
		{
			replace_string(&(info->command_argv[i]),
				str_duplicate(_strchr(node->str, '=') + 1));
			continue;
		}
		replace_string(&info->command_argv[i], str_duplicate(""));

	}
	return (0);
}

/**
 * replace_string - replaces string
 * @old: address of old string
 * @new: new string
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_string(char **old, char *new)
{
	free(*old);
	*old = new;
	return (1);
}
