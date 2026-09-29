# projeto-p4-lucas-almeida-carrazzone
 
# Mini Packet Tracer em C++
 
### ALUNO: Lucas Almeida Carrazzone
### Etapa atual do projeto: ``` [P4-ETAPA-04] Implementação Orientada a Objetos ```
 
## Visão Geral do Projeto
 
O projeto é uma versão simplificada, em terminal, de um simulador de redes no estilo do Cisco Packet Tracer. O usuário descreve uma rede (dispositivos, seus endereços IP e as conexões entre eles) e coloca pacotes em alguns dispositivos. O programa simula o envio desses pacotes pela rede e mostra, para cada um, até onde ele chegou e por quais dispositivos passou.
 
O mesmo problema é resolvido em vários paradigmas de programação (imperativo, orientado a objetos, funcional, lógico e integrado), para comparar como cada um modela a mesma solução.
 
A linguagem escolhida é C++, por ser a linguagem com que tenho mais familiaridade e por permitir escrever tanto no estilo imperativo quanto no orientado a objetos, o que facilita comparar os dois paradigmas.
 
## Descrição do Problema
 
A rede é formada por **dispositivos** (nós), identificados por um número de 1 a N. Cada dispositivo tem um **endereço IP** e pode estar conectado a outros dispositivos por **cabos**, que funcionam nos dois sentidos.
 
Cada **pacote** tem um nome e sai de um dispositivo de origem. A simulação acontece em passos: a cada passo, **todos os pacotes avançam ao mesmo tempo**, um dispositivo por vez, seguindo as regras abaixo.
 
1. **Mesma rede:** um pacote só passa por dispositivos com o mesmo IP do seu dispositivo de origem. O IP pertence ao dispositivo, não ao pacote.
2. **Roteamento:** o pacote segue para o vizinho de **maior identificador** entre os que têm o mesmo IP, sem voltar para o dispositivo de onde acabou de vir.
3. **Parada:** se não houver para onde ir, o pacote para no dispositivo atual.
4. **Sem repetição:** se o próximo dispositivo já estiver no caminho do pacote, ele é descartado e a saída dele vira `"Package cannot go through the same emitter twice"`.
5. **Colisão na entrada:** se dois pacotes começarem no mesmo dispositivo, a saída inteira é `"Package Collision"`.
6. **Colisão na simulação:** um dispositivo não pode ter dois pacotes ao mesmo tempo. Se isso acontecer ao fim de qualquer passo, a saída inteira é `"Package Collision"`. Um pacote parado continua ocupando seu dispositivo.
O contrato completo (formato de entrada, formato de saída, regras e os 15 casos de teste) está em [`testes/casos.md`](testes/casos.md).
 
### Exemplo
 
Entrada:
 
```
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
```
 
Saída:
 
```
"test" 3
1 2 3
"important" 4
6 5 4
```
 
Os dispositivos 1, 2 e 3 estão na rede 40, e os dispositivos 4, 5 e 6 na rede 50. Cada pacote percorre apenas a sua rede e para ao chegar ao limite dela.
 
## Evolução do Problema
 
A proposta da Etapa 1 previa um grafo direcionado com pesos, dispositivos do tipo *relay* que multiplicavam pacotes e retrocesso quando não houvesse saída. Ao escrever o contrato semântico na Etapa 2, o problema foi simplificado: os cabos passaram a funcionar nos dois sentidos, o menor peso foi trocado pelo vizinho de maior identificador, os relays e o retrocesso foram removidos, e foram adicionadas as regras de colisão e de descarte por repetição.
 
Com essas mudanças, cada entrada tem exatamente uma saída esperada, o que permite validar todas as implementações com os mesmos casos de teste.
 
## Andamento
 
| Etapa | Descrição | Status |
|---|---|---|
| `[P4-ETAPA-01]` | Proposta do problema | concluída |
| `[P4-ETAPA-02]` | Contrato semântico e testes | concluída |
| `[P4-ETAPA-03]` | Implementação imperativa | concluída |
| `[P4-ETAPA-04]` | Implementação orientada a objetos | em andamento |
 
## Estrutura do Projeto
 
```
projeto-p4-lucas-almeida-carrazzone/
│
├── README.md
│
├── docs/
│   ├── problema.md
│   ├── especificacao.md
│   ├── decisoes.md
│   └── comparacao-final.md
│
├── testes/
│   └── casos.md
│
├── imperativo/
│   ├── codigo.cpp
│   └── decisoes.md
│
├── poo/
│   ├── codigo.cpp
│   └── reflexão.md
│
├── funcional/
├── logico/
└── integrado/
```
