#!/bin/bash
gcc -c foo.c
./9cc "main() { a=1;b=2;c=3;foo(a, a + b, a + b + c, 11, 22, 33, 44, 55, 66); return 1; }" > tmp.s
gcc -o tmp tmp.s foo.o
./tmp
try() {
  expected="$1"
  input="$2"

  ./9cc "$input" > tmp.s
  gcc -o tmp tmp.s
  ./tmp
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual."
    exit 1
  fi
}


try 0 'main() { return 0; }'
try 42 'main() { return 42; }'
try 21 'main() { return 5+20-4; }'
try 2 'main() { return 5 - 3; }'
try 41 'main() { return 12 + 34 - 5; }'
try 47 "main() { return 5+6*7; }"
try 15 "main() { return 5*(9-6); }"
try 4 "main() { return (3+5)/2; }"
try 6 "main() { return -4+10; }"
try 5 "main() { return +5; }"
try 8 "main() { return -(-4*+2); }"
try 5 "main() { return -3*+5+20; }"
try 1 "main() { return 20==20; }"
try 0 "main() { return 10==20; }"
try 1 "main() { return 16 != 32; }"
try 0 "main() { return 32 != 32; }"
try 1 "main() { return 1 < 100; }"
try 0 "main() { return 10 < 10; }"
try 0 "main() { return 16 < 1; }"
try 1 "main() { return 3 <= 30; }"
try 1 "main() { return 3 <= 3; }"
try 0 "main() { return 30 <= 1; }"
try 0 "main() { return 1 > 100; }"
try 0 "main() { return 10 > 10; }"
try 1 "main() { return 16 > 1; }"
try 0 "main() { return 3 >= 30; }"
try 1 "main() { return 3 >= 3; }"
try 1 "main() { return 30 >= 1; }"
try 60 "main() { 1+2+4;return 10+20+30; }"
try 5 "main() { a=2;return a+3; }"
try 4 "main() { a=b=2;return a+b; }"
try 4 "main() { return 4; }"
try 5 "main() { a=3;return 5;a+10; }"
try 10 "main() { foo=3;bar=7;return foo + bar; }"
try 100 "main() { foo=1;bar=9;hoge=3;return (foo + bar) * (hoge + 7); }"
try 21 "main() { if (1 == 1) return 21;return 64; }"
try 64 "main() { if (1 == 2) return 21;return 64; }"
try 19 "main() { if (1 == 1) return 19; else return 33; }"
try 33 "main() { if (1 == 2) return 19; else return 33; }"
try 13 "main() { if (1 == 2) return 1; if (1 == 1) return 13; }"
try 29 "main() { if (1 == 3) return 1; if (2 == 3) return 2; if (3 == 3) return 29; }"
try 97 "main() { if (1 == 4) return 1; if (2 == 4) return 2; if (3 == 4) return 29; else return 97; }"
try 2 "main() { a=0;a = a + 1;a = a + 1;return a; }"
try 10 "main() { a=0;while (a < 10) a = a + 1;return a; }"
try 10 "main() { a=0;while (a < 10) a = a + 1;return a; }"
try 100 "main() { a=0;for (i=0;i<100;i = i +1;) a = a + 2;return i; }"
try 200 "main() { a=0;for (i=0;i<100;i = i +1;) a = a + 2;return a; }"
try 6 "main() { if (1 == 1)  { a = 1; b = 2; c = 3; return a+b+c; } }"
try 10 "main() { if (1 == 2)  { a = 1; b = 2; c = 3; return a+b+c; } else { return 10; } }"
try 55 "main() { a=b=0; while (b < 50) {  a = a + 1; b = b + a; } return b; }"
try 55 "main() { a=b=0;for (i=0;i<10;i = i +1;) { a = a + 1; b = a + b; } return b; }"
try 10 "somefunc() { return 10; } main() { return somefunc(); }"
try 33 "add(x, y) { return x+y; } main() { return add(11, 22); }"
try 110 "add(x, y) { return x+y; } main() { return add(11, 22) + add(33, 44); }"
try 15 "add(x, y) { return x+y; } main() { return add(add(1, 2), add(5, 7)); }"
try 144 "fib(n) { if (n==0) return 0; if (n == 1) return 1; return fib(n-1) + fib(n-2); } main() { return fib(12); }"

echo OK
