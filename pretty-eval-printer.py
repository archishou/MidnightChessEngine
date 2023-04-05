# Credit to Analog Hors
import re, sys

tables = ""
for line in sys.stdin:
    if line.startswith("#"):
        print(line, end="")
    else:
        tables += line

names = iter(re.findall("\w+(?=\[\])", tables))
ints = iter(re.findall("-?[0-9]+", tables))
for name in names:
    print(f"constexpr Score {name}[] = {{")
    for _ in range(8):
        print("   ", end="")
        for _ in range(8):
            mg = next(ints)
            eg = next(ints)
            print(f" S({mg:>4}, {eg:>4}),", end="")
        print()
    read_table = False
    print("};")