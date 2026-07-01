#include "Transporte.h"
#include <cmath>

Transporte::Transporte(const std::string& nome, char tipo, int capacidade,
                       int velocidade, int distancia_entre_descansos,
                       int tempo_de_descanso, Cidade* localAtual)
    : nome(nome), tipo(tipo), capacidade(capacidade), velocidade(velocidade),
      distancia_entre_descansos(distancia_entre_descansos),
      tempo_de_descanso(tempo_de_descanso), tempo_de_descanso_atual(0),
      localAtual(localAtual), emMovimento(false) {}

std::string Transporte::getNome() const { return nome; }
char Transporte::getTipo() const { return tipo; }
int Transporte::getCapacidade() const { return capacidade; }
int Transporte::getVelocidade() const { return velocidade; }
int Transporte::getDistanciaEntreDescansos() const { return distancia_entre_descansos; }
int Transporte::getTempoDescanso() const { return tempo_de_descanso; }
int Transporte::getTempoDescansoAtual() const { return tempo_de_descanso_atual; }
Cidade* Transporte::getLocalAtual() const { return localAtual; }
bool Transporte::getEmMovimento() const { return emMovimento; }

void Transporte::setLocalAtual(Cidade* local) {
    localAtual = local;
}

void Transporte::setEmMovimento(bool estado) {
    emMovimento = estado;
}

void Transporte::adicionarTempoDescanso(int horas) {
    tempo_de_descanso_atual += horas;
}

void Transporte::resetarTempoDescanso() {
    tempo_de_descanso_atual = 0;
}

double Transporte::calcularTempoViagem(int distancia) const {
    if (velocidade <= 0) return 0;
    
    double tempoViagem = static_cast<double>(distancia) / velocidade;
    

    if (distancia_entre_descansos > 0) {
        int paradas = distancia / distancia_entre_descansos;
        if (distancia % distancia_entre_descansos == 0 && paradas > 0) {
            paradas--;
        }
        tempoViagem += paradas * tempo_de_descanso;
    }
    
    return tempoViagem;
}