STM8 (SPŠE) toolchain
==============================

* Toto je startovací strom zdrojových kódů a `Makefile` pro výuku Mikroprocesorové
  techniky
  s [STM8S](https://www.st.com/en/microcontrollers-microprocessors/stm8s-series.html).
* Strom je určen pro překladač [SDCC](http://sdcc.sourceforge.net/) nebo 
  [SDCC-gas](https://github.com/XaviDCR92/sdcc-gas).
* Standardní knihovnu pro práci s periferiemi 
  [SPL](https://www.st.com/content/st_com/en/products/embedded-software/mcu-mpu-embedded-software/stm8-embedded-software/stsw-stm8069.html)
  je třeba (z licenčních důvodů) stáhnou zvlášť ze stránek výrobce a použít
  [patch](https://github.com/gicking/STM8-SPL_SDCC_patch) (`make spl`).
* Konkurence a inspirace: \
  * <https://gitlab.com/wykys/stm8-tools>
  * <https://github.com/matejkrenek/stm8-toolchain>

Tři mejkfaily pro studenta
------------------------------------

K dispozici jsou celkem tři `Makefile` v adresáři `.make`. Na začátku si musíte
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


### Který `Makefile` vybrat?

Detailní popis najdete na <https://chytrosti.marrek.cz/stm8oss.html>.

#### SDCC

Z hlediska STM8 má [SDCC](http://sdcc.sourceforge.net/) jednu zásadní nevýhodu:
nedokáže odstranit mrtvý nepoužívaný kód. To může zapříčinit velké binární
soubory plné nepoužívaného kódu. Pokud nepoužíváte knihovny 2. a 3. stran
(třeba SPL) asi vám to nevadí.

#### sdccrm

[sdccrm](https://github.com/XaviDCR92/sdccrm) je nástroj pro optimalizaci
mrtvého kódu vytvořeného SDCC, který odstraňuje nepoužívané funkce. Kód se
nejprve zkompiluje do assembleru klasickým SDCC, poté se pomocí sdccrm vymaže
kód, který se nepoužívá, celý proces se dokončí a kód se převede z assembleru
do strojového kódu. Z logiky věci toto řešení vylučuje použití debugeru.

Dále **je nutné** ručně zadat/editovat funkce, které nechcete optimalizovat –-
tedy vyhodit. Proto je třeba sledovat chybová hlášení a název chybějící funkce
zadat do souboru `exclude_reference` uvnitř projektového adresáře.

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

Nejprve je třeba v `Makefile` správně nastavit µprocesor a jeho frakvenci;
případně cestu k instalaci SDCC
[STVP](https://www.st.com/en/development-tools/stvp-stm8.html).

```make
#DEVICE_FLASH=stm8s103f3
DEVICE_FLASH=stm8s208rb

### STlink version for falsh2 targer (stm8flash program)
#STLINK=stlinkv2
#STLINK=stlink
STLINK=stlinkv21

F_CPU=16000000

ifeq ($(OS),Windows_NT)
	CC_ROOT = "/c/Program Files/SDCC"
	STVP_ROOT = "/c/Program Files (x86)/STMicroelectronics/st_toolset/stvp"
else
	CC_ROOT = /usr
endif
```

Pokud používáte `sdcc` nebo `sdccrm` je ještě potřebné v `Makefile`
odkomentovat nebo zakomentovat nebo přidat ty části SPL knihovny, které zrovna
(ne)používáte.

```make
SPL_SOURCE  = stm8s_gpio.c stm8s_clk.c stm8s_tim4.c stm8s_itc.c 
SPL_SOURCE += stm8s_uart1.c
#SPL_SOURCE += stm8s_adc2.c
#SPL_SOURCE += stm8s_tim1.c
SPL_SOURCE += stm8s_tim2.c
#SPL_SOURCE += stm8s_tim3.c
```

... no a potom už jen bastlíte, programujete a voláte `make`.

Pokud používáte `sdcc-gas` budete možná muset vytvořit pro váš µprocesor
linker-script. Jsou to soubory s příponou `*.x` v adresáři `.make`. Tento
toolchain *zatím* obsahuje pouze linker-scripty pro µprocesory:  `stm8s003f3`,
`stm8s103f3` a `stm8s208rb`. Pokud tak učiníte, jistě mi jej zašlete k zařazení
do tohoto toolchainu.

| příkaz&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;||
|:---------- |:--------------------------- |
| `make spl` | stáhne a nachystá knihovny |
| `make` | provede kompilaci |
| `make flash` | nahraje program do chipu. Na Linuxu se použije [OpenOCD](https://openocd.org/). Na Windows se použije [STVP](https://www.st.com/en/development-tools/stvp-stm8.html) verze pro příkazový řádek.|
| `make flash2` | záložní varianta, protože OpenOCD někdy nechce čip naprogramovat (používá [stm8flash](https://github.com/vdudouyt/stm8flash)).
| `make clean` | smaže všechno, co nakompiloval
| `make rebuild` | smaže vše a znovu zkompiluje
| `make openocd` | pustí `openocd` pro debug
| `make debug` | spustí STM8-gdb


Závislosti
---------------

* [GNU Make](https://www.gnu.org/software/make/)
* [GNU Bash](https://www.gnu.org/software/bash/) -- ten se na Windows
  dá nainstalovat společně s [Git](https://git-scm.com/download/win)em.
* [SDCC](http://sdcc.sourceforge.net/)
  nebo [SDCC-gas](https://github.com/XaviDCR92/sdcc-gas)
* [STM8 binutils](https://stm8-binutils-gdb.sourceforge.io) (`stm8-gdb`, `stm8-ln`)
* [OpenOCD](https://openocd.org/) pro `flash` a `debug`
  nebo [STVP](https://www.st.com/en/development-tools/stvp-stm8.html)
  pro `flash` na Windows.
* ([stm8flash](https://github.com/vdudouyt/stm8flash) pro `flash2`)

### Na Windows

[`choco`](https://chocolatey.org/)` install git make vscode mingw`

