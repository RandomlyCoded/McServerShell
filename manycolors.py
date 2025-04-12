#!/usr/bin/python3

import sys
import time

for bg in range(8):
    for fg in range(8):
        print(f"\033[3{fg};4{bg}mHello!\033[0m {(fg * 10) + bg + 1:2} ", end="")

    print()
    sys.stdout.flush()
    time.sleep(.5)

