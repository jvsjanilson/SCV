#ifndef VIAGEM_H
#define VIAGEM_H

#include <vector>
#include <string>
#include "Transporte.h"
#include "Passageiro.h"
#include "Cidade.h"
#include "Trajeto.h"

class Viagem
{
    private:
        Transporte* transporte;
        std::vector<Passageiro*> passageiros;
        Cidade* origem;
        Cidade* destino;
        Trajeto* trajeto;
        Viagem* proximaViagem;
        double horasEmTransito;
        double distanciaPercorrida;
        bool emAndamento;
        bool concluida;

    public:
        Viagem(Transporte*  transporte, const std::vector<Passageiro*>& passageiros, 
        Cidade* origem, Cidade* destino, Trajeto* trajeto );
        ~Viagem();

    Transporte* getTransporte() const;
    std::vector<Passageiro*> getPassageiros() const;
    Cidade* getOrigem() const;
    Cidade* getDestino() const;
    Trajeto* getTrajeto() const;
    Viagem* getProximaViagem() const;
    
    double getHorasEmTransito() const;
    bool getEmAndamento() const;
    bool getConcluida() const;

    void setProximaViagem(Viagem* viagem);
    void setConcluida(bool estado);

    bool iniciarViagem();
    bool avancarHoras(double horas);
    bool podeIniciar() const;

    double calcularTempoTotal() const;

    std::string relatarEstado() const;
};

#endif