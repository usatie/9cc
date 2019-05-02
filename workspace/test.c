#include "9cc.h"

// Unit tests
static int expect(int line, int expected, int actual) {
  if (expected == actual)
    return;
  fprintf(stderr, "%d: %d expected, but got %d\n", line, expected, actual);
  exit(1);
}

void runtest() {
	test_vector();
	test_map();
}

void test_vector() {
  Vector *vec = new_vector();
  expect(__LINE__, 0, vec->len);

  for (int i = 0; i < 100; i++)
    vec_push(vec, (void *)i);

  expect(__LINE__, 100, vec->len);
  expect(__LINE__, 0, (int)vec->data[0]);
  expect(__LINE__, 50, (int)vec->data[50]);
  expect(__LINE__, 99, (int)vec->data[99]);

  printf("OK\n");
}

void test_map() {
	Map *map = new_map();
  expect(__LINE__, 0, (long)map_get(map, "foo"));

	map_put(map, "foo", (void *)2);
  expect(__LINE__, 2, (long)map_get(map, "foo"));

	map_put(map, "bar", (void *)4);
  expect(__LINE__, 4, (long)map_get(map, "bar"));

	map_put(map, "foo", (void *)6);
  expect(__LINE__, 6, (long)map_get(map, "foo"));
}
