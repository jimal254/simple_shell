#include "my_shell.h"

/**
 * is_interactive - returns true if shell is in interactive mode
 * @shell_info: struct address
 *
 * Return: 1 if interactive mode, 0 otherwise
 */
int is_interactive(shell_info_t *shell_info)
{
    return (isatty(STDIN_FILENO) && shell_info->readfd <= 2);
}

/**
 * is_delimiter - checks if character is a delimiter
 * @c: the char to check
 * @delim: the delimiter string
 * Return: 1 if true, 0 if false
 */
int is_delimiter(char c, char *delim)
{
    while (*delim)
        if (*delim++ == c)
            return (1);
    return (0);
}

/**
 * is_alpha - checks for alphabetic character
 * @c: The character to input
 * Return: 1 if c is alphabetic, 0 otherwise
 */
int is_alpha(int c)
{
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

/**
 * string_to_int - converts a string to an integer
 * @s: the string to be converted
 * Return: 0 if no numbers in string, converted number otherwise
 */
int string_to_int(char *s)
{
    int i, sign = 1, flag = 0, result;
    unsigned int output = 0;

    for (i = 0; s[i] != '\0' && flag != 2; i++)
    {
        if (s[i] == '-')
            sign *= -1;

        if (s[i] >= '0' && s[i] <= '9')
        {
            flag = 1;
            output *= 10;
            output += (s[i] - '0');
        }
        else if (flag == 1)
            flag = 2;
    }

    result = (sign == -1) ? -output : output;
    return result;
}
