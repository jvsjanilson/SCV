#include "Passageiro.h"

Passageiro::Passageiro(const std::string& nome, Cidade* localAtual)
    : nome(nome), localAtual(localAtual), emViagem(false) {}

std::string Passageiro::getNome() const { return nome; }
Cidade* Passageiro::getLocalAtual() const { return localAtual; }
bool Passageiro::getEmViagem() const { return emViagem; }

void Passageiro::setLocalAtual(Cidade* local) {
    localAtual = local;
}

void Passageiro::setEmViagem(bool estado) {
    emViagem = estado;
}