#!/bin/sh

# Build compiler
echo "[Building TIMAC]"
gcc -g -Iinclude timac.c -o timac

# Build tests:
echo "[Building tests/test_1.ti]"
./timac tests/test_1.ti -o build/test_1.asm
echo "[Building tests/test_2.ti]"
./timac tests/test_2.ti -o build/test_2.asm
