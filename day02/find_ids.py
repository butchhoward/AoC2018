#!/usr/bin/env python

import sys

def diff_positions(a, b):
    v = []
    for i in range(min(len(a), len(b))):
        if a[i] != b[i]:
            v.append(a[i])

    return v

def common_positions(a, b):
    v = []
    for i in range(min(len(a), len(b))):
        if a[i] == b[i]:
            v.append(a[i])

    return v


#the assumption is that there is exactly one pair of keys that differ by exaclty one char
def find_targets(data):
    for value in data:
        for value2 in data:
            if len(diff_positions(value, value2)) == 1:
                return value, value2
    return [],[]

if len(sys.argv) != 2:
    sys.exit()

data = [list(line.rstrip('\n')) for line in open(sys.argv[1])]

t1, t2 = find_targets(data)
print(''.join(common_positions(t1,t2)))
