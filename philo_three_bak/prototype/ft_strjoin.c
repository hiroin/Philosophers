/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/24 06:40:01 by user42            #+#    #+#             */
/*   Updated: 2020/11/05 02:07:10 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static size_t	ft_strlcat(char *restrict dst,
					const char *restrict src, size_t size)
{
	size_t	count;
	size_t	dest_size;
	size_t	src_size;

	count = 0;
	dest_size = ft_strlen(dst);
	src_size = ft_strlen(src);
	if (dest_size >= size)
		return (src_size + size);
	while (src[count] && (size - 1 > count + dest_size))
	{
		dst[count + dest_size] = src[count];
		count++;
	}
	dst[count + dest_size] = '\0';
	return (dest_size + src_size);
}

static size_t	ft_strlcpy(char *restrict dst,
						const char *restrict src, size_t dstsize)
{
	size_t	count;
	size_t	size;

	count = 0;
	size = 0;
	if (!dst)
		return (0);
	while (src[size])
		size++;
	if (!dstsize)
		return (size);
	while (src[count] && count < dstsize - 1)
	{
		dst[count] = src[count];
		count++;
	}
	dst[count] = '\0';
	return (size);
}

static char		*ft_strcpy(char *dst, const char *src)
{
	unsigned int	i;

	i = 0;
	while (src[i])
	{
		dst[i] = src[i];
		i += 1;
	}
	dst[i] = '\0';
	return (dst);
}

static char		*ft_strdup(const char *s)
{
	char	*str;

	if (!(str = malloc(ft_strlen(s) + 1)))
		return (NULL);
	return (ft_strcpy(str, s));
}

char			*ft_strjoin(char const *s1, char const *s2)
{
	char			*out;
	unsigned int	dstsize;

	if (!s1 && !s2)
		return (ft_strdup(""));
	else if (!s1)
		return (ft_strdup(s2));
	else if (!s2)
		return (ft_strdup(s1));
	else
	{
		dstsize = ft_strlen(s1) + ft_strlen(s2) + 1;
		out = malloc(dstsize * sizeof(char));
		if (!out)
			return (NULL);
		ft_strlcpy(out, s1, dstsize);
		ft_strlcat(out, s2, dstsize);
		return (out);
	}
}
