
# [P4-ETAPA-03] Decisões de Implementação — Paradigma Imperativo

Este documento explica as decisões tomadas na implementação em `imperativo/`, seguindo o que foi pedido no enunciado da Etapa 3. A solução foi validada contra todos os 15 casos definidos em `testes/casos.md`.

## Quais estados são mantidos

O estado da simulação inteira fica em vetores globais, um por característica, indexados pelo id do nó ou pelo índice do pacote:

- `ip[no]` — endereço IP de cada nó.
- `adj[no]` — lista de nós conectados a cada nó.
- `caminho[pacote]` — sequência de nós já percorridos por cada pacote.
- `anterior[pacote]` — nó de onde o pacote veio no último passo, usado para não voltar por onde já passou.
- `ativo[pacote]` — se o pacote ainda está se movendo.
- `descartado[pacote]` — se o pacote foi eliminado por repetir um nó (R4).

Não existe nenhuma estrutura que agrupe esses dados em um só lugar (nem `struct`, nem classe). Cada vetor guarda uma única informação, e a posição `i` em todos eles se refere sempre ao mesmo pacote. É o próprio índice que amarra o estado de cada pacote entre os vetores.

## Quais operações modificam estado

- `Entrada()` monta os vetores a partir da entrada: preenche `ip`, `adj`, `nome` e a origem de cada pacote em `caminho`.
- `Step()` é quem mais modifica estado: a cada chamada, ela decide o próximo nó de cada pacote ativo e atualiza `caminho`, `anterior`, `ativo` e `descartado` conforme o resultado.
- A `main` também modifica estado, mas de forma indireta: ela decide quando parar de chamar `Step()`, controlando o andamento da simulação através do valor que essa função devolve.

`Next()` e `Collision()` não modificam nada — só leem o estado atual e respondem uma pergunta (qual o próximo nó? existe colisão?).

## Onde aparecem efeitos colaterais

O maior efeito colateral está em `Step()`: ela não devolve o novo estado dos pacotes, ela altera os vetores globais diretamente e só retorna um `bool` dizendo se algo se moveu. Ou seja, o efeito relevante (os pacotes terem andado) acontece por fora do valor de retorno.

`Entrada()` também é puro efeito colateral: ela não retorna nada, só preenche as variáveis globais lendo da entrada padrão.

A leitura (`cin`) e a escrita (`cout`) também são efeitos colaterais no sentido mais literal — o programa interage com o mundo fora dele (terminal), e não só com dados internos.

## Quais estruturas de controle foram escolhidas

- **`for` com índice**, em vez de `for` com `auto` ou iteradores, para percorrer nós, vizinhos e pacotes. Isso deixa explícito qual posição do vetor está sendo lida ou alterada a cada iteração, o que é mais fácil de acompanhar passo a passo do que uma iteração "escondida" em um iterador.
- **`while (Step())`** na `main`, controlando a simulação inteira. Ela para sozinha quando `Step()` retorna `false`, ou seja, quando nenhum pacote ativo conseguiu se mover no último passo. Isso evita ter que calcular de antemão quantos passos a simulação vai precisar.
- **`if` / `continue`** dentro dos laços de `Step()`, para pular pacotes que já pararam (`!ativo[i]`) sem precisar aninhar o resto do código em um bloco `else`.

## Como os subprogramas foram organizados

A solução tem quatro funções, cada uma com uma responsabilidade:

| Função | O que faz |
|---|---|
| `Entrada()` | Lê o problema e prepara os vetores de estado. |
| `Next(atual, veioDe, rede)` | Aplica a R2: escolhe o próximo nó de um pacote, dado onde ele está e de onde veio. |
| `Collision()` | Aplica R5/R6: verifica se dois pacotes estão no mesmo nó. |
| `Step()` | Avança todos os pacotes ativos em um nó, usando `Next()` para decidir o destino de cada um. |

A `main` amarra tudo: chama `Entrada()`, checa colisão inicial (R5), roda `Step()` em loop checando colisão a cada passo (R6), e no final imprime o resultado de cada pacote. A impressão fica na `main` de propósito, para que ela conte a história inteira da simulação do início ao fim, sem esconder nenhuma etapa dentro de uma função.

## Por que a solução pode ser considerada predominantemente imperativa

Não existe nenhuma classe, nenhum objeto e nenhum método — só funções soltas que leem e alteram variáveis globais. O estado (quem está onde, quem já passou por onde) muda ao longo do tempo através de atribuições diretas nos vetores, e o comportamento do programa depende diretamente desse estado indo de um valor para o outro a cada passo, não de valores imutáveis passados adiante entre chamadas.

O fluxo de execução também é sequencial e explícito: a `main` mostra, em ordem, exatamente o que o programa faz — ler, verificar, simular passo a passo, imprimir — sem recursão, sem funções de ordem superior e sem nenhuma composição que esconda essa sequência. Isso é o oposto de uma solução funcional (que evitaria estado mutável e usaria valores imutáveis) ou orientada a objetos (que agruparia nó e pacote em entidades com métodos próprios).

## Validação

Todos os 15 casos de `testes/casos.md` foram executados contra a implementação e bateram com a saída esperada, cobrindo os casos normais, os casos-limite e os de entrada inválida.
