# ADT Library in C

## Struttura
- `include/`: header pubblici
- `src/`: implementazioni
- `test/`: test modulari (`test_stack.c`, `test_queue.c`, `test_list.c`)
- `Makefile`: build di libreria e test

## Build
```bash
make
```

Target principali:
- `make lib` -> crea `build/lib/libadt.a`
- `make tests` -> crea `build/bin/test_stack`, `build/bin/test_queue`, `build/bin/test_list`
- `make clean` -> rimuove `build/`

## Output build
- `build/obj/`: file oggetto
- `build/lib/`: libreria statica
- `build/bin/`: eseguibili
