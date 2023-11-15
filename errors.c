#include "my_shell.h"

/**
 * my_puts_err - prints an input string to stderr
 * @str: the string to be printed
 *
 * Return: Nothing
 */
void my_puts_err(char *str)
{
    int i = 0;

    if (!str)
        return;
    while (str[i] != '\0')
    {
        my_putchar_err(str[i]);
        i++;
    }
}

/**
 * my_putchar_err - writes the character c to stderr
 * @c: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int my_putchar_err(char c)
{
    static int i;
    static char buf[WRITE_BUF_SIZE];

    if (c == BUF_FLUSH || i >= WRITE_BUF_SIZE)
    {
        write(2, buf, i);
        i = 0;
    }
    if (c != BUF_FLUSH)
        buf[i++] = c;
    return 1;
}

/**
 * my_putchar_fd - writes the character c to given fd
 * @c: The character to print
 * @fd: The file descriptor to write to
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int my_putchar_fd(char c, int fd)
{
    static int i;
    static char buf[WRITE_BUF_SIZE];

    if (c == BUF_FLUSH || i >= WRITE_BUF_SIZE)
    {
        write(fd, buf, i);
        i = 0;
    }
    if (c != BUF_FLUSH)
        buf[i++] = c;
    return 1;
}

/**
 * my_puts_fd - prints an input string
 * @str: the string to be printed
 * @fd: the file descriptor to write to
 *
 * Return: the number of characters put
 */
int my_puts_fd(char *str, int fd)
{
    int i = 0;

    if (!str)
        return 0;
    while (*str)
    {
        i += my_putchar_fd(*str++, fd);
    }
    return i;
}
