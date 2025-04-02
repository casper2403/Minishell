/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstevens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 15:00:19 by cstevens          #+#    #+#             */
/*   Updated: 2024/11/28 15:00:21 by cstevens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "get_next_line.h"
#include <stdio.h>

int		read_file(char **saved, int fd);
char	*gettheline(char *saved);
char	*save_remainder(char *saved);

char	*get_next_line(int fd)
{
	int			bytes_read;
	char		*line;
	static char	*saved[1024];

	if (fd < 0 || fd > 1023 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
	{
		if (saved[fd])
			free(saved[fd]);
		saved[fd] = NULL;
		return (NULL);
	}
	bytes_read = 1;
	while (bytes_read > 0 && !ft_isinstr(saved[fd]))
	{
		bytes_read = read_file(&saved[fd], fd);
	}
	if (bytes_read <= 0 && (!saved[fd] || !saved[fd][0]))
	{
		free(saved[fd]);
		saved[fd] = NULL;
		return (NULL);
	}
	line = gettheline(saved[fd]);
	saved[fd] = save_remainder(saved[fd]);
	return (line);
}

int	read_file(char **saved, int fd)
{
	int		bytes_read;
	char	*buffer;
	char	*temp;

	if (!*saved)
		*saved = ft_gnl_strdup("");
	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (-1);
	bytes_read = read(fd, buffer, BUFFER_SIZE);
	if (bytes_read > 0)
	{
		buffer[bytes_read] = '\0';
		temp = *saved;
		*saved = ft_gnl_strjoin(temp, buffer);
	}
	else
		free(buffer);
	return (bytes_read);
}

char	*gettheline(char *saved)
{
	int		i;
	char	*line;

	i = 0;
	if (!saved)
		return (NULL);
	while (saved[i] != '\n' && saved[i])
		i++;
	line = malloc(i + 2);
	if (!line)
		return (NULL);
	ft_strlcpy(line, saved, i + 2);
	return (line);
}

char	*save_remainder(char *saved)
{
	int		i;
	char	*remainder;

	if (!saved)
		return (NULL);
	i = 0;
	while (saved[i] != '\n' && saved[i])
		i++;
	if (!saved[i])
	{
		free(saved);
		return (NULL);
	}
	i++;
	remainder = ft_gnl_strdup(saved + i);
	if (!remainder)
		free(saved);
	if (!remainder)
		return (NULL);
	free(saved);
	return (remainder);
}
/*
#include <stdio.h>
#include <fcntl.h>
#include "get_next_line.h"

int main(void)
{
    int     fd;
    char    *line;

    fd = open("read_error.txt", O_RDONLY);
    if (fd == -1)
    {
        printf("Error opening file.\n");
        return (1);
    }
    line = get_next_line(fd);
    while (line != NULL)
    {
        printf("%s", line);
        free(line);
        line = get_next_line(fd);
    }
    close(fd);
    printf("\nEnter some text (press Ctrl+D to end):\n");
    line = get_next_line(STDIN_FILENO);
    while (line != NULL)
    {
        printf("%s", line);
        free(line);
        line = get_next_line(STDIN_FILENO);
    }
    return (0);
}*/
