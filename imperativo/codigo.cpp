#include <bits/stdc++.h>
using namespace std;

int n, m, p;
vector<int> ip;
vector<vector<int>> adj;

vector<string> nome;
vector<vector<int>> caminho;
vector<int> anterior;
vector<bool> ativo, descartado;

void Entrada() {
    cin >> n >> m;
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

    cin >> p;
    nome.resize(p);
    caminho.resize(p);
    anterior.resize(p);
    ativo.resize(p, true);
    descartado.resize(p, false);
    for (int i = 0; i < p; i++) {
        int origem;
        cin >> nome[i] >> origem;
        caminho[i].push_back(origem);
    }
}

// Escolhe o vizinho de maior ID com o mesmo IP, ignorando o nó de onde veio, retorna 0 se não houver
int Next(int atual, int veioDe, int rede) {
    int melhor = 0;
    for (int i = 0; i < (int)adj[atual].size(); i++) {
        int v = adj[atual][i];
        if (v != veioDe && ip[v] == rede && v > melhor) {
            melhor = v;
        }
    }
    return melhor;
}

// Pacotes em movimento e parados ocupam um nó e apenas os descartados deixam de ocupar
bool Collision() {
    vector<int> ocupado(n + 1, 0);
    for (int i = 0; i < p; i++) {
        if (descartado[i]) {
            continue;
        }
        int no = caminho[i].back();
        ocupado[no]++;
        if (ocupado[no] > 1) {
            return true;
        }
    }
    return false;
}

// Avança todos os pacotes ativos em um nó ao mesmo tempo e retorna se algum pacote se moveu
bool Step() {
    bool moved = false;
    for (int i = 0; i < p; i++) {
        if (!ativo[i]) {
            continue;
        }

        int atual = caminho[i].back();
        int prox = Next(atual, anterior[i], ip[caminho[i][0]]);

        if (prox == 0) {
            ativo[i] = false;
            continue;
        }

        bool repetido = false;
        for (int j = 0; j < (int)caminho[i].size(); j++) {
            if (caminho[i][j] == prox) {
                repetido = true;
            }
        }
        if (repetido) {
            ativo[i] = false;
            descartado[i] = true;
            continue;
        }

        anterior[i] = atual;
        caminho[i].push_back(prox);
        moved = true;
    }
    return moved;
}

int main() {
    Entrada();

    if (Collision()) {
        cout << "\"Package Collision\"\n";
        return 0;
    }
    while (Step()) {
        if (Collision()) {
            cout << "\"Package Collision\"\n";
            return 0;
        }
    }

    for (int i = 0; i < p; i++) {
        if (descartado[i]) {
            cout << "\"Package cannot go through the same emitter twice\"\n";
            continue;
        }
        cout << nome[i] << " " << caminho[i].back() << "\n";
        for (int j = 0; j < (int)caminho[i].size(); j++) {
            cout << caminho[i][j] << (j + 1 < (int)caminho[i].size() ? " " : "\n");
        }
    }
    return 0;
}
