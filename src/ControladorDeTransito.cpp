#include "ControladorDeTransito.h"
#include <iostream>
#include <algorithm>
#include <queue>
#include <limits>
#include <fstream>
#include <sstream>

ControladorDeTransito::ControladorDeTransito() {}

ControladorDeTransito::~ControladorDeTransito() {
    for (auto p : cidades) delete p;
    for (auto p : trajetos) delete p;
    for (auto p : transportes) delete p;
    for (auto p : passageiros) delete p;
    for (auto p : viagens) delete p;
}

Cidade* ControladorDeTransito::encontrarCidade(const std::string& nome) {
    for (Cidade* c : cidades) {
        if (c->getNome() == nome) {
            return c;
        }
    }
    return nullptr;
}

Transporte* ControladorDeTransito::encontrarTransporte(const std::string& nome) {
    for (Transporte* t : transportes) {
        if (t->getNome() == nome) {
            return t;
        }
    }
    return nullptr;
}

Passageiro* ControladorDeTransito::encontrarPassageiro(const std::string& nome) {
    for (Passageiro* p : passageiros) {
        if (p->getNome() == nome) {
            return p;
        }
    }
    return nullptr;
}

Trajeto* ControladorDeTransito::encontrarTrajeto(Cidade* origem, Cidade* destino, char tipoTransporte) {
    for (Trajeto* t : trajetos) {
        if (t->getOrigem() == origem && t->getDestino() == destino && 
            t->ehCompativel(tipoTransporte)) {
            return t;
        }
    }
    return nullptr;
}

std::vector<Trajeto*> ControladorDeTransito::encontrarMelhorCaminho(Cidade* origem, Cidade* destino, char tipoTransporte) {
    std::map<Cidade*, Cidade*> predecessor;
    std::map<Cidade*, Trajeto*> trajetoUsado;
    std::queue<Cidade*> fila;
    std::map<Cidade*, bool> visitado;
    
    fila.push(origem);
    visitado[origem] = true;
    
    while (!fila.empty()) {
        Cidade* atual = fila.front();
        fila.pop();
        
        if (atual == destino) {
            break;
        }
        
        for (Trajeto* t : trajetos) {
            if (t->getOrigem() == atual && t->ehCompativel(tipoTransporte)) {
                Cidade* proximo = t->getDestino();
                if (!visitado[proximo]) {
                    visitado[proximo] = true;
                    predecessor[proximo] = atual;
                    trajetoUsado[proximo] = t;
                    fila.push(proximo);
                }
            }
        }
    }
    
    std::vector<Trajeto*> caminho;
    if (visitado[destino]) {
        Cidade* atual = destino;
        while (atual != origem) {
            caminho.push_back(trajetoUsado[atual]);
            atual = predecessor[atual];
        }
        std::reverse(caminho.begin(), caminho.end());
    }
    
    return caminho;
}

void ControladorDeTransito::cadastrarCidade(const std::string& nome) {
    if (encontrarCidade(nome) != nullptr) {
        std::cout << "Erro: Cidade '" << nome << "' já cadastrada!" << std::endl;
        return;
    }
    
    Cidade* novaCidade = new Cidade(nome);
    cidades.push_back(novaCidade);
    visitasCidades[nome] = 0;
    std::cout << "Cidade '" << nome << "' cadastrada com sucesso!" << std::endl;
}

void ControladorDeTransito::cadastrarTrajeto(const std::string& nomeOrigem, 
                                            const std::string& nomeDestino,
                                            char tipo, int distancia) {
    Cidade* origem = encontrarCidade(nomeOrigem);
    Cidade* destino = encontrarCidade(nomeDestino);
    
    if (origem == nullptr) {
        std::cout << "Erro: Cidade de origem '" << nomeOrigem << "' não encontrada!" << std::endl;
        return;
    }
    
    if (destino == nullptr) {
        std::cout << "Erro: Cidade de destino '" << nomeDestino << "' não encontrada!" << std::endl;
        return;
    }
    
    if (origem == destino) {
        std::cout << "Erro: Origem e destino são a mesma cidade!" << std::endl;
        return;
    }
    
    if (encontrarTrajeto(origem, destino, tipo) != nullptr) {
        std::cout << "Erro: Trajeto já cadastrado entre " << nomeOrigem 
                  << " e " << nomeDestino << "!" << std::endl;
        return;
    }
    
    Trajeto* novoTrajeto = new Trajeto(origem, destino, tipo, distancia);
    trajetos.push_back(novoTrajeto);
    std::cout << "Trajeto cadastrado com sucesso!" << std::endl;
}

