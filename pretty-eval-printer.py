import re, sys

raw = sys.stdin.read()
ints = iter(re.findall("-?[0-9]+", raw))
for _ in range(8):
    print("   ", end="")
    for _ in range(8):
        mg = next(ints)
        eg = next(ints)
        print(f" S({mg:>4}, {eg:>4}),", end="")
    print()
