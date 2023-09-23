# 2d Jacobi

Este programa tem como objetivo comparar a eficiência de diferentes implementações da computação 2d de jacobi utilizando diferentes técnicas de paralelismo.

As técnicas aplicadas foram paralelização por threads com pthreads, por processos com Open MPI, e uma abordagem híbrida.

Desenvolvido durante a disciplina de Programação Paralela no curso de Ciência da Computação na UEM.

## Requisitos

Esse projeto foi desenvolvido para sistemas linux, utilizando o WSL2 Debian, com o GCC. São utilizadas as bibliotecas nativas `math.h`, `argp.h` e `pthread.h`. Também foi utilizada a biblioteca `mpi.h`, que pode ser obtida com:

```bash
$ sudo apt-get update
$ sudo apt install openmpi-bin libopenmpi-dev
```

## Como executar
Para compilar, é possível utilizar o makefile. Os binários serão gerados na pasta bin.

```bash
$ make
```

Alguns parâmetros, como tamanho da matriz e a flag de debug, devem ser alterados diretamente no [headerfile](src/common.h). Para recompilar:

```bash
$ make clean
$ make
```

Além disso, cada executável pode receber parâmetros adicionais em tempo de execução, que podem ser listados com `--help`.
```bash
bin/jacobi-2d-pthread -d MEDIUM -s 123 -t 4
```
