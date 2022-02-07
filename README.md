STM8 startovací toolchain
==============================

* Toto je Startovací strom zdrojových kódů pro výuku mikroprocesorové techniky s kitem
  [NUCLEO-8S208RB](https://www.st.com/en/evaluation-tools/nucleo-8s208rb.html).
* Strom je určen pro překladač [SDCC](http://sdcc.sourceforge.net/) nebo 
  [SDCC-gas](https://github.com/XaviDCR92/sdcc-gas).
* Standardní knihovnu pro práci s periferiemi 
  [SPL](https://www.st.com/content/st_com/en/products/embedded-software/mcu-mpu-embedded-software/stm8-embedded-software/stsw-stm8069.html)
  je třeba (z licenčních důvodů) stáhnou zvlášť ze stránek výrobce a použít
  [patch](https://github.com/gicking/STM8-SPL_SDCC_patch).
* Konkurence a inspirace: <https://gitlab.com/wykys/stm8-tools>

Tři Makefile
---------------

K dispozici jsou celkem tři Makefile v adresáři `.make`. Na začátku si musíte
jeden z nich vybrat:

```bash
make sdcc       # nebo
make sdccrm     # nebo
make sdcc-gas   # nebo
```

Potom můžete mezi nimi přepínat:

    make switch-sdcc
    make switch-sdccrm
    make switch-sdcc-gas

Přepnutí jen znamená, že se udělá symlink do root-adresáře projektu. Na divných
systémech, které symlinky neumí (například Windows) se natvrdo kopíruje, takže
tato operace může být ztrátová. Na normálních systémech (asi všechny, kromě
Windows) je tato operace bezztrátová.


### Který Makefile vybrat?

Detailní popis najdete na <https://chytrosti.marrek.cz/stm8oss.html>.

#### SDCC

Z hlediska STM8 má [SDCC](http://sdcc.sourceforge.net/) jednu zásadní nevýhodu:
nedokáže odstranit mrtvý nepoužívaný kód. To může zapříčinit velké binární soubory plné
nepoužívaného kódu. Pokud nepoužíváte knihovny 2. a 3. stran asi vám to nevadí.

#### sdccrm

[sdccrm](https://github.com/XaviDCR92/sdccrm) je nástroj pro optimalizaci
mrtvého kódu vytvořeného SDCC, který odstraňuje nepoužívané funkce. Kód se
nejprve zkompiluje do assembleru klasickým SDCC, poté se pomocí sdccrm vymaže
kód, který se nepoužívá a celý proces se dokončí a kód se převede z assembleru
do strojového kódu. Z logiky věci toto řešení vylučuje použití debugeru.

Dále je nutné ručně zadat funkce, které nechcete optimalizovat – tedy
vyhodit. Proto je třeba sledovat chybová hlášení a název chybějící funkce zadat
do souboru `.make/exclude_reference` uvnitř projektového adresáře.

#### SDCC-gas

[SDCC-gas](https://github.com/XaviDCR92/sdcc-gas) vzniklo, aby vyřešilo problém
optimalizace mrtvého kódu přidáním podpory [GNU
Assembleru](https://cs.wikipedia.org/wiki/GNU_Assembler) tedy *gas* do SDCC
3.9.0. [gas](https://codedocs.org/what-is/gnu-assembler) je výhodou i nevýhodou
tohoto řešení. Na jednu stranu to znamená, že můžeme používat klasické nástroje
z [GNU binutils](https://cs.wikipedia.org/wiki/GNU_binutils), na druhou stranu
to znamená, že nelze použít ty části sdcc-libraries, které jsou napsané v STM8
assembleru a je nutné použít méně optimální kód napsaný v C nebo STM8 assembler
přepsat do GNU assembleru.


Použití
--------------

* `make spl` -- stáhne a nachystá knihovny
* `make` -- provede kompilaci
* `make flash` -- nahraje program do chipu
* `make clean` -- smaže všechno, co nakompiloval
* `make rebuild` -- smaže vše a znovu zkompiluje
* `make openocd` -- pustí `openocd` pro debug
* `make debug` -- spustí STM8-gdb


Závislosti
---------------

* GNU Make, GNU Bash, 
* [SDCC](http://sdcc.sourceforge.net/)
* [SDCC-gas](https://github.com/XaviDCR92/sdcc-gas)
* [OpenOCD](https://openocd.org/)
* [STM8 binutils](https://stm8-binutils-gdb.sourceforge.io) (`stm8-gdb`, `stm8-ln`)
