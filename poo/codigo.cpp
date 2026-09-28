#include <bits/stdc++.h>
using namespace std;
 
class Rede {
    vector<int> ip;
    vector<vector<int>> adj;
 
public:
    void Entrada(int n) {
        ip.resize(n + 1);
        adj.resize(n + 1);
        for (int i = 0; i < n; i++) {
            int id, endereco, vizinho;
            cin >> id >> endereco >> vizinho;
            ip[id] = endereco;
            if (vizinho != id) {
                adj[id].push_back(vizinho);
                adj[vizinho].push_back(id);
            }
        }
    }
 
    int getIp(int no) const { return ip[no]; }
    const vector<int>& getVizinhos(int no) const { return adj[no]; }
    int getTamanho() const { return (int)ip.size() - 1; }
};
 
// Interface da regra de roteamento: trocar a regra não exige mudar Pacote nem Simulador
class Roteamento {
public:
    virtual ~Roteamento() {}
    virtual int Next(const Rede& rede, int atual, int veioDe, int ipPacote) const = 0;
};
 
class MaiorIdMesmoIp : public Roteamento {
public:
    int Next(const Rede& rede, int atual, int veioDe, int ipPacote) const override {
        const vector<int>& vizinhos = rede.getVizinhos(atual);
        int melhor = 0;
        for (int i = 0; i < (int)vizinhos.size(); i++) {
            int v = vizinhos[i];
            if (v != veioDe && rede.getIp(v) == ipPacote && v > melhor) {
                melhor = v;
            }
        }
        return melhor;
    }
};
 
class Pacote {
    string nome;
    vector<int> caminho;
    int anterior;
    bool ativo;
    bool descartado;
 
public:
    Pacote(string nome, int origem) : nome(nome), anterior(0), ativo(true), descartado(false) {
        caminho.push_back(origem);
    }
 
    int getPosicao() const { return caminho.back(); }
    bool estaDescartado() const { return descartado; }
 
    bool Step(const Rede& rede, const Roteamento& roteamento) {
        if (!ativo) {
            return false;
        }
 
        int atual = caminho.back();
        int prox = roteamento.Next(rede, atual, anterior, rede.getIp(caminho[0]));
 
        if (prox == 0) {
            ativo = false;
            return false;
        }
 
        bool repetido = false;
        for (int j = 0; j < (int)caminho.size(); j++) {
            if (caminho[j] == prox) {
                repetido = true;
            }
        }
        if (repetido) {
            ativo = false;
            descartado = true;
            return false;
        }
 
        anterior = atual;
        caminho.push_back(prox);
        return true;
    }
 
    void Imprimir() const {
        if (descartado) {
            cout << "\"Package cannot go through the same emitter twice\"\n";
            return;
        }
        cout << nome << " " << caminho.back() << "\n";
        for (int j = 0; j < (int)caminho.size(); j++) {
            cout << caminho[j] << (j + 1 < (int)caminho.size() ? " " : "\n");
        }
    }
};
 
class Simulador {
    Rede rede;
    vector<Pacote> pacotes;
    const Roteamento& roteamento;
 
    bool Collision() const {
        vector<int> ocupado(rede.getTamanho() + 1, 0);
        for (int i = 0; i < (int)pacotes.size(); i++) {
            if (pacotes[i].estaDescartado()) {
                continue;
            }
            int no = pacotes[i].getPosicao();
            ocupado[no]++;
            if (ocupado[no] > 1) {
                return true;
            }
        }
        return false;
    }
 
    bool Step() {
        bool moved = false;
        for (int i = 0; i < (int)pacotes.size(); i++) {
            if (pacotes[i].Step(rede, roteamento)) {
                moved = true;
            }
        }
        return moved;
    }
 
public:
    Simulador(const Roteamento& roteamento) : roteamento(roteamento) {}
 
    void Entrada() {
        int n, m, p;
        cin >> n >> m;
        rede.Entrada(n);
 
        cin >> p;
        for (int i = 0; i < p; i++) {
            string nome;
            int origem;
            cin >> nome >> origem;
            pacotes.push_back(Pacote(nome, origem));
        }
    }
 
    void Executar() {
        if (Collision()) {
            cout << "\"Package Collision\"\n";
            return;
        }
        while (Step()) {
            if (Collision()) {
                cout << "\"Package Collision\"\n";
                return;
            }
        }
        for (int i = 0; i < (int)pacotes.size(); i++) {
            pacotes[i].Imprimir();
        }
    }
};
 
int main() {
    MaiorIdMesmoIp regra;
    Simulador simulador(regra);
    simulador.Entrada();
    simulador.Executar();
    return 0;
}
 


