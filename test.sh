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
try 30 "5 * 4 + 20 / 2"
echo OK
