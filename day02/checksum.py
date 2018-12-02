#!/usr/bin/env python

import sys


def count_letters(value):
    accumulator={}
    for letter in value:
        if letter not in accumulator:
            accumulator[letter] = 0

        accumulator[letter] = accumulator[letter] + 1
    return accumulator

def twos_and_threes_inc(accumulator):
    twos=0
    threes=0
    for letter, count in accumulator.items():
        if count==2:
            twos=1
        if count==3:
            threes=1
            
    return twos, threes

def hash_data(data):
    twos=0
    threes=0
    for value in data:
        accumulator = count_letters(value)
        twos_inc, threes_inc = twos_and_threes_inc(accumulator)
        twos += twos_inc
        threes += threes_inc
    
    return twos * threes


if len(sys.argv) != 2:
    sys.exit()

data = [line.rstrip('\n') for line in open(sys.argv[1])]

print(hash_data(data))
