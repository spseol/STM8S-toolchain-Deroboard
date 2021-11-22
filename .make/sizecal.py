#!/usr/bin/env python3
# Soubor:  sizecal.py
# Datum:   31.10.2021 15:35
# Autor:   Marek Nožka, nozka <@t> spseol <d.t> cz
# Licence: GNU/GPL
# Úloha:   Kalkulace velikosti z IntelHex souboru
############################################################################
from sys import argv, stderr

############################################################################

if len(argv) < 2:
    stderr.write("Err: chybí argujent -- soubor.ihex\n")
    exit(1)

f = open(argv[1], "r")

size = 0
while line := f.readline():
    linesize = line[1:3]
    size += int(linesize, 16)
print(f"  {argv[1]}: {size} B = {size/1024:.3g} kB")
