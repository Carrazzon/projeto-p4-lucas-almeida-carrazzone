# [P4-ETAPA-06] Implementação Funcional

A solução foi escrita em C++17, no estilo funcional, para manter a mesma linguagem das versões imperativa e orientada a objetos e facilitar a comparação entre os paradigmas. Ela passa nos 15 casos de `testes/casos.md` com a mesma saída das outras versões.

## Visão geral

O programa é uma sequência de transformações:

```
cin → Entrada → Rede + pacotes → simulação → texto da saída → cout
```

Toda a lógica do problema está em `resolver`, que é pura. A leitura (`lerEntrada`) e a escrita (`cout` na `main`) ficam nas pontas do programa.

## Funções puras

Recebem dados, devolvem dados novos e não alteram nada fora delas. Com os mesmos argumentos, sempre dão o mesmo resultado.

| Função | O que faz |
|---|---|
| `montarRede` | transforma as linhas lidas em uma `Rede` (IPs e vizinhos) |
| `maiorIdMesmoIp` | escolhe o próximo nó (R1, R2) |
| `posicao` | devolve o nó onde o pacote está |
| `avancar` | devolve o pacote depois de um passo (R3, R4) |
| `passo` | devolve todos os pacotes depois de um passo |
| `posicoesOcupadas`, `temRepetido`, `temColisao` | verificam colisão (R5, R6) |
| `simular` | aplica passos recursivamente até o fim ou até uma colisão |
| `formatarPacote`, `formatarResultado` | transformam o resultado em texto |
| `resolver` | compõe tudo: da `Entrada` ao texto final |

Algumas dessas funções alteram uma variável local (o `push_back` em `avancar`, o `sort` em `temRepetido`, o acumulador em `montarRede`). Essa alteração acontece só em uma **cópia local**: o dado recebido nunca muda e nada fora da função é afetado. Por isso a função continua pura para quem a chama.

## Funções impuras

| Função | Por que é impura |
|---|---|
| `lerEntrada` | lê do `cin`: o resultado depende do que o usuário digita |
| `main` | escreve no `cout` |

São os **únicos pontos em que o programa interage com o mundo externo**.

## Dados imutáveis

`Rede`, `Pacote`, `LinhaNo` e `Entrada` são apenas dados. Depois de criados, nunca são alterados: as funções os recebem por `const&` e, quando precisam de uma versão diferente, criam um valor novo. Por exemplo, `avancar` não move o pacote: devolve **outro** `Pacote`, com o caminho maior ou com outra `Situacao`.

## Transformações de dados

| Transformação | Função |
|---|---|
| linhas de nó → `Rede` | `montarRede` (fold com `accumulate`) |
| vizinhos → próximo nó | `maiorIdMesmoIp` (fold com `accumulate`) |
| `Pacote` → `Pacote` depois de um passo | `avancar` |
| `vector<Pacote>` → `vector<Pacote>` depois de um passo | `passo` (map com `transform`) |
| `vector<Pacote>` → posições ocupadas | `posicoesOcupadas` (filter com `copy_if` + map com `transform`) |
| resultado → texto | `formatarPacote` e `formatarResultado` (fold com `accumulate`) |

## Funções de primeira classe

Funções são tratadas como valores:

- **`Roteamento`** é um tipo de função. `maiorIdMesmoIp` é uma variável que guarda uma função e é passada como argumento para `resolver`, `simular`, `passo` e `avancar`. Trocar a regra é passar outra função, sem criar classes.
- `posicao` é passada pelo nome para `transform`, e várias funções anônimas (lambdas) são passadas para `copy_if`, `any_of` e `accumulate`.

## Funções de alta ordem

Funções que recebem outras funções:

- da biblioteca: `transform` (map), `copy_if` (filter), `accumulate` (fold) e `any_of`;
- do projeto: `avancar`, `passo`, `simular` e `resolver` recebem a regra de roteamento como parâmetro.

## Composição funcional

O resultado de uma função vira a entrada da próxima:

- `resolver`: `formatarResultado(simular(montarRede(e), rota, e.pacotes))`
- `temColisao`: `temRepetido(posicoesOcupadas(pacotes))`

## Recursão

`simular` substitui o `while` das outras versões:

```cpp
if (temColisao(pacotes)) return nullopt;                 // caso base: colisão
if (!algumAtivo) return pacotes;                         // caso base: ninguém mais anda
return simular(rede, rota, passo(rede, rota, pacotes));  // chama de novo com os pacotes novos
```

Não existe variável de controle sendo alterada: cada chamada recebe uma versão nova dos pacotes. A recursão termina porque um pacote nunca repete nó (R4), então são no máximo N passos.

## Reflexão: de "que estado modificar?" para "que transformação aplicar?"

No imperativo, eu pensava em **onde guardar e o que alterar**: os vetores globais eram a memória da simulação, e cada passo do `Step` escrevia nela (`ativo[i] = false`, `caminho[i].push_back(prox)`). Para entender o programa, era preciso acompanhar como essa memória mudava ao longo do tempo.

No funcional, a pergunta passou a ser **"dado isto, o que eu devolvo?"**:

- Um pacote não "anda": `avancar` recebe um pacote e devolve o pacote do passo seguinte.
- A simulação não é um laço que altera variáveis: é a aplicação repetida da transformação `passo`, até que os pacotes parem ou colidam.
- A colisão não é contada em um vetor global: é a composição "filtrar quem ocupa nó → pegar as posições → ver se há repetidas".
- A saída não é impressa durante a execução: o resultado inteiro vira um texto, e só no fim esse texto é escrito.

A consequência mais visível é que **o estado não tem mais "antes" e "depois" no mesmo lugar**: cada passo gera valores novos, e os antigos continuam existindo e inalterados. Isso elimina perguntas que existiam no imperativo, como "em que ordem posso atualizar `anterior` e `caminho`?", porque não há atualização: o novo pacote é montado de uma vez a partir do antigo.

Também mudou a forma de testar: como `resolver` é pura, dá para chamá-la com uma `Entrada` montada no próprio código e comparar o texto devolvido, sem depender do teclado, da tela ou de variáveis globais preenchidas antes.

O custo é que cada passo copia os pacotes e os caminhos, o que gasta mais memória do que alterar no lugar. Para o tamanho deste problema, isso não faz diferença.
