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
    echo "$expected expected, but got $actual"
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

echo OK
