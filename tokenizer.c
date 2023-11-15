#include "shell.h"

/**
 * split_string - splits a string into words based on delimiters
 * @str: the input string
 * @delimiters: the delimiters
 * Return: a pointer to an array of strings, or NULL on failure
 */
char **split_string(char *str, char *delimiters)
{
	int i, j, k, m, num_words = 0;
	char **words;

	if (str == NULL || str[0] == '\0')
		return NULL;

	if (!delimiters)
		delimiters = " ";

	for (i = 0; str[i] != '\0'; i++) {
		if (!is_delim(str[i], delimiters) &&
		    (is_delim(str[i + 1], delimiters) || !str[i + 1]))
			num_words++;
	}

	if (num_words == 0)
		return NULL;

	words = malloc((1 + num_words) * sizeof(char *));
	if (!words)
		return NULL;

	for (i = 0, j = 0; j < num_words; j++) {
		while (is_delim(str[i], delimiters))
			i++;

		k = 0;
		while (!is_delim(str[i + k], delimiters) && str[i + k])
			k++;

		words[j] = malloc((k + 1) * sizeof(char));
		if (!words[j]) {
			free_string_array(words, j);
			return NULL;
		}

		for (m = 0; m < k; m++)
			words[j][m] = str[i++];

		words[j][m] = '\0';
	}

	words[j] = NULL;
	return words;
}

/**
 * split_string2 - splits a string into words based on a single delimiter
 * @str: the input string
 * @delimiter: the delimiter
 * Return: a pointer to an array of strings, or NULL on failure
 */
char **split_string2(char *str, char delimiter)
{
	int i, j, k, m, num_words = 0;
	char **words;

	if (str == NULL || str[0] == '\0')
		return NULL;

	for (i = 0; str[i] != '\0'; i++) {
		if ((str[i] != delimiter && str[i + 1] == delimiter) ||
		    (str[i] != delimiter && !str[i + 1]) || str[i + 1] == delimiter)
			num_words++;
	}

	if (num_words == 0)
		return NULL;

	words = malloc((1 + num_words) * sizeof(char *));
	if (!words)
		return NULL;

	for (i = 0, j = 0; j < num_words; j++) {
		while (str[i] == delimiter && str[i] != delimiter)
			i++;

		k = 0;
		while (str[i + k] != delimiter && str[i + k] && str[i + k] != delimiter)
			k++;

		words[j] = malloc((k + 1) * sizeof(char));
		if (!words[j]) {
			free_string_array(words, j);
			return NULL;
		}

		for (m = 0; m < k; m++)
			words[j][m] = str[i++];

		words[j][m] = '\0';
	}

	words[j] = NULL;
	return words;
}

/**
 * free_string_array - frees memory allocated for an array of strings
 * @arr: the array of strings
 * @size: the size of the array
 */
void free_string_array(char **arr, int size)
{
	int i;
	if (arr) {
		for (i = 0; i < size; i++)
			free(arr[i]);
		free(arr);
	}
}
