/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arthur <arthur@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/28 21:34:38 by atinseau          #+#    #+#             */
/*   Updated: 2021/07/29 15:02:43 by arthur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atinseau <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/29 14:54:42 by atinseau          #+#    #+#             */
/*   Updated: 2020/11/29 14:54:47 by atinseau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char *substr(char *str, int start, int len, int is_free)
{
	char *new;
	int i;

	i = 0;
	if (!(new = malloc(sizeof(char) * (len + 1))))
		return (NULL);
	if (str == NULL)
	{
		*new = '\0';
		return (new);
	}
	while (str[start] && i < len)
	{
		new[i] = str[start];
		i++;
		start++;
	}
	new[i] = '\0';
	if (is_free == 1)
		free(str);
	return (new);
}

int ft_strlen(char *str)
{
	int i;

	if (!str)
		return (0);
	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

int last_line(char **line, char *str)
{
	*line = substr(str, 0, ft_strlen(str), 0);
	if (str)
		*str = '\0';
	return (0);
}

int pos_n(char *str)
{
	int i;

	i = 0;
	while (str[i] != '\n' && str[i])
		i++;
	return (i);
}

char *ft_strjoin(char *s1, char *s2)
{
	char *new;
	int e;
	int i;

	if (!s1 && !s2)
		return (NULL);
	if (!(new = (char *)malloc(sizeof(char) *
								   (ft_strlen(s1) + ft_strlen(s2)) +
							   1)))
		return (NULL);
	e = 0;
	i = 0;
	while (s1 && s1[i] != '\0')
		new[e++] = s1[i++];
	i = 0;
	while (s2[i] != '\0')
		new[e++] = s2[i++];
	new[e] = '\0';
	free(s1);
	return (new);
}

int has_return(char *str)
{
	int i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

int get_next_line(char **line)
{
	int ret;
	static char *str;
	char *temp;

	if (!line || BUFFER_SIZE <= 0 ||
		!(temp = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1))))
		return (-1);
	while (has_return(str) == 0 && (ret = read(0, temp, BUFFER_SIZE)))
	{
		if (ret == -1)
		{
			free(temp);
			return (-1);
		}
		temp[ret] = '\0';
		str = ft_strjoin(str, temp);
	}
	free(temp);
	if (!has_return(str))
		return (last_line(line, str));
	*line = substr(str, 0, pos_n(str), 0);
	str = substr(str, pos_n(str) + 1, ft_strlen(str) - pos_n(str), 1);
	return (1);
}
