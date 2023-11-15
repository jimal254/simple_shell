#include "my_shell.h"

/**
 * str_copy - copies a string
 * @dest: the destination
 * @src: the source
 *
 * Return: pointer to destination
 */
char *str_copy(char *dest, char *src)
{
    int i = 0;

    if (dest == src || src == 0)
        return dest;
    while (src[i])
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return dest;
}

/**
 * str_duplicate - duplicates a string
 * @str: the string to duplicate
 *
 * Return: pointer to the duplicated string
 */
char *str_duplicate(const char *str)
{
    int length = 0;
    char *result;

    if (str == NULL)
        return NULL;
    while (*str++)
        length++;
    result = malloc(sizeof(char) * (length + 1));
    if (!result)
        return NULL;
    for (length++; length--;)
        result[length] = *--str;
    return result;
}

/**
 * my_puts - prints an input string
 * @str: the string to be printed
 *
 * Return: Nothing
 */
void my_puts(char *str)
{
    int i = 0;

    if (!str)
        return;
    while (str[i] != '\0')
    {
        my_putchar(str[i]);
        i++;
    }
}

/**
 * my_putchar - writes the character c to stdout
 * @c: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int my_putchar(char c)
{
    static int i;
    static char buffer[WRITE_BUF_SIZE];

    if (c == BUF_FLUSH || i >= WRITE_BUF_SIZE)
    {
        write(1, buffer, i);
        i = 0;
    }
    if (c != BUF_FLUSH)
        buffer[i++] = c;
    return 1;
}