void ControladorDeTransito::cadastrarTransporte(const std::string& nome, char tipo,
                                               int capacidade, int velocidade,
                                               int distancia_entre_descansos,
                                               int tempo_de_descanso,
                                               const std::string& localAtual) {
    if (encontrarTransporte(nome) != nullptr) {
        std::cout << "Erro: Transporte '" << nome << "' já cadastrado!" << std::endl;
        return;
    }
    
    Cidade* local = encontrarCidade(localAtual);
    if (local == nullptr) {
        std::cout << "Erro: Cidade '" << localAtual << "' não encontrada!" << std::endl;
        return;
    }
    
    Transporte* novoTransporte = new Transporte(nome, tipo, capacidade, velocidade,
                                                distancia_entre_descansos, tempo_de_descanso,
                                                local);
    transportes.push_back(novoTransporte);
    std::cout << "Transporte '" << nome << "' cadastrado com sucesso!" << std::endl;
}

void ControladorDeTransito::cadastrarPassageiro(const std::string& nome, 
                                               const std::string& localAtual) {
    if (encontrarPassageiro(nome) != nullptr) {
        std::cout << "Erro: Passageiro '" << nome << "' já cadastrado!" << std::endl;
        return;
    }
    
    Cidade* local = encontrarCidade(localAtual);
    if (local == nullptr) {
        std::cout << "Erro: Cidade '" << localAtual << "' não encontrada!" << std::endl;
        return;
    }
    
    Passageiro* novoPassageiro = new Passageiro(nome, local);
    passageiros.push_back(novoPassageiro);
    std::cout << "Passageiro '" << nome << "' cadastrado com sucesso!" << std::endl;
}

bool ControladorDeTransito::iniciarViagem(const std::string& nomeTransporte,
                                         const std::vector<std::string>& nomesPassageiros,
                                         const std::string& nomeOrigem,
                                         const std::string& nomeDestino) {
    Transporte* transporte = encontrarTransporte(nomeTransporte);
    if (transporte == nullptr) {
        std::cout << "Erro: Transporte '" << nomeTransporte << "' não encontrado!" << std::endl;
        return false;
    }
    
    Cidade* origem = encontrarCidade(nomeOrigem);
    Cidade* destino = encontrarCidade(nomeDestino);
    
    if (origem == nullptr) {
        std::cout << "Erro: Cidade de origem '" << nomeOrigem << "' não encontrada!" << std::endl;
        return false;
    }
    
    if (destino == nullptr) {
        std::cout << "Erro: Cidade de destino '" << nomeDestino << "' não encontrada!" << std::endl;
        return false;
    }
    
    if (transporte->getLocalAtual() != origem) {
        std::cout << "Erro: Transporte não está em " << nomeOrigem << "!" << std::endl;
        return false;
    }
    
    if (transporte->getEmMovimento()) {
        std::cout << "Erro: Transporte já está em movimento!" << std::endl;
        return false;
    }
    
    std::vector<Passageiro*> passageirosViagem;
    for (const std::string& nome : nomesPassageiros) {
        Passageiro* p = encontrarPassageiro(nome);
        if (p == nullptr) {
            std::cout << "Erro: Passageiro '" << nome << "' não encontrado!" << std::endl;
            return false;
        }
        if (p->getLocalAtual() != origem) {
            std::cout << "Erro: Passageiro '" << nome << "' não está em " << nomeOrigem << "!" << std::endl;
            return false;
        }
        if (p->getEmViagem()) {
            std::cout << "Erro: Passageiro '" << nome << "' já está em viagem!" << std::endl;
            return false;
        }
        passageirosViagem.push_back(p);
    }
    
    if (static_cast<int>(passageirosViagem.size()) > transporte->getCapacidade()) {
        std::cout << "Erro: Capacidade do transporte excedida!" << std::endl;
        return false;
    }
    
    std::vector<Trajeto*> caminho = encontrarMelhorCaminho(origem, destino, transporte->getTipo());
    
    if (caminho.empty()) {
        std::cout << "Erro: Não há trajeto disponível entre " << nomeOrigem 
                  << " e " << nomeDestino << "!" << std::endl;
        return false;
    }
    
    Viagem* primeiraViagem = new Viagem(transporte, passageirosViagem, 
                                        origem, destino, caminho[0]);
    
    Viagem* viagemAtual = primeiraViagem;
    for (size_t i = 1; i < caminho.size(); i++) {
        Viagem* proxima = new Viagem(transporte, passageirosViagem,
                                     caminho[i-1]->getDestino(),
                                     (i == caminho.size() - 1) ? destino : caminho[i]->getDestino(),
                                     caminho[i]);
        viagemAtual->setProximaViagem(proxima);
        viagemAtual = proxima;
    }
    
    if (primeiraViagem->iniciarViagem()) {
        viagens.push_back(primeiraViagem);
        std::cout << "Viagem iniciada com sucesso!" << std::endl;
        return true;
    } else {
        delete primeiraViagem;
        std::cout << "Erro: Não foi possível iniciar a viagem!" << std::endl;
        return false;
    }
}

