#include "Viagem.h"
#include <iostream>
#include <sstream>
#include <cmath>

Viagem::Viagem(Transporte* transporte, const std::vector<Passageiro*>& passageiros,
               Cidade* origem, Cidade* destino, Trajeto* trajeto)
    : transporte(transporte), passageiros(passageiros), origem(origem),
      destino(destino), trajeto(trajeto), proximaViagem(nullptr),
      horasEmTransito(0), distanciaPercorrida(0), 
      emAndamento(false), concluida(false) {}

Viagem::~Viagem() {

}

Transporte* Viagem::getTransporte() const { return transporte; }
std::vector<Passageiro*> Viagem::getPassageiros() const { return passageiros; }
Cidade* Viagem::getOrigem() const { return origem; }
Cidade* Viagem::getDestino() const { return destino; }
Trajeto* Viagem::getTrajeto() const { return trajeto; }
Viagem* Viagem::getProximaViagem() const { return proximaViagem; }
double Viagem::getHorasEmTransito() const { return horasEmTransito; }
bool Viagem::getEmAndamento() const { return emAndamento; }
bool Viagem::getConcluida() const { return concluida; }

void Viagem::setProximaViagem(Viagem* viagem) {
    proximaViagem = viagem;
}

void Viagem::setConcluida(bool estado) {
    concluida = estado;
}

bool Viagem::podeIniciar() const {
    if (transporte->getLocalAtual() != origem) {
        return false;
    }
    
    if (static_cast<int>(passageiros.size()) > transporte->getCapacidade()) {
        return false;
    }
    
    if (!trajeto->ehCompativel(transporte->getTipo())) {
        return false;
    }
    
    for (Passageiro* p : passageiros) {
        if (p->getLocalAtual() != origem) {
            return false;
        }
    }
    
    return true;
}

double Viagem::calcularTempoTotal() const {
    return transporte->calcularTempoViagem(trajeto->getDistancia());
}

bool Viagem::iniciarViagem() {
    if (!podeIniciar()) {
        return false;
    }
    
    emAndamento = true;
    horasEmTransito = 0;
    distanciaPercorrida = 0;
    transporte->setEmMovimento(true);
    
    for (Passageiro* p : passageiros) {
        p->setEmViagem(true);
    }
    
    return true;
}

bool Viagem::avancarHoras(double horas) {
    if (!emAndamento || concluida) {
        return false;
    }
    
    horasEmTransito += horas;
    double tempoTotal = calcularTempoTotal();
    
    double distanciaEmHoras = transporte->getVelocidade() * horas;
    distanciaPercorrida += distanciaEmHoras;
    
    if (horasEmTransito >= tempoTotal) {
        concluida = true;
        emAndamento = false;
        transporte->setEmMovimento(false);
        
        for (Passageiro* p : passageiros) {
            p->setLocalAtual(destino);
            p->setEmViagem(false);
        }
        
        transporte->setLocalAtual(destino);
        
        if (proximaViagem != nullptr && proximaViagem->podeIniciar()) {
            proximaViagem->iniciarViagem();
        }
        
        return true;
    }
    
    return false;
}

std::string Viagem::relatarEstado() const {
    std::stringstream ss;
    
    if (concluida) {
        ss << "Viagem concluída de " << origem->getNome() 
           << " para " << destino->getNome();
    } else if (emAndamento) {
        ss << "Viagem em andamento: " << origem->getNome() 
           << " -> " << destino->getNome()
           << " | Transporte: " << transporte->getNome()
           << " | Progresso: " << (horasEmTransito / calcularTempoTotal() * 100)
           << "% | Passageiros: " << passageiros.size();
        
        if (proximaViagem != nullptr) {
            ss << " (com conexão)";
        }
    } else {
        ss << "Viagem não iniciada: " << origem->getNome() 
           << " -> " << destino->getNome();
    }
    
    return ss.str();
}