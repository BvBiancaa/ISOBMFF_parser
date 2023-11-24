#include "includes/challenge.h"

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

void	ft_putstr(char *s)
{
	int	index;

	index = -1;
	while (s[++index])
		write(1, &s[index], 1);
}