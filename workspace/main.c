#include "9cc.h"

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
  // **node is a pointer to Node's pointer
  // i.e. pointer to the array of Node's pointer
  Node **node = parse(argv[1]);

  // Print assembly's first half
  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");

  // Prologue
  // Layout memory for 26 variables
  printf("	push rbp\n");
  printf("	mov rbp, rsp\n");
  printf("	sub rsp, 208\n");

  // Generate code with going down abstract tree
  while (*node) {
    gen(*node);
    node++;

    // One value should be on the top stack.
    printf("	pop rax\n");
  }

  // Epilogue
  // Free memory for variables
  printf("	mov rsp, rbp\n");
  printf("	pop rbp\n");

  // The result should be at the top of stack
  // Load it to RAX as a return value.
  printf("	ret\n");
  return 0;
}
