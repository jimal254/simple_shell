#include "shell.h"
#include <ctype.h>  // Include for isspace and isalpha

// ... (Other functions remain unchanged)

int is_delim(char c, const char *delim) {
    while (*delim) {
        if (*delim++ == c)
            return 1;
    }
    return 0;
}

int _isalpha(int c) {
    return isalpha(c);
}

int _atoi(char *s) {
    int i, sign = 1, flag = 0, output;
    unsigned int result = 0;

    for (i = 0; s[i] != '\0' && flag != 2; i++) {
        if (s[i] == '-')
            sign *= -1;

        if (isdigit(s[i])) {
            flag = 1;
            result *= 10;
            result += (s[i] - '0');
        } else if (flag == 1) {
            flag = 2;
        }
    }

    if (sign == -1)
        output = -result;
    else
        output = result;

    return output;
}
