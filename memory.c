#include "shell.h"
/**
 * bfree - frees a pointer and NULLs the address
 * @ptr: address of the pointer to free
 *
 * Return: 1 if the pointer was freed, 0 if the pointer was already NULL or if
 * the input pointer is NULL.
 */
int bfree(void **ptr)
{
    if (ptr && *ptr)
    {
        free(*ptr);
        *ptr = NULL;
        return 1;
    }
    return 0;
}
