#include <stdio.h>

void String_reverse(char *start)
{
	char *end = start;
	char tmp;

	// move the 'end' pointer to the last character
	while (*end) { 
		end++;
	}
	end--;

	while (start < end) {
		tmp = *start;
		*start = *end;
		*end = tmp;
		start++;
		end--;
	}
}

int main()
{
	char str[] = "pihclanoitan";

	String_reverse(str);
	printf("%s\n", str);

	return 0;
}
