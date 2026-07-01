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

bool ControladorDeTransito::carregarDados() {
    std::cout << "Carregando dados..." << std::endl;
    // TODO: Implementar leitura dos arquivos
    return true;
}

bool ControladorDeTransito::salvarDados() {
    // Implementação da persistência
    std::cout << "Salvando dados..." << std::endl;
    // TODO: Implementar escrita nos arquivos
    return true;
}