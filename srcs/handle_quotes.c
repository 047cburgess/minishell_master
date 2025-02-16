#include <stdio.h>
#define TRUE 1
#define FALSE 0

char	*ft_strchr(char *str, char c)
{
	while (*str)
	{
		if (*str == c)
			return (str);
		str++;
	}
	return (NULL);
}

int	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

int	unclosed_quote_detected(char *input)
{
	while (*input != '\0')
	{
		if (is_quote(*input))
		{
			input = ft_strchr(input + 1, *input);
			if (input == NULL)
				return (TRUE);
		}
		input++;
	}
	return (FALSE);
}

void	TEST(char *input, int expected)
{
	int	result = unclosed_quote_detected(input);
	if (result != expected)
		printf("!! KO: [%s]\n\tResult: %i\n\tExpected: %i\n\n", input, result, expected);
	else
		printf(":) OK: [%s]\n\tResult: %i\n\tExpected: %i\n\n", input, result, expected);
}

// TEST //
int	main(void)
{
	// no unclosed quotes should be found
	TEST("echo hello world", FALSE);
	TEST("echo \"hello\" \"world\"", FALSE);
	TEST("echo \"hello world\"", FALSE);
	TEST("echo helloworld", FALSE);
	TEST("echo \'hello world\'", FALSE);
	TEST("echo 'hello' 'world'", FALSE);
	TEST("echo hello 'world'", FALSE);
	TEST("     echo hello world", FALSE);
	TEST("echo "" hello "" world", FALSE);
	TEST("echo hello'$HOME'world", FALSE);
	TEST("echo hello\"$HOME\"world", FALSE);
	TEST("echo hello \"'$HOME'\" world", FALSE);
	TEST("echo hello '\"$HOME\"'world", FALSE);

	// should detect unclosed quotes
	TEST("echo hello''wo'rld", TRUE);
	TEST("echo \"hello\"world\"", TRUE);
	TEST("echo 'hello world", TRUE);
	TEST("echo '' hello \"world", TRUE);
	TEST("echo \"hello' world'", TRUE);
	TEST("'echo'' helo\"world", TRUE);
}
