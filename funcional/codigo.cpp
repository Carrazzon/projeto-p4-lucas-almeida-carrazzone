
#include <bits/stdc++.h>
using namespace std;

enum class Situacao { Ativo, Parado, Descartado };

struct Rede {
    vector<int> ip;
    vector<vector<int>> adj;
};

struct Pacote {
    string nome;
    vector<int> caminho;
    int anterior;
    Situacao situacao;
};

struct LinhaNo {
    int id, ip, vizinho;
};

struct Entrada {
    int n;
    vector<LinhaNo> nos;
    vector<Pacote> pacotes;
};

using Roteamento = function<int(const Rede&, int, int, int)>;

Rede montarRede(const Entrada& e) {
    Rede vazia{vector<int>(e.n + 1), vector<vector<int>>(e.n + 1)};
    return accumulate(e.nos.begin(), e.nos.end(), vazia, [](Rede r, const LinhaNo& l) {
        r.ip[l.id] = l.ip;
        if (l.vizinho != l.id) {
            r.adj[l.id].push_back(l.vizinho);
            r.adj[l.vizinho].push_back(l.id);
        }
        return r;
    });
}

// Escolhe o vizinho de maior ID com o mesmo IP, ignorando o nó de onde veio, retorna 0 se não houver
const Roteamento maiorIdMesmoIp = [](const Rede& rede, int atual, int veioDe, int ipRede) {
    const vector<int>& viz = rede.adj[atual];
    return accumulate(viz.begin(), viz.end(), 0, [&](int melhor, int v) {
        return (v != veioDe && rede.ip[v] == ipRede && v > melhor) ? v : melhor;
    });
};

int posicao(const Pacote& p) {
    return p.caminho.back();
}

// Devolve um novo pacote após um passo, o pacote recebido não é alterado
Pacote avancar(const Rede& rede, const Roteamento& rota, const Pacote& p) {
    if (p.situacao != Situacao::Ativo) return p;

    int prox = rota(rede, posicao(p), p.anterior, rede.ip[p.caminho[0]]);
    if (prox == 0) return {p.nome, p.caminho, p.anterior, Situacao::Parado};

    bool repetido = find(p.caminho.begin(), p.caminho.end(), prox) != p.caminho.end();
    if (repetido) return {p.nome, p.caminho, p.anterior, Situacao::Descartado};

    vector<int> novoCaminho = p.caminho;
    novoCaminho.push_back(prox);
    return {p.nome, novoCaminho, posicao(p), Situacao::Ativo};
}

vector<Pacote> passo(const Rede& rede, const Roteamento& rota, const vector<Pacote>& pacotes) {
    vector<Pacote> novos;
    transform(pacotes.begin(), pacotes.end(), back_inserter(novos),
              [&](const Pacote& p) { return avancar(rede, rota, p); });
    return novos;
}

vector<int> posicoesOcupadas(const vector<Pacote>& pacotes) {
    vector<Pacote> presentes;
    copy_if(pacotes.begin(), pacotes.end(), back_inserter(presentes),
            [](const Pacote& p) { return p.situacao != Situacao::Descartado; });
    vector<int> posicoes;
    transform(presentes.begin(), presentes.end(), back_inserter(posicoes), posicao);
    return posicoes;
}

bool temRepetido(vector<int> valores) {
    sort(valores.begin(), valores.end());
    return adjacent_find(valores.begin(), valores.end()) != valores.end();
}

bool temColisao(const vector<Pacote>& pacotes) {
    return temRepetido(posicoesOcupadas(pacotes));
}

// Retorna os pacotes no estado final, ou nullopt se houver colisão
optional<vector<Pacote>> simular(const Rede& rede, const Roteamento& rota, const vector<Pacote>& pacotes) {
    if (temColisao(pacotes)) return nullopt;
    bool algumAtivo = any_of(pacotes.begin(), pacotes.end(),
                             [](const Pacote& p) { return p.situacao == Situacao::Ativo; });
    if (!algumAtivo) return pacotes;
    return simular(rede, rota, passo(rede, rota, pacotes));
}

string formatarPacote(const Pacote& p) {
    if (p.situacao == Situacao::Descartado) {
        return "\"Package cannot go through the same emitter twice\"\n";
    }
    string caminho = accumulate(p.caminho.begin() + 1, p.caminho.end(), to_string(p.caminho[0]),
                                [](const string& acc, int no) { return acc + " " + to_string(no); });
    return p.nome + " " + to_string(posicao(p)) + "\n" + caminho + "\n";
}

string formatarResultado(const optional<vector<Pacote>>& resultado) {
    if (!resultado) return "\"Package Collision\"\n";
    return accumulate(resultado->begin(), resultado->end(), string(),
                      [](const string& acc, const Pacote& p) { return acc + formatarPacote(p); });
}

string resolver(const Entrada& e, const Roteamento& rota) {
    return formatarResultado(simular(montarRede(e), rota, e.pacotes));
}

// Única parte impura junto com a main: lê do mundo externo
Entrada lerEntrada() {
    int n, m, p;
    cin >> n >> m;
    vector<LinhaNo> nos(n);
    for (int i = 0; i < n; i++) {
        cin >> nos[i].id >> nos[i].ip >> nos[i].vizinho;
    }
    cin >> p;
    vector<Pacote> pacotes(p);
    for (int i = 0; i < p; i++) {
        string nome;
        int origem;
        cin >> nome >> origem;
        pacotes[i] = {nome, {origem}, 0, Situacao::Ativo};
    }
    return {n, nos, pacotes};
}

int main() {
    const Entrada entrada = lerEntrada();
    cout << resolver(entrada, maiorIdMesmoIp);
    return 0;
}
