# projeto-p4-lucas-almeida-carrazzone

O problema escolhido foi o seguinte:

# Simulador de Rede de Computadores Simplificada

ALUNO: Lucas Almeida Carrazzone

### Visão Geral do Projeto

A linguagem de programação selecionada para resolver este problema é C++, por ser a linguagem na qual eu tenho mais familiaridade e que julgo mais adequada para este problema.

### Descrição do Problema

O problema consiste em um grafo direcionado composto por roteadores, relays e destinos finais, no qual uma string colocada na raiz do grafo deve percorrer o caminho de menor peso, passando apenas por nós que possuam o mesmo endereço IP — não podendo passar por nós com IPs diferentes.

Os roteadores recebem pacotes e os enviam para outros roteadores com o mesmo endereço; o peso representa a distância entre um roteador e outro, e deve ser possível visualizar pelo terminal todos os nós pelos quais o pacote passou. Caso o pacote chegue a um relay, ele deve ser espalhado para todos os nós aos quais o relay está conectado direcionalmente, ou seja, o pacote é multiplicado e enviado a todos os nós que possuem o mesmo endereço IP.

O endereço IP do pacote deve ser gerenciado pelos nós, e não pelo pacote em si: o nó deve ter uma variável que armazena seu próprio endereço. Antes de repassar o pacote, o nó deve confirmar se o próximo nó de menor peso possui o mesmo endereço; caso contrário, deve encaminhar o pacote ao próximo nó de menor peso com o mesmo IP. Se não houver nenhum, o algoritmo deve retroceder pelo caminho em busca de uma saída; e, se isso também for impossível, o programa deve imprimir no terminal que não foi possível entregar o pacote ao destino final.

### Estrutura do Projeto

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
├── poo/
├── funcional/
├── logico/
└── integrado/
```
