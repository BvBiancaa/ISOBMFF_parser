#include "includes/challenge.h"

// I had fun reimplementing some functions, I had never used a FILE* variable and since I liked this project a lot I will probably try reimplementing that as well

char	*ft_scanf(void)
{
	int	index;
	int	c;
	char	*ret;

	index = -1;
	ret = NULL;
	while (++index >= 0)
	{
		ret = (char *)realloc(ret, (index + 1) * sizeof(char));
		if (ret == NULL || !ret)
			return (NULL);
		c = getc(stdin);
		if (c == '\n' || c == EOF)
			break;
		else
			ret[index] = c;
	}
	ret[index] = '\0';
	return (ret);
}

//scanf reimplementation
//the function loops (i could have written while (1) but i saved a line putting ++index there, I come from 42 so saving a line is A LOT).
//basically the function waits to take as input a char written on the stdin (fd = 0) and when it gets a newline as input is stops, ends the line with the \0 and returns the string
//it reallocs every char so better get your argv right. 

void	ft_putstr(char *s)
{
	int	index;

	index = -1;
	while (s[++index])
		write(1, &s[index], 1);
}

//little putstr

int	ft_strcmp(char *s1, char *s2)
{
	unsigned int	i;

	i = 0;
	while (s1[i] == s2[i] && s1[i])
		i++;
	return (s1[i] - s2[i]);
}

//strcmp reimplementation, not really protected but enough for its scope in the code
//the function iterates the two strings, when it finds they have at i position two different characters it returns the difference between the two.

int	ft_strstr(const char *haystack, const char *needle)
{
	size_t	i;
	size_t	j;


	if (!needle || needle == NULL || !haystack || haystack == NULL)
		return (-1);
	i = -1;
	j = 0;
	while (haystack[++i])
	{
		if (haystack[i] == needle[j])
		{
			while (haystack[i + j] == needle[j])
			{
				if (needle[j + 1] == '\0')
					return (i + j);
				j++;
			}
			j = 0;
		}
	}
	return (-1);
}

//a modified version of strstr, it returns the position of the last character of the string the function has to find. 

int	find_str(const char *s, const char *s1)
{
	int i = 0;
	for (; s[i] && s1[i] && s[i] == s1[i]; i++)
		;
	if (s[i] == '\0' && s1[i] == '=')
		return (i + 1);
	return (-1);
}

//finds where the value of the requested env is.

int	ft_getenv(const char *s, char **env)
{
	char	*ret;
	ssize_t	index;
	
	index = 0;
	ret = NULL;
	for (ssize_t i = 0; env[i]; i++)
	{
		index = find_str(s, env[i]);
		if (index > 0 && (size_t)index <= strlen(env[i]))
		{
			ret = env[i] + index;
			index = i;
			break ;
		}
	}
	return (ft_atoi(ret));
}

//getenv reimplementation, the function takes the string and the env variable,
//it loops the env variable until it finds the string, it then returns the number after the string.

ssize_t	ft_atoi(char const *str)
{
	int	i;
	int	sign;
	ssize_t	res;

	i = 0;
	sign = 1;
	res = 0;
	if (str == NULL)
		return (0);
	while ((str[i] <= 13 && str[i] >= 9) || str[i] == 32)
		i++;
	if (str[i] == 43 || str[i] == 45)
	{
		if (str[i] == 45)
			sign *= -1;
		i++;
	}
	while (str[i] >= 48 && str[i] <= 57)
	{
		res = res * 10 + (str[i] - 48);
		i++;
	}
	return (res * sign);
}

//atoi reimplementation, the function takes a string, removes the leading white spaces, check if there's a sign
//if a sign is found and it's a minus the sign becomes -1, else it just goes on.
//it iterates the string until it containes 0 - 9 ascii characters taking the number, multiplying it for 10
//(it's "shifting" the number position) and adding the number it just processed turining it in a number.
//the function then returns the number * sign.
