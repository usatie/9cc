#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Enum represents token type
enum {
	TK_NUM = 256, // integer
	TK_EOF,				// End of input
};

typedef struct {
	int ty;				// Token type
	int val;			// The value if ty is TK_NUM
	char *input;	// Token character (for error message)
} Token;

Token tokens[100];

void error(char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	exit(1);
}

void tokenize(char *p) {
	int i = 0;
	while (*p) {
		if (isspace(*p)) {
			p++;
			continue;
		}

		if (*p == '+' || *p == '-') {
			tokens[i].ty = *p;
			tokens[i].input = p;
			i++;
			p++;
			continue;
		}

		if (isdigit(*p)) {
			tokens[i].ty = TK_NUM;
			tokens[i].input = p;
			tokens[i].val = strtol(p, &p, 10);
			i++;
			continue;
		}

		error("Cannot tokenize: %s", p);
		exit(1);
	}

	tokens[i].ty = TK_EOF;
	tokens[i].input = p;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "The number of argments is invalid.");
    return 1;
  }

	// Tokenize
	tokenize(argv[1]);

	// Print assembly's first half
  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");

	// Check if the first character is a number. It should be so.
	if (tokens[0].ty != TK_NUM)
		error("The first item is not number");
  printf("	mov rax, %d\n", tokens[0].val);
	
	// Consume tokens like `+ <num>` or `- <num>`
	// And then print assembly
	int i = 1;
	while (tokens[i].ty != TK_EOF) {
		if (tokens[i].ty == '+') {
			i++;
			if (tokens[i].ty != TK_NUM)
				error("Unexpected token: %s", tokens[i].input);

			printf("	add rax, %d\n", tokens[i].val);
			i++;
			continue;
		}

		if (tokens[i].ty == '-') {
			i++;
			if (tokens[i].ty != TK_NUM)
				error("Unexpected token: %s", tokens[i].input);

			printf("	sub rax, %d\n", tokens[i].val);
			i++;
			continue;
		}

		error("Unexpected character: '%c'\n", tokens[i].input);
	}

  printf("	ret\n");
  return 0;
}
