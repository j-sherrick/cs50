import subprocess
import os
import time

f_random = sorted([x.name for x in os.scandir() if x.name.startswith('random')], key=lambda x: os.stat(x).st_size)
f_reversed = sorted([x.name for x in os.scandir() if x.name.startswith('reversed')], key=lambda x: os.stat(x).st_size)
f_sorted = sorted([x.name for x in os.scandir() if x.name.startswith('sorted')], key=lambda x: os.stat(x).st_size)

algos = ['./sort1', './sort2', './sort3']

for algo in algos:
    for rando in f_random:
        now = time.time()
        subprocess.run([algo, rando], capture_output=True)
        elapsed = time.time() - now
        print(algo, ': ', rando, ': runs in ', elapsed)
    for rev in f_reversed:
        now = time.time()
        subprocess.run([algo, rev], capture_output=True)
        elapsed = time.time() - now
        print(algo, ': ', rev, ': runs in ', elapsed)
    for sorted in f_sorted:
        now = time.time()
        subprocess.run([algo, sorted], capture_output=True)
        elapsed = time.time() - now
        print(algo, ': ', sorted, ': runs in ', elapsed)
    print()
    