# [P4-ETAPA-02] Contrato Semântico e Testes

Este documento define **o que** o Simulador de Rede de Computadores Simplificada deve fazer, independentemente da linguagem ou do paradigma. Os mesmos casos serão usados para avaliar as implementações imperativa, orientada a objetos, funcional e lógica.

## 1. Formato da entrada

```
N M
<id> <ip> <vizinho>        (uma linha para cada um dos N nós)
P
"<nome>" <origem>          (uma linha para cada um dos P pacotes)
```

- `N` é a quantidade de nós, identificados de `1` a `N`, e `M` é a quantidade de conexões distintas entre nós.
- Cada linha de nó informa o identificador do nó, o endereço IP desse nó e um nó ao qual ele está conectado. As conexões valem nos dois sentidos, e uma conexão declarada duas vezes (por exemplo, `3 55 4` e `4 55 3`) conta uma única vez em `M`.
- Quando `<vizinho>` é o próprio nó (por exemplo, `5 90 5`), a linha não declara nenhuma conexão nova; ela apenas informa o IP do nó.
- `P` é a quantidade de pacotes. Cada pacote tem um nome (entre aspas) e o nó de onde ele sai.

## 2. Formato da saída

Para cada pacote, na ordem em que aparece na entrada, são impressas duas linhas: o nome do pacote seguido do nó onde ele parou (o quão longe ele foi), e depois o caminho completo percorrido, do nó de origem ao nó de parada, separado por espaços.

```
"<nome>" <nó de parada>
<origem> ... <nó de parada>
```

## 3. Regras de comportamento

**R1 — Mesmo IP.** Um pacote só pode passar por nós que tenham o mesmo endereço IP do seu nó de origem. O IP pertence ao nó, não ao pacote.

**R2 — Escolha do próximo nó.** A cada passo, os candidatos são os vizinhos do nó atual que têm o mesmo IP, excluindo o nó de onde o pacote acabou de vir. Entre os candidatos, o pacote segue para o de **maior identificador**.

**R3 — Parada.** Se não houver nenhum candidato, o pacote para no nó atual. O pacote não retrocede pelo caminho já feito.

**R4 — Sem repetição de nó.** Se o próximo nó escolhido pela R2 já estiver no caminho do pacote, a saída desse pacote é substituída por `"Package cannot go through the same emitter twice"` e ele é descartado da simulação.

**R5 — Colisão na entrada.** Se dois ou mais pacotes tiverem o mesmo nó de origem, a entrada é inválida: nenhuma simulação é feita e a saída inteira é apenas `"Package Collision"`.

**R6 — Colisão durante a simulação.** Um nó não pode ter dois pacotes ao mesmo tempo. Todos os pacotes avançam simultaneamente, um nó por passo, e um pacote parado continua ocupando o seu nó. Se, ao fim de qualquer passo, dois pacotes estiverem no mesmo nó, a saída inteira é apenas `"Package Collision"`.

---

## Casos Normais

```
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

Descrição:
  Escolhe o vizinho de maior ID e ignora o nó de IP diferente.
```

```
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

Descrição:
  Dois pacotes em redes de IPs diferentes.
```

```
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

Descrição:
  Para ao encontrar um vizinho de IP diferente, sem retroceder.
```

```
ID 04
Entrada:
  4 3
  1 10 2
  2 10 3
  3 10 4
  4 10 4
  1
  "test" 1

Saída:
  "test" 4
  1 2 3 4

Descrição:
  Percorre uma cadeia linear inteira.
```

```
ID 05
Entrada:
  4 3
  1 20 2
  2 20 2
  3 20 1
  4 20 1
  1
  "package" 1

Saída:
  "package" 4
  1 4

Descrição:
  Em uma estrela, escolhe o vizinho de maior ID.
```

```
ID 06
Entrada:
  5 4
  1 7 2
  2 7 3
  3 7 4
  4 7 5
  5 7 5
  1
  "interesting" 3

Saída:
  "interesting" 5
  3 4 5

Descrição:
  Parte do meio da cadeia e vai até a extremidade.
```

```
ID 07
Entrada:
  3 2
  1 30 2
  2 30 3
  3 99 3
  1
  "message" 2

Saída:
  "message" 1
  2 1

Descrição:
  Segue para o vizinho de menor ID quando o maior tem IP diferente.
```

```
ID 08
Entrada:
  6 5
  1 40 2
  2 40 3
  3 40 4
  4 50 5
  5 50 6
  6 50 6
  2
  "test" 1
  "important" 6

Saída:
  "test" 3
  1 2 3
  "important" 4
  6 5 4

Descrição:
  Cada pacote fica restrito à rede do seu IP.
```

```
ID 09
Entrada:
  6 5
  1 3 2
  2 3 3
  3 3 6
  4 3 2
  5 3 4
  6 3 6
  1
  "package" 1

Saída:
  "package" 5
  1 2 4 5

Descrição:
  Segue o ramo de maior ID, não o mais longo.
```

```
ID 10
Entrada:
  7 5
  1 60 2
  2 60 3
  3 60 3
  4 60 5
  5 60 6
  6 70 7
  7 70 7
  3
  "test" 1
  "interesting" 4
  "important" 7

Saída:
  "test" 3
  1 2 3
  "interesting" 5
  4 5
  "important" 6
  7 6

Descrição:
  Três pacotes simultâneos sem colisão.
```

## Casos-Limite

```
ID 11
Entrada:
  4 3
  1 20 2
  2 30 3
  3 30 4
  4 30 4
  1
  "package" 1

Saída:
  "package" 1
  1

Descrição:
  Único vizinho tem IP diferente; o pacote não se move.
```

```
ID 12
Entrada:
  1 0
  1 5 1
  1
  "package" 1

Saída:
  "package" 1
  1

Descrição:
  Rede com um único nó e nenhuma conexão.
```

```
ID 13
Entrada:
  3 2
  1 1 2
  2 1 3
  3 1 3
  2
  "test" 1
  "interesting" 3

Saída:
  "Package Collision"

Descrição:
  Dois pacotes chegam ao mesmo nó no mesmo passo.
```

## Casos de Entrada Inválida

```
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
  "Package Collision"

Descrição:
  Dois pacotes no mesmo nó de origem.
```

```
ID 15
Entrada:
  3 3
  1 41 2
  2 41 3
  3 41 1
  1
  "package" 1

Saída:
  "Package cannot go through the same emitter twice"

Descrição:
  Ciclo faz o pacote voltar a um nó já visitado.
```
