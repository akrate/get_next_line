#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUFFER_SIZE 5 

int ft_strchr(char *str)
{
    int i = 0;

    if (!str)
        return (-1);
    while (str[i])
    {
        if (str[i] == '\n')
            return (i);
        i++;
    }
    return (-1);
}

int ft_strlen(char *str)
{
    int i = 0;

    while (str[i])
        i++;
    return (i);
}

char *strjoin(char *str1, char *str2)
{
    int i = 0;
    int j = 0;
    char *result;

    if (!str1)
    {
        str1 = (char *)malloc(1 * sizeof(char));
        str1[0] = '\0';
    }
    result = (char *)malloc((ft_strlen(str1) + ft_strlen(str2) + 1) * sizeof(char));
    if (!result)
        return (NULL);

    while (str1[i])
        result[j++] = str1[i++];
    i = 0;
    while (str2[i])
        result[j++] = str2[i++];
    result[j] = '\0';
    free(str1);
    return (result);
}

char *get_left_line(char *str)
{
    int i = 0;
    char *result;

    if (!str)
        return (NULL);
    while (str[i] && str[i] != '\n')
        i++;
    result = (char *)malloc((i + 2) * sizeof(char));
    if (!result)
        return (NULL);
    i = 0;
    while (str[i] && str[i] != '\n')
    {
        result[i] = str[i];
        i++;
    }
    if (str[i] == '\n')
    {
        result[i] = '\n';
        i++;
    }
    result[i] = '\0';
    return (result);
}

char *get_right_line(char *str)
{
    int i = 0;
    int j = 0;
    char *result;

    if (!str)
        return (NULL);
    while (str[i] && str[i] != '\n')
        i++;
    if (str[i] == '\0')  // If there is no newline
    {
        free(str);
        return (NULL);  // No more content to read
    }
    i++;  // Skip the newline
    result = (char *)malloc((ft_strlen(str) - i + 1) * sizeof(char));
    if (!result)
        return (NULL);
    while (str[i])
        result[j++] = str[i++];
    result[j] = '\0';
    free(str);
    return (result);
}

char *get_next_line(int fd)
{
    int read_size;
    char *str_read;
    static char *remainder;

    if (fd < 0)
        return (NULL);
    str_read = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
    if (!str_read)
        return (NULL);
    while ((read_size = read(fd, str_read, BUFFER_SIZE)) > 0)
    {
        str_read[read_size] = '\0';
        remainder = strjoin(remainder, str_read);
        if (ft_strchr(remainder) != -1)
            break;
    }
    free(str_read);
    if (!remainder || read_size == 0)
    {
        if (remainder && ft_strlen(remainder) > 0)
        {
            str_read = get_left_line(remainder);
            remainder = get_right_line(remainder);
            return (str_read);
        }
        return (NULL);
    }
    str_read = get_left_line(remainder);
    remainder = get_right_line(remainder);
    return (str_read);
}


int main()
{
    int fd = open("test.txt", O_RDONLY);
    if (fd == -1)
    {
        perror("Error opening file");
        return (1);
    }

    char *str;
    while ((str = get_next_line(fd)) != NULL)
    {
        printf("%s", str);
        free(str);  // Free the memory after use
    }

    close(fd);
    return (0);
}
