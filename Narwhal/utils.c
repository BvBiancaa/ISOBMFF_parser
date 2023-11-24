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