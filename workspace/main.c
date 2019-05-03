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

  // Generate code with going down abstract tree
  while (*node) {
    gen(*node);
    node++;
  }

  return 0;
}
