#ifndef TRANSPORTE_H
#define TRANSPORTE_H

#include <string>
#include "Cidade.h"

class Transporte {
    private:
        std::string nome;
        char tipo;
        int capacidade;
        int velocidade;
        int distancia_entre_descansos;
        int tempo_de_descanso;
        int tempo_de_descanso_atual;
        Cidade* localAtual;
        bool emMovimento;

    public:
        Transporte(const std::string& nome, char tipo, int capacidade, 
            int velocidade, int distancia_entre_descansos, int tempo_de_desconso,
            Cidade* localAtual);
        
        std::string getNome() const;
        char getTipo() const;
        int getCapacidade() const;
        int getVelocidade() const;
        int getDistanciaEntreDescansos() const;
        int getTempoDescanso() const;
        int getTempoDescansoAtual() const;
        Cidade* getLocalAtual() const;
        bool getEmMovimento() const;

        void setLocalAtual(Cidade* local);
        void setEmMovimento(bool estado);
        void adicionarTempoDescanso(int horas);
        void resetarTempoDescanso();

        double calcularTempoViagem(int distancia) const;

};


#endif