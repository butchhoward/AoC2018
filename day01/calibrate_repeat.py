#!/usr/bin/env python

#bash was very slow, so knocked this out to get the answer sooner

import sys

if len(sys.argv) != 2:
    sys.exit()

data = [int(line.rstrip('\n')) for line in open(sys.argv[1])]

frequency = 0
frequencies = [0]
rounds = 0

while True:
    rounds+=1
    for value in data:
        frequency += value
        if frequency in frequencies:
            print("frequency: {} rounds: {}".format(frequency, rounds))
            sys.exit()

        frequencies.append(frequency)





