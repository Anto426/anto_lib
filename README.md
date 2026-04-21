<p align="center">
  <h1 align="center">anto_lib</h1>
</p>

<p align="center">
  Libreria ADT in C11 con struttura modulare e build statica.
</p>

<p align="center">
  <img src="./asset/divider.gif" width="440" height="40" />
</p>

## <img src="./asset/icon.gif" width="44px" /> About

```sh
anto_lib@c:~$ whoami
Minimal ADT Library in C

anto_lib@c:~$ echo "focus"
moduli semplici, build pulita, test rapidi

anto_lib@c:~$ echo "entry header"
anto_lib.h
```

`anto_lib` e' una libreria C organizzata per moduli (`stack`, `queue`, `list`, `tree`, `hash_table`, `common`, `util`) e compilata come archivio statico (`libadt.a`).

<p align="center">
  <img src="./asset/divider.gif" width="440" height="40" />
</p>

## <img src="./asset/icon2.gif" width="48px" /> Highlights

- Build libreria statica con `Makefile`
- Header modulari in `include/`
- Header di ingresso unico opzionale: `anto_lib.h`
- Utility di debug riusabile in `util` (`debug_status`)
- ADT principali implementati e testati (`stack`, `queue`, `list`, `tree`, `hash_table`)

<p align="center">
  <img src="./asset/divider.gif" width="440" height="40" />
</p>

## <img src="./asset/icon3.gif" width="48px" /> Quick Start

```bash
make lib
```

Test completi:

```bash
make tests
./build/bin/test_stack
./build/bin/test_list
./build/bin/test_queue
./build/bin/test_tree
./build/bin/test_hash_table
```

Windows (MSYS2):

```powershell
C:\msys64\usr\bin\bash.exe -lc "export PATH=/ucrt64/bin:/usr/bin:$PATH; cd /d/Uni/programmazione2/es1 && make tests && ./build/bin/test_stack && ./build/bin/test_list && ./build/bin/test_queue && ./build/bin/test_tree && ./build/bin/test_hash_table"
```

<p align="center">
  <img src="./asset/divider.gif" width="440" height="40" />
</p>

## <img src="./asset/icon4.gif" width="48px" /> Project Layout

- `include/`: header pubblici per i moduli
- `src/`: implementazioni dei moduli
- `test/`: test modulari (`test_stack.c`, `test_queue.c`, `test_list.c`, `test_tree.c`, `test_hash_table.c`)
- `anto_lib.h`: punto di ingresso unico opzionale
- `asset/`: risorse grafiche del README
- `Makefile`: build di libreria e test

<p align="center">
  <img src="./asset/divider.gif" width="440" height="40" />
</p>

