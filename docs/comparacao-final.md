# [P4-ETAPA-05] Comparação entre Imperativo e POO

Análise comparativa entre `imperativo/codigo.cpp` e `poo/codigo.cpp`. As duas implementações resolvem o mesmo problema, seguem o mesmo contrato (`testes/casos.md`) e produzem a mesma saída nos 15 casos de teste.

## Visão geral

| | Imperativo | Orientado a objetos |
|---|---|---|
| Unidades | 4 funções + `main` | 5 classes + `main` |
| Estado | 10 variáveis globais | atributos privados em 3 classes |
| Tamanho | 128 linhas | cerca de 175 linhas |

## Aspectos comparados

### Representação do estado

- **Imperativo:** vetores globais paralelos (`ip`, `adj`, `nome`, `caminho`, `anterior`, `ativo`, `descartado`). Um pacote não existe como unidade: é apenas o índice `i`, que liga seus dados entre os vetores.
- **POO:** o estado pertence a quem ele descreve. `ip` e `adj` são atributos de `Rede`; `nome`, `caminho`, `anterior`, `ativo` e `descartado` são atributos de cada objeto `Pacote`.

### Mutabilidade

- **Imperativo:** todo o estado é mutável e acessível por qualquer função. Nada impede, por exemplo, escrever `ativo[i] = true` em um pacote descartado.
- **POO:** o estado continua mutável, mas a mutação é controlada. `ativo`, `descartado` e `caminho` só mudam dentro de `Pacote::Step`. A `Rede` só é alterada em `Rede::Entrada`; depois disso, só oferece métodos de leitura (`const`).

### Fluxo de controle

- **Imperativo:** o fluxo inteiro está visível na `main`: `Entrada()`, `Collision()`, `while (Step())` com `Collision()` a cada passo, e a impressão.
- **POO:** a mesma sequência está em `Simulador::Executar`, mas parte do fluxo fica distribuída entre objetos: o `Simulador` pede a cada `Pacote` que ande, e o `Pacote` pede ao `Roteamento` o próximo nó. Para seguir a execução, é preciso passar por três classes.

### Decomposição do problema

- **Imperativo:** por **ação**. Cada função é uma etapa do algoritmo: `Entrada`, `Next`, `Step`, `Collision`.
- **POO:** por **entidade**. Cada classe é um elemento do problema: `Rede`, `Pacote`, `Roteamento`, `Simulador`.

### Reutilização

- **Imperativo:** as funções são reutilizadas dentro do programa (o `Collision` serve para R5 e R6), mas dependem das variáveis globais e não funcionam fora dele.
- **POO:** `Pacote::Step` é escrito uma vez e usado por todos os pacotes. A regra de roteamento é independente: pode ser usada com outra rede ou em outra simulação.

### Manutenção

- **Imperativo:** uma mudança na forma de guardar um dado (por exemplo, trocar `adj` por uma matriz) exige revisar todas as funções que o acessam.
- **POO:** a mudança fica isolada na classe dona do dado. Trocar a estrutura interna da `Rede` não afeta `Pacote` nem `Simulador`, desde que `getIp` e `getVizinhos` continuem iguais.

### Facilidade de extensão

- **Imperativo:** uma nova regra de roteamento exige editar a função `Next` ou acrescentar condicionais.
- **POO:** basta criar uma nova classe que herda de `Roteamento` e entregá-la ao `Simulador` na `main`. Nenhuma classe existente é alterada.

### Tratamento de erros

Praticamente igual nas duas versões. As situações inválidas previstas no contrato (colisão e repetição de nó) são tratadas como **saídas normais do programa**, por mensagens no `cout`. Nenhuma das implementações usa exceções nem valida o formato da entrada. A diferença é apenas onde a detecção fica: no imperativo, no `Step` e na `main`; no POO, em `Pacote::Step` (repetição) e `Simulador::Collision` (colisão).

### Efeitos colaterais

- **Imperativo:** `Entrada` e `Step` alteram variáveis globais. O `Step` devolve apenas um `bool`, e o resultado principal (os pacotes terem andado) acontece fora do valor de retorno.
- **POO:** os efeitos ainda existem, mas ficam restritos ao próprio objeto: `Pacote::Step` só altera o pacote em que foi chamado. Os métodos que apenas consultam (`Next`, `Collision`, getters, `Imprimir`) são marcados como `const`, e o compilador garante que não alteram nada.

