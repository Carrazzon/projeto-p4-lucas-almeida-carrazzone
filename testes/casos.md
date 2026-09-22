# CASOS DE TESTE:
## Casos Normais:

ID 01
Entrada:
  8 7
  1 55 2
  2 55 3
  3 55 4
  4 55 3
  5 55 3
  6 12 3
  7 55 6
  8 55 5
  1
  "package" 1

Saída:
  "package" 8
  1 2 3 5 8

ID 02
Entrada:
  6 5
  1 5 2
  2 5 3
  3 1 3
  4 1 5
  5 1 6
  6 1 3
  2
  "test" 1
  "important" 4

Saída:
  "test" 2
  1 2
  "important" 3
  4 5 6 3

ID 03
Entrada:
  5 4
  1 90 2
  2 90 3
  3 90 4
  4 1 5
  5 90 5
  1
  "package" 2

Saída:
  "package" 3
  2 3


## Casos-Limite:

ID 11

  

## Casos de Entrada Inválida:

ID 14
Entrada:
  5 4
  1 50 2
  2 50 3
  3 50 4
  4 50 4
  5 50 3
  2
  "package" 1
  "training" 1

Saída:
  "Package Colision."
  
ID 15
Entrada:
  3 3
  1 41 2
  2 41 3
  3 41 1
  1
  "package" 1

Saída:
  "Package cannot go through the same emitter twice."
