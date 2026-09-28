# [P4-ETAPA-04] Reflexão — Do Imperativo para o Orientado a Objetos
 
## Como meu modelo mudou?
 
No imperativo, o programa era um conjunto de funções soltas agindo sobre vetores globais. No orientado a objetos, o problema foi modelado pelas **entidades que existem nele**: a rede, os pacotes, a regra de roteamento e o simulador que coordena tudo. Cada entidade guarda seus próprios dados e sabe executar suas próprias ações.
 
| Imperativo | Orientado a objetos |
|---|---|
| `ip`, `adj` globais | atributos privados de `Rede` |
| `nome[i]`, `caminho[i]`, `anterior[i]`, `ativo[i]`, `descartado[i]` | atributos privados de cada objeto `Pacote` |
| `Next()` função solta | método de `Roteamento`, com implementação em `MaiorIdMesmoIp` |
| `Step()` altera todos os vetores | `Pacote::Step()` move um pacote; `Simulador::Step()` pede a cada pacote que se mova |
| `Collision()` lê vetores globais | método privado de `Simulador` |
| `main()` com todo o fluxo | `Simulador::Executar()` |
 
## Representação do estado
 
No imperativo, um pacote não existia como unidade: seus dados estavam espalhados em cinco vetores, ligados apenas pelo índice `i`. Nada impedia, por exemplo, que `ativo` tivesse um tamanho diferente de `caminho`.
 
No orientado a objetos, **cada pacote é um objeto** que guarda seu nome, caminho, nó anterior e situação. O índice deixou de ser a "cola" entre os dados: o próprio objeto os mantém juntos. A rede também virou um objeto, dono dos IPs e da lista de vizinhos.
 
## Responsabilidades
 
- **`Rede`**: montar e guardar a topologia; responder qual o IP de um nó e quem são seus vizinhos.
- **`Roteamento` / `MaiorIdMesmoIp`**: decidir o próximo nó (R1 e R2).
- **`Pacote`**: avançar a si mesmo, parar (R3), detectar repetição e se descartar (R4) e imprimir o próprio resultado.
- **`Simulador`**: ler os pacotes, conduzir os passos e detectar colisões (R5 e R6).
 
No imperativo, o `Step` fazia tudo por todos os pacotes. Agora cada pacote cuida da própria regra de movimento e o simulador apenas coordena.
 
## Relacionamento entre componentes
 
- **Composição:** o `Simulador` contém a `Rede` e a lista de `Pacote`s. Eles existem apenas dentro dele e são destruídos com ele.
- **Agregação:** o `Simulador` guarda uma referência a um `Roteamento` que ele não cria nem destrói. A regra é criada fora (na `main`) e entregue a ele.
- **Dependência:** o `Pacote` recebe a `Rede` e o `Roteamento` como parâmetros de `Step`, sem guardá-los. Ele só os usa para decidir o movimento.
 
## Reutilização
 
A lógica de movimento está escrita uma única vez em `Pacote::Step` e é reaproveitada por todos os objetos `Pacote`. A regra de roteamento é independente do pacote e do simulador: a mesma `Rede` pode ser usada com outra regra, e a mesma regra pode ser usada em outra simulação.
 
## Encapsulamento
 
Todos os atributos são privados. No imperativo, qualquer função podia escrever `ativo[i] = true` em um pacote descartado e deixar o estado inconsistente. Agora, o único jeito de alterar um pacote é chamar `Step()`, que aplica as regras corretamente. Quem está de fora só consegue **perguntar** (`getPosicao`, `estaDescartado`) e **pedir ações** (`Step`, `Imprimir`).
 
Da mesma forma, a `Rede` não expõe seus vetores para escrita: fora dela só é possível consultar o IP e os vizinhos.
 
## Extensão do sistema
 
O ponto de extensão principal é a regra de roteamento. Para criar, por exemplo, uma regra que escolhe o vizinho de **menor** ID, basta criar outra classe:
 
```cpp
class MenorIdMesmoIp : public Roteamento {
    int Next(...) const override { ... }
};
```
 
e entregá-la ao `Simulador` na `main`. `Pacote`, `Rede` e `Simulador` não precisam de nenhuma alteração. No imperativo, seria preciso editar a função `Next` diretamente ou espalhar `if`s pelo código.
 
## Uso de herança e polimorfismo
 
A herança foi usada **apenas** em `Roteamento` → `MaiorIdMesmoIp`, porque é o único ponto do problema em que faz sentido ter **várias variações do mesmo comportamento**. `Roteamento` é uma classe abstrata (interface) e o `Pacote` chama `roteamento.Next(...)` sem saber qual regra concreta está por trás: isso é polimorfismo.
 
Onde a herança **não** foi usada, e por quê:
 
- **Pacote parado / descartado:** poderia virar subclasses (`PacoteParado`, `PacoteDescartado`), mas um pacote muda de situação durante a execução, e um objeto não pode trocar de classe. Dois atributos `bool` resolvem de forma mais simples.
- **Rede e Pacote:** não há relação "é um" entre eles. O simulador **tem** uma rede e **tem** pacotes, então a relação correta é composição.
 


