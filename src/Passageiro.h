#ifndef PASSAGEIRO_H
#define PASSAGEIRO_H

#include <string>
#include "Cidade.h"

class Passageiro
{
    private:
        std::string nome;
        Cidade* localAtual;
        bool emViagem;
    public:
        Passageiro(const std::string nome, Cidade* localAtual);
    
        std::string getNome() const;
        Cidade* getLocalAtual() const;
        bool getEmViagem() const;

        void setLocalAtual(Cidade* local);
        void setEmViagem(bool estado);
};

#endif