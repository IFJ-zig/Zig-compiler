# IFJ24 Project

## Project Overview

Compiler for imperative language Zig. Built using C.

## Team Members

- **xbohatd00**: Responsible for IFJcode24 code generator.
- **xvozdaa00**: Responsible for Syntactic analysis and AST generation.
- **xhorakt00**: Responsible for Semantic analysis and AST library.
- **xpuchn02**: Responsible for Lexical analysis.

## Grading Breakdown

- **18 points** for compiler implementation in C.
  - 5 points will be awarded for the **IAL** course based on this component.
- **5 points** for documentation and project management.
  - Another 5 points for **IAL** will be based on these.
- **5 points** for additional activities (e.g., extensions, forum activity, bug reports, or competitions).
- **5 points** for the final project defense.
  - An additional 5 points for **IAL** will be based on the defense.

## Useful links

- [IFJ E-Learning](https://moodle.vut.cz/course/view.php?id=280931)
- [Zadání projektu (pdf)](https://www.fit.vut.cz/study/course/IFJ/private/projekt/ifj2024.pdf)
- [IFJ Youtube stream](https://www.youtube.com/playlist?list=PL_eb8wrKJwYv5QeMHsmWNBKohVDK6yqHk)

## Guide to run tests

In root run

```bash
cd ./tests
git clone https://github.com/jansko03/IFJ24-tests.git
chmod +x ./IFJ24-tests/test.sh
cd ..
```

Then

Linux/Mac:

```bash
./tests/IFJ24-tests/test.sh ./tests/IFJ24-tests ./ifj24.out ./ifjcode24/ic24int_linux_2024-11-21/ic24int
```

Windows:

```bash
./tests/IFJ24-tests/test.sh ./tests/IFJ24-tests ./ifj24.out ./ifjcode24/ic24int_win_2024-11-21/ic24int
```
