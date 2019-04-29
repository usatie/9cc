#include "cc.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "The number of argments is invalid.");
    return 1;
  }

	if (!strcmp(argv[1], "-test")) {
		runtest();
		return 0;
	}

	// Tokenize and parse
  Node *node = parse(argv[1]);

	// Print assembly's first half
  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");

  // Generate code with going down abstract tree
  gen(node);

  // The result should be at the top of stack
  // Load it to RAX as a return value.
  printf("	pop rax\n");
  printf("	ret\n");
  return 0;
}
