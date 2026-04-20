<p align="center">
  <h1 align="center">anto_lib</h1>
</p>

<p align="center">
  Libreria ADT in C11 con struttura modulare e build statica.
</p>

---

## About

```sh
anto_lib@c:~$ whoami
Minimal ADT Library in C

anto_lib@c:~$ echo "focus"
moduli semplici, build pulita, test rapidi

anto_lib@c:~$ echo "entry header"
anto_lib.h
```

`anto_lib` e' una libreria C organizzata per moduli (`stack`, `queue`, `list`, `tree`, `common`, `util`) e compilata come archivio statico (`libadt.a`).

---

## Highlights

- Build libreria statica con `Makefile`
- Header modulari in `include/`
- Header di ingresso unico opzionale: `anto_lib.h`
- Utility di debug riusabile in `util` (`debug_status`)
- Stack gia' implementato e testato

---

## Quick Start

```bash
make lib
```

Test stack:

```bash
make test_stack
./build/bin/test_stack
```

Windows (MSYS2):

```powershell
C:\msys64\usr\bin\bash.exe -lc "export PATH=/ucrt64/bin:/usr/bin:$PATH; cd /d/Uni/programmazione2/es1 && make test_stack && ./build/bin/test_stack"
```

---

## Project Layout

- `include/`: header pubblici per i moduli
- `src/`: implementazioni dei moduli
- `test/`: test modulari (`test_stack.c` attivo)
- `anto_lib.h`: punto di ingresso unico opzionale
- `Makefile`: build di libreria e test

---

## Stato Attuale

- `stack`: implementato e verificato
- `queue`, `list`, `tree`: da completare
- `test_queue.c`, `test_list.c`: da completare

---

## Output Build

- `build/obj/`: file oggetto
- `build/lib/`: libreria statica
- `build/bin/`: eseguibili