### Facilidade para testar

- **Imperativo:** para testar o `Next` isoladamente, é preciso preencher antes as globais `adj` e `ip`, e o estado de um teste permanece para o seguinte.
- **POO:** `MaiorIdMesmoIp::Next` e `Pacote::Step` recebem a `Rede` por parâmetro, então podem ser testados com objetos criados só para o teste. A limitação é que a `Rede` só pode ser preenchida pelo `cin`.

### Organização do código

- **Imperativo:** dados no topo do arquivo, funções abaixo. Dados e operações ficam separados.
- **POO:** dados e operações ficam juntos em cada classe, com a parte pública separada da privada.

### Complexidade

- **Imperativo:** menor. Menos linhas, menos conceitos e leitura de cima a baixo.
- **POO:** maior. Mais linhas, cinco classes, getters, referências, `virtual` e `override`. Para um problema deste tamanho, o custo estrutural é maior do que o do algoritmo em si.

## Perguntas

### 1. Qual problema ficou mais fácil de expressar de forma imperativa?

A função `Entrada`. No imperativo ela é extremamente direta: uma única função lê tudo do `cin` e grava direto nos vetores globais. No POO, a mesma leitura precisou ser dividida em duas (`Rede::Entrada` para os nós e `Simulador::Entrada` para os pacotes), porque cada classe só pode preencher os próprios dados.

### 2. Qual problema ficou mais fácil de expressar utilizando orientação a objetos?

O roteamento. No imperativo, a regra era uma função fixa (`Next`). No POO, ela virou a interface `Roteamento`, com a regra atual em `MaiorIdMesmoIp`. Isso deixa claro que "escolher o próximo nó" é uma parte separada do problema e pode ser trocada sem mexer no resto.

### 3. Onde a orientação a objetos realmente trouxe vantagem?

- **Modularidade:** o código ficou dividido em classes com responsabilidades separadas (`Rede`, `Pacote`, `Roteamento`, `Simulador`). Tenho mais familiaridade com código modular, então ficou mais fácil de ler e de alterar uma parte sem afetar as outras.
- **O pacote cuida de si mesmo:** cada `Pacote` guarda o próprio caminho e situação, decide se anda, para ou é descartado (`Pacote::Step`) e imprime o próprio resultado (`Imprimir`). No imperativo, isso ficava espalhado entre o `Step` e a `main`.

### 4. Em quais situações a utilização de objetos acrescentou complexidade desnecessária?

- **Getters:** `getIp`, `getVizinhos`, `getTamanho`, `getPosicao` e `estaDescartado` só repassam dados que, no imperativo, eram lidos direto dos vetores.
- **Herança:** a hierarquia `Roteamento` → `MaiorIdMesmoIp` existe para uma única regra. Enquanto não houver uma segunda, a classe abstrata e o `virtual` são código a mais sem ganho prático. Ela só se paga quando o sistema for estendido.

### 5. Que partes do problema praticamente não mudaram entre as duas implementações?

`Next`, `Step` e `Collision`. A lógica é a mesma nas duas versões:

- `Next`: o mesmo laço procurando o maior vizinho com o mesmo IP;
- `Step`: as mesmas três decisões (parar, descartar, andar), na mesma ordem;
- `Collision`: a mesma contagem de pacotes por nó.

O que mudou foi apenas onde elas ficam (dentro de classes) e como acessam os dados (atributos e getters em vez de globais).

### 6. Que partes precisaram ser completamente remodeladas?

- **Estado:** de vetores globais paralelos para atributos privados dentro dos objetos.
- **Pacote:** de um índice `i` ligando cinco vetores para um objeto com dados e comportamento próprios.
- **Impressão:** saiu da `main` e foi para `Pacote::Imprimir`; cada pacote imprime a si mesmo.
- **Leitura da entrada:** de uma função única que preenchia as globais para duas, uma em cada classe dona dos dados, sendo que a do `Simulador` cria os objetos `Pacote`.

## Conclusão

Para este problema, a versão imperativa é mais curta e mais direta, e a lógica central (`Next`, `Step`, `Collision`) é a mesma nas duas. A orientação a objetos não simplificou o algoritmo: ela reorganizou o estado, protegeu os dados do pacote e criou um ponto de extensão para a regra de roteamento. O ganho aparece na manutenção e na extensão, e o custo aparece no tamanho e na quantidade de conceitos.
