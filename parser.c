#include "shell.h"

/**
 * is_cmd - determines if a file is an executable command
 * @info: the info struct
 * @path: path to the file
 * Return: 1 if true, 0 otherwise
 */
int is_cmd(info_t *info, char *path)
{
    struct stat file_stat;

    (void)info;

    if (!path || stat(path, &file_stat))
        return 0;

    return (file_stat.st_mode & S_IFREG) ? 1 : 0;
}

/**
 * dup_chars - duplicates characters
 * @path_str: the PATH string
 * @start: starting index
 * @stop: stopping index
 * Return: pointer to new buffer
 */
char *dup_chars(char *path_str, int start, int stop)
{
    static char buffer[1024];
    int i = 0, k = 0;

    for (k = 0, i = start; i < stop; i++)
        if (path_str[i] != ':')
            buffer[k++] = path_str[i];

    buffer[k] = '\0';
    return buffer;
}

/**
 * find_path - finds this cmd in the PATH string
 * @info: the info struct
 * @path_str: the PATH string
 * @cmd: the cmd to find
 * Return: full path of cmd if found or NULL
 */
char *find_path(info_t *info, char *path_str, char *cmd)
{
    int i = 0, curr_pos = 0;
    char *full_path;

    if (!path_str)
        return NULL;

    if ((_strlen(cmd) > 2) && starts_with(cmd, "./"))
    {
        if (is_cmd(info, cmd))
            return cmd;
    }

    while (1)
    {
        if (!path_str[i] || path_str[i] == ':')
        {
            full_path = dup_chars(path_str, curr_pos, i);

            if (!*full_path)
                _strcat(full_path, cmd);
            else
            {
                _strcat(full_path, "/");
                _strcat(full_path, cmd);
            }

            if (is_cmd(info, full_path))
                return full_path;

            if (!path_str[i])
                break;

            curr_pos = i;
        }

        i++;
    }

    return NULL;
}
