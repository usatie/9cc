#!/bin/bash
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


try 0 '0;'
try 42 '42;'
try 21 '5+20-4;'
try 2 '5 - 3;'
try 41 '12 + 34 - 5;'
try 47 "5+6*7;"
try 15 "5*(9-6);"
try 4 "(3+5)/2;"
try 6 "-4+10;"
try 5 "+5;"
try 8 "-(-4*+2);"
try 5 "-3*+5+20;"
try 1 "20==20;"
try 0 "10==20;"
try 1 "16 != 32;"
try 0 "32 != 32;"
try 1 "1 < 100;"
try 0 "10 < 10;"
try 0 "16 < 1;"
try 1 "3 <= 30;"
try 1 "3 <= 3;"
try 0 "30 <= 1;"
try 0 "1 > 100;"
try 0 "10 > 10;"
try 1 "16 > 1;"
try 0 "3 >= 30;"
try 1 "3 >= 3;"
try 1 "30 >= 1;"
try 60 "1+2+4;10+20+30;"
try 5 "a=2;a+3;"
try 4 "a=b=2;a+b;"
try 4 "return 4;"
try 5 "a=3;return 5;a+10;"
try 10 "foo=3;bar=7;return foo + bar;"
try 100 "foo=1;bar=9;hoge=3;return (foo + bar) * (hoge + 7);"
try 21 "if (1 == 1) return 21;return 64;"
try 64 "if (1 == 2) return 21;return 64;"
try 19 "if (1 == 1) return 19; else 33;"
try 33 "if (1 == 2) return 19; else 33;"
try 13 "if (1 == 2) return 1; if (1 == 1) return 13;"
try 29 "if (1 == 3) return 1; if (2 == 3) return 2; if (3 == 3) return 29;"
try 97 "if (1 == 4) return 1; if (2 == 4) return 2; if (3 == 4) return 29; else return 97;"
try 2 "a=0;a = a + 1;a = a + 1;return a;"
try 10 "a=0;while (a < 10) a = a + 1;return a;"
try 10 "a=0;while (a < 10) a = a + 1;return a;"
try 100 "a=0;for (i=0;i<100;i = i +1;) a = a + 2;return i;"
try 200 "a=0;for (i=0;i<100;i = i +1;) a = a + 2;return a;"
try 6 "if (1 == 1)  { a = 1; b = 2; c = 3; return a+b+c; }"
try 10 "if (1 == 2)  { a = 1; b = 2; c = 3; return a+b+c; } else { return 10; }"
try 55 "a=b=0; while (b < 50) {  a = a + 1; b = b + a; } return b;"
try 55 "a=b=0;for (i=0;i<10;i = i +1;) { a = a + 1; b = a + b; } return b;"

echo OK
