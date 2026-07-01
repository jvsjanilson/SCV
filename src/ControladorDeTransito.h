#ifndef CONTROLADOR_H
#define CONTROLADOR_H
#include <vector>
#include <string>
#include <map>
#include "Cidade.h"
#include "Trajeto.h"
#include "Transporte.h"
#include "Passageiro.h"
#include "Viagem.h"

class ControladorDeTransito {
    private:
        std::vector<Cidade*> cidades;
        std::vector<Trajeto*> trajetos;
        std::vector<Transporte*> transportes;
        std::vector<Passageiro*> passageiros;
        std::vector<Viagem*> viagens;
        std::map<std::string, int> visitasCidades;

        Cidade* encontrarCidade(const std::string& nome);
        Transporte* encontrarTransporte(const std::string& nome);
        Passageiro* encontrarPassageiro(const std::string& nome);
        Trajeto* encontrarTrajeto(Cidade* origem, Cidade* destino, char tipoTransporte);
        std::vector<Trajeto*> encontrarMelhorCaminho(Cidade* origem, Cidade* destino, char tipoTransporte);

        bool salvarCidades();
        bool salvarTrajetos();
        bool salvarTransportes();
        bool salvarPassageiros();
        bool salvarViagens();
        
        bool carregarCidades();
        bool carregarTrajetos();
        bool carregarTransportes();
        bool carregarPassageiros();
        bool carregarViagens();
    public:
        ControladorDeTransito();
        ~ControladorDeTransito();

        void cadastrarCidade(const std::string& nome);
        void cadastrarTrajeto(const std::string& nomeOrigem, const std::string& nomeDestino, 
                         char tipo, int distancia);

        void cadastrarTransporte(const std::string& nome, char tipo, int capacidade,
                                int velocidade, int distancia_entre_descansos,
                                int tempo_de_descanso, const std::string& localAtual);
        void cadastrarPassageiro(const std::string& nome, const std::string& localAtual);
        
        bool iniciarViagem(const std::string& nomeTransporte, 
                        const std::vector<std::string>& nomesPassageiros,
                        const std::string& nomeOrigem, const std::string& nomeDestino);
        
        void avancarHoras(int horas);
        
        void relatarEstadoGeral() const;
        void relatarLocalizacaoPessoas() const;
        void relatarLocalizacaoTransportes() const;
        void relatarViagensAndamento() const;
        void relatarCidadesMaisVisitadas() const;
        bool carregarDados();
        bool salvarDados();
};

#endif