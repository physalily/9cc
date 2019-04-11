#!/bin/bash

try(){
	expected="$1"
	input="$2"

	./9cc "$input" >tmp.s
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

try 20 "4 * 5"
try 10 "10 / 1"
try 60 "5 *(4 + 20) / 2"
#try 10 " 5 + 2 + 3 + 5"
try 1 "1*1*1*1*1*1*1*1*1"
try 47 "5+6*7"
try 15 "5*(9-6)"
try 8 "4 + ( 3 + 5 ) / 2"
echo OK
