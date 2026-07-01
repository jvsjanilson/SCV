#include "Cidade.h"

Cidade::Cidade(const std::string& nome) : nome(nome) {}

std::string Cidade::getNome() const {
    return nome;
}