void ControladorDeTransito::avancarHoras(int horas) {
    std::cout << "Avançando " << horas << " horas..." << std::endl;
    
    for (Viagem* v : viagens) {
        if (v->getEmAndamento() && !v->getConcluida()) {
            bool concluida = v->avancarHoras(horas);
            if (concluida) {
                visitasCidades[v->getDestino()->getNome()]++;
                std::cout << "Viagem concluída: " << v->relatarEstado() << std::endl;
            }
        }
    }
}

void ControladorDeTransito::relatarEstadoGeral() const {
    std::cout << "\n=== RELATÓRIO GERAL DO SISTEMA ===" << std::endl;
    std::cout << "Cidades: " << cidades.size() << std::endl;
    std::cout << "Trajetos: " << trajetos.size() << std::endl;
    std::cout << "Transportes: " << transportes.size() << std::endl;
    std::cout << "Passageiros: " << passageiros.size() << std::endl;
    std::cout << "Viagens em andamento: ";
    int count = 0;
    for (const Viagem* v : viagens) {
        if (v->getEmAndamento()) count++;
    }
    std::cout << count << std::endl;
}

void ControladorDeTransito::relatarLocalizacaoPessoas() const {
    std::cout << "\n=== LOCALIZAÇÃO DOS PASSAGEIROS ===" << std::endl;
    for (const Passageiro* p : passageiros) {
        std::cout << p->getNome() << ": ";
        if (p->getEmViagem()) {
            for (const Viagem* v : viagens) {
                if (v->getEmAndamento()) {
                    auto passageirosViagem = v->getPassageiros();
                    if (std::find(passageirosViagem.begin(), passageirosViagem.end(), p) 
                        != passageirosViagem.end()) {
                        std::cout << "Em trânsito (" << v->getOrigem()->getNome() 
                                  << " -> " << v->getDestino()->getNome() 
                                  << " | Transporte: " << v->getTransporte()->getNome() 
                                  << ")" << std::endl;
                        break;
                    }
                }
            }
        } else {
            std::cout << "Em " << p->getLocalAtual()->getNome() << std::endl;
        }
    }
}

void ControladorDeTransito::relatarLocalizacaoTransportes() const {
    std::cout << "\n=== LOCALIZAÇÃO DOS TRANSPORTES ===" << std::endl;
    for (const Transporte* t : transportes) {
        std::cout << t->getNome() << " (" << t->getTipo() << "): ";
        if (t->getEmMovimento()) {
            for (const Viagem* v : viagens) {
                if (v->getEmAndamento() && v->getTransporte() == t) {
                    std::cout << "Em trânsito (" << v->getOrigem()->getNome() 
                              << " -> " << v->getDestino()->getNome() 
                              << " | " << (v->getHorasEmTransito() / v->calcularTempoTotal() * 100) 
                              << "% completo)" << std::endl;
                    break;
                }
            }
        } else {
            std::cout << "Em " << t->getLocalAtual()->getNome() << std::endl;
        }
    }
}

void ControladorDeTransito::relatarViagensAndamento() const {
    std::cout << "\n=== VIAGENS EM ANDAMENTO ===" << std::endl;
    bool found = false;
    for (const Viagem* v : viagens) {
        if (v->getEmAndamento()) {
            std::cout << v->relatarEstado() << std::endl;
            found = true;
        }
    }
    if (!found) {
        std::cout << "Nenhuma viagem em andamento." << std::endl;
    }
}

void ControladorDeTransito::relatarCidadesMaisVisitadas() const {
    std::cout << "\n=== CIDADES MAIS VISITADAS ===" << std::endl;
    
    std::vector<std::pair<std::string, int>> visitas;
    for (const auto& par : visitasCidades) {
        visitas.push_back(par);
    }
    
    std::sort(visitas.begin(), visitas.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });
    
    for (const auto& par : visitas) {
        std::cout << par.first << ": " << par.second << " visitas" << std::endl;
    }
}

bool ControladorDeTransito::salvarCidades() {
    std::string arquivo = "data/cidades.txt";
    std::ofstream out(arquivo);
    
    if (!out.is_open()) {
        std::cerr << "Erro ao abrir arquivo para salvar cidades: " << arquivo << std::endl;
        return false;
    }
    
    out << cidades.size() << std::endl;
    
    for (const Cidade* cidade : cidades) {
        out << cidade->getNome() << std::endl;
    }
    
    out.close();
    return true;
}

bool ControladorDeTransito::salvarTrajetos() {
    std::string arquivo = "data/trajetos.txt";
    std::ofstream out(arquivo);
    
    if (!out.is_open()) {
        std::cerr << "Erro ao abrir arquivo para salvar trajetos: " << arquivo << std::endl;
        return false;
    }
    
    out << trajetos.size() << std::endl;
    
    for (const Trajeto* trajeto : trajetos) {
        out << trajeto->getOrigem()->getNome() << ","
            << trajeto->getDestino()->getNome() << ","
            << trajeto->getTipo() << ","
            << trajeto->getDistancia() << std::endl;
    }
    
    out.close();
    return true;
}

bool ControladorDeTransito::salvarTransportes() {
    std::string arquivo = "data/transportes.txt";
    std::ofstream out(arquivo);
    
    if (!out.is_open()) {
        std::cerr << "Erro ao abrir arquivo para salvar transportes: " << arquivo << std::endl;
        return false;
    }
    
    out << transportes.size() << std::endl;
    
    for (const Transporte* transporte : transportes) {
        out << transporte->getNome() << ","
            << transporte->getTipo() << ","
            << transporte->getCapacidade() << ","
            << transporte->getVelocidade() << ","
            << transporte->getDistanciaEntreDescansos() << ","
            << transporte->getTempoDescanso() << ","
            << transporte->getLocalAtual()->getNome() << ","
            << transporte->getEmMovimento() << std::endl;
    }
    
    out.close();
    return true;
}

bool ControladorDeTransito::salvarPassageiros() {
    std::string arquivo = "data/passageiros.txt";
    std::ofstream out(arquivo);
    
    if (!out.is_open()) {
        std::cerr << "Erro ao abrir arquivo para salvar passageiros: " << arquivo << std::endl;
        return false;
    }
    
    out << passageiros.size() << std::endl;
    
    for (const Passageiro* passageiro : passageiros) {
        out << passageiro->getNome() << ","
            << passageiro->getLocalAtual()->getNome() << ","
            << passageiro->getEmViagem() << std::endl;
    }
    
    out.close();
    return true;
}

bool ControladorDeTransito::salvarViagens() {
    std::string arquivo = "data/viagens.txt";
    std::ofstream out(arquivo);
    
    if (!out.is_open()) {
        std::cerr << "Erro ao abrir arquivo para salvar viagens: " << arquivo << std::endl;
        return false;
    }
    
    std::vector<Viagem*> viagensParaSalvar;
    for (Viagem* v : viagens) {
        if (v->getConcluida() || v->getEmAndamento()) {
            viagensParaSalvar.push_back(v);
        }
    }
    
    out << viagensParaSalvar.size() << std::endl;
    
    for (const Viagem* viagem : viagensParaSalvar) {
        out << viagem->getTransporte()->getNome() << ","
            << viagem->getOrigem()->getNome() << ","
            << viagem->getDestino()->getNome() << ","
            << viagem->getHorasEmTransito() << ","
            << viagem->getEmAndamento() << ","
            << viagem->getConcluida() << std::endl;
        auto passageirosViagem = viagem->getPassageiros();
        out << passageirosViagem.size();
        for (const Passageiro* p : passageirosViagem) {
            out << "," << p->getNome();
        }
        out << std::endl;
        
        Viagem* atual = viagem->getProximaViagem();
        int conexoes = 0;
        while (atual != nullptr) {
            conexoes++;
            atual = atual->getProximaViagem();
        }
        
        out << conexoes << std::endl;

        out << conexoes << std::endl;
        atual = viagem->getProximaViagem();

        while (atual != nullptr) {
            out << atual->getOrigem()->getNome() << ","
                << atual->getDestino()->getNome() << ","
                << atual->getHorasEmTransito() << ","
                << atual->getEmAndamento() << ","
                << atual->getConcluida() << std::endl;
            atual = atual->getProximaViagem();
        }
    }
    
    out.close();
    return true;
}

bool ControladorDeTransito::carregarCidades() {
    std::string arquivo = "data/cidades.txt";
    std::ifstream in(arquivo);
    
    if (!in.is_open()) {
        std::cout << "Arquivo de cidades não encontrado. Criando novo..." << std::endl;
        return true;
    }
    
    int quantidade;
    in >> quantidade;
    in.ignore();
    
    for (int i = 0; i < quantidade; i++) {
        std::string nome;
        std::getline(in, nome);
        if (!nome.empty()) {
            Cidade* cidade = new Cidade(nome);
            cidades.push_back(cidade);
            visitasCidades[nome] = 0;
        }
    }
    
    in.close();
    std::cout << "Carregadas " << cidades.size() << " cidades." << std::endl;
    return true;
}


bool ControladorDeTransito::carregarTrajetos() {
    std::string arquivo = "data/trajetos.txt";
    std::ifstream in(arquivo);
    
    if (!in.is_open()) {
        std::cout << "Arquivo de trajetos não encontrado. Criando novo..." << std::endl;
        return true;
    }
    
    int quantidade;
    in >> quantidade;
    in.ignore();
    
    for (int i = 0; i < quantidade; i++) {
        std::string linha;
        std::getline(in, linha);
        
        std::stringstream ss(linha);
        std::string nomeOrigem, nomeDestino, tipoStr;
        int distancia;
        
        std::getline(ss, nomeOrigem, ',');
        std::getline(ss, nomeDestino, ',');
        std::getline(ss, tipoStr, ',');
        ss >> distancia;

          if (!nomeOrigem.empty() && !nomeDestino.empty()) {
            Cidade* origem = encontrarCidade(nomeOrigem);
            Cidade* destino = encontrarCidade(nomeDestino);
            
            if (origem && destino) {
                char tipo = tipoStr.empty() ? 'T' : tipoStr[0];
                Trajeto* trajeto = new Trajeto(origem, destino, tipo, distancia);
                trajetos.push_back(trajeto);
            }
        }
    }
    
    in.close();
    std::cout << "Carregados " << trajetos.size() << " trajetos." << std::endl;
    return true;
}

bool ControladorDeTransito::carregarTransportes() {
    std::string arquivo = "data/transportes.txt";
    std::ifstream in(arquivo);
    
    if (!in.is_open()) {
        std::cout << "Arquivo de transportes não encontrado. Criando novo..." << std::endl;
        return true;
    }
    
    int quantidade;
    in >> quantidade;
    in.ignore();
    
    for (int i = 0; i < quantidade; i++) {
        std::string linha;
        std::getline(in, linha);
        
        std::stringstream ss(linha);
        std::string nome, tipoStr, localAtualStr;
        std::string capacidadeStr, velocidadeStr, distDescansoStr, tempoDescansoStr;
        std::string emMovimentoStr;

        std::getline(ss, nome, ',');
        std::getline(ss, tipoStr, ',');
        std::getline(ss, capacidadeStr, ',');
        std::getline(ss, velocidadeStr, ',');
        std::getline(ss, distDescansoStr, ',');
        std::getline(ss, tempoDescansoStr, ',');
        std::getline(ss, localAtualStr, ',');
        std::getline(ss, emMovimentoStr, ',');
        
        if (!nome.empty()) {
            char tipo = tipoStr.empty() ? 'T' : tipoStr[0];
            int capacidade = std::stoi(capacidadeStr);
            int velocidade = std::stoi(velocidadeStr);
            int distDescanso = std::stoi(distDescansoStr);
            int tempoDescanso = std::stoi(tempoDescansoStr);
            
            Cidade* localAtual = encontrarCidade(localAtualStr);
            if (!localAtual && !cidades.empty()) {
                localAtual = cidades[0]; 
            }
             if (localAtual) {
                Transporte* transporte = new Transporte(nome, tipo, capacidade, 
                                                       velocidade, distDescanso, 
                                                       tempoDescanso, localAtual);
                bool emMovimento = (emMovimentoStr == "1");
                transporte->setEmMovimento(emMovimento);
                
                transportes.push_back(transporte);
            }
        }
    }
    
    in.close();
    std::cout << "Carregados " << transportes.size() << " transportes." << std::endl;
    return true;
}

bool ControladorDeTransito::carregarPassageiros() {
    std::string arquivo = "data/passageiros.txt";
    std::ifstream in(arquivo);
    
    if (!in.is_open()) {
        std::cout << "Arquivo de passageiros não encontrado. Criando novo..." << std::endl;
        return true;
    }
    
    int quantidade;
    in >> quantidade;
    in.ignore();
    
    for (int i = 0; i < quantidade; i++) {
        std::string linha;
        std::getline(in, linha);
        
        std::stringstream ss(linha);
        std::string nome, localAtualStr, emViagemStr;
        
        std::getline(ss, nome, ',');
        std::getline(ss, localAtualStr, ',');
        std::getline(ss, emViagemStr, ',');

        if (!nome.empty()) {
            Cidade* localAtual = encontrarCidade(localAtualStr);
            if (!localAtual && !cidades.empty()) {
                localAtual = cidades[0];
            }
            
            if (localAtual) {
                Passageiro* passageiro = new Passageiro(nome, localAtual);
                bool emViagem = (emViagemStr == "1");
                passageiro->setEmViagem(emViagem);
                
                passageiros.push_back(passageiro);
            }
        }
    }
    
    in.close();
    std::cout << "Carregados " << passageiros.size() << " passageiros." << std::endl;
    return true;
}

