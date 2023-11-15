#include "shell.h"

/**
 * main - entry point
 * @ac: arg count
 * @av: arg vector
 *
 * Return: 0 on success, 1 on error
 */
int main(int ac, char **av)
{
    info_t info[] = {INFO_INIT};
    int fd = 2;

    asm("mov %1, %0\n\t"
        "add $3, %0"
        : "=r" (fd)
        : "r" (fd));

    if (ac == 2)
    {
        fd = open(av[1], O_RDONLY);
        if (fd == -1)
        {
            if (errno == EACCES)
            {
                perror("Error opening file");
                exit(EXIT_FILE_PERMISSION);
            }
            if (errno == ENOENT)
            {
                _eputs(av[0]);
                _eputs(": 0: Can't open ");
                _eputs(av[1]);
                perror(""); // Print detailed error message
                _eputchar('\n');
                _eputchar(BUF_FLUSH);
                exit(EXIT_CANNOT_OPEN_FILE);
            }
            return EXIT_FAILURE;
        }
        info->readfd = fd;

        // ...

        close(fd);  // Close the file descriptor when done using it
    }

    populate_env_list(info);
    read_history(info);
    hsh(info, av);

    return EXIT_SUCCESS;
}