bool ControladorDeTransito::carregarViagens() {
    std::string arquivo = "data/viagens.txt";
    std::ifstream in(arquivo);
    
    if (!in.is_open()) {
        std::cout << "Arquivo de viagens não encontrado. Criando novo..." << std::endl;
        return true;
    }
    
    int quantidade;
    in >> quantidade;
    in.ignore();
    
    for (int i = 0; i < quantidade; i++) {
        std::string linha;
        std::getline(in, linha);
        
        std::stringstream ss(linha);
        std::string nomeTransporte, nomeOrigem, nomeDestino;
        std::string horasStr, emAndamentoStr, concluidaStr;

           std::getline(ss, nomeTransporte, ',');
        std::getline(ss, nomeOrigem, ',');
        std::getline(ss, nomeDestino, ',');
        std::getline(ss, horasStr, ',');
        std::getline(ss, emAndamentoStr, ',');
        std::getline(ss, concluidaStr, ',');
        
        Transporte* transporte = encontrarTransporte(nomeTransporte);
        Cidade* origem = encontrarCidade(nomeOrigem);
        Cidade* destino = encontrarCidade(nomeDestino);
        
        if (transporte && origem && destino) {
            std::string linhaPassageiros;
            std::getline(in, linhaPassageiros);
            
            std::vector<Passageiro*> passageirosViagem;
            std::stringstream ssPass(linhaPassageiros);
            std::string nomePass;
            int numPassageiros;
            ssPass >> numPassageiros;

            for (int j = 0; j < numPassageiros; j++) {
                std::getline(ssPass, nomePass, ',');
                if (!nomePass.empty()) {
                    Passageiro* p = encontrarPassageiro(nomePass);
                    if (p) passageirosViagem.push_back(p);
                }
            }
            
            Trajeto* trajeto = encontrarTrajeto(origem, destino, transporte->getTipo());
            if (!trajeto) {
                trajeto = new Trajeto(origem, destino, transporte->getTipo(), 100);
                trajetos.push_back(trajeto);
            }
            
            Viagem* viagem = new Viagem(transporte, passageirosViagem, origem, destino, trajeto);
            
            double horas = std::stod(horasStr);
            bool emAndamento = (emAndamentoStr == "1");
            bool concluida = (concluidaStr == "1");

            int numConexoes;
            in >> numConexoes;
            in.ignore();
            
            Viagem* atual = viagem;
            for (int j = 0; j < numConexoes; j++) {
                std::string linhaConexao;
                std::getline(in, linhaConexao);
                
                std::stringstream ssConex(linhaConexao);
                std::string origConex, destConex;
                std::string horasConex, emAndConex, conclConex;
                
                std::getline(ssConex, origConex, ',');
                std::getline(ssConex, destConex, ',');
                std::getline(ssConex, horasConex, ',');
                std::getline(ssConex, emAndConex, ',');
                std::getline(ssConex, conclConex, ',');
                
                Cidade* origC = encontrarCidade(origConex);
                Cidade* destC = encontrarCidade(destConex);
                 if (origC && destC) {
                    Trajeto* trajConex = encontrarTrajeto(origC, destC, transporte->getTipo());
                    if (!trajConex) {
                        trajConex = new Trajeto(origC, destC, transporte->getTipo(), 100);
                        trajetos.push_back(trajConex);
                    }
                    
                    Viagem* proxViagem = new Viagem(transporte, passageirosViagem, 
                                                   origC, destC, trajConex);
                    
                    if (conclConex == "1") {
                        proxViagem->setConcluida(true);
                    }
                    
                    atual->setProximaViagem(proxViagem);
                    atual = proxViagem;
                }
            }
             if (concluida) {
                viagem->setConcluida(true);
 
                for (Passageiro* p : passageirosViagem) {
                    p->setLocalAtual(destino);
                    p->setEmViagem(false);
                }
                transporte->setLocalAtual(destino);
                transporte->setEmMovimento(false);
            } else if (emAndamento) {
                viagem->iniciarViagem();
            }
            
            viagens.push_back(viagem);
            } else {
            std::string skip;
            std::getline(in, skip);
            int numConexoes;
            in >> numConexoes;
            in.ignore();
            for (int j = 0; j < numConexoes; j++) {
                std::getline(in, skip);
            }
        }
    }
    
    in.close();
    std::cout << "Carregadas " << viagens.size() << " viagens." << std::endl;
    return true;
}

bool ControladorDeTransito::carregarDados() {
    std::cout << "Carregando dados..." << std::endl;
    
    for (auto p : cidades) delete p;
    for (auto p : trajetos) delete p;
    for (auto p : transportes) delete p;
    for (auto p : passageiros) delete p;
    for (auto p : viagens) delete p;
    
    cidades.clear();
    trajetos.clear();
    transportes.clear();
    passageiros.clear();
    viagens.clear();
    visitasCidades.clear();

    bool sucesso = true;
    sucesso &= carregarCidades();
    sucesso &= carregarTrajetos();
    sucesso &= carregarTransportes();
    sucesso &= carregarPassageiros();
    sucesso &= carregarViagens();
    
    if (sucesso) {
        std::cout << "Dados carregados com sucesso!" << std::endl;
    } else {
        std::cout << "Erro ao carregar alguns dados!" << std::endl;
    }
    
    return sucesso;
}

bool ControladorDeTransito::salvarDados() {
    std::cout << "Salvando dados..." << std::endl;

    bool sucesso = true;
    sucesso &= salvarCidades();
    sucesso &= salvarTrajetos();
    sucesso &= salvarTransportes();
    sucesso &= salvarPassageiros();
    sucesso &= salvarViagens();
    
    if (sucesso) {
        std::cout << "Dados salvos com sucesso!" << std::endl;
    } else {
        std::cout << "Erro ao salvar alguns dados!" << std::endl;
    }
    
    return sucesso;
    
}