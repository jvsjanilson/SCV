#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <limits>
#include "ControladorDeTransito.h"
#include "Persistencia.h"

using namespace std;

void exibirMenu() {
    cout << "\n=== SISTEMA CONTROLADOR DE VIAGENS ===" << endl;
    cout << "1. Cadastrar Cidade" << endl;
    cout << "2. Cadastrar Trajeto" << endl;
    cout << "3. Cadastrar Transporte" << endl;
    cout << "4. Cadastrar Passageiro" << endl;
    cout << "5. Iniciar Viagem" << endl;
    cout << "6. Avançar Horas" << endl;
    cout << "7. Relatório Geral" << endl;
    cout << "8. Localização dos Passageiros" << endl;
    cout << "9. Localização dos Transportes" << endl;
    cout << "10. Viagens em Andamento" << endl;
    cout << "11. Cidades Mais Visitadas" << endl;
    cout << "12. Salvar Dados" << endl;
    cout << "13. Carregar Dados" << endl;
    cout << "0. Sair" << endl;
    cout << "Escolha uma opção: ";
}

void limparBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

string obterString(const string& mensagem) {
    string valor;
    cout << mensagem;
    getline(cin, valor);
    return valor;
}

char obterChar(const string& mensagem) {
    string valor;
    cout << mensagem;
    getline(cin, valor);
    return valor.empty() ? 'T' : toupper(valor[0]);
}

int obterInteiro(const string& mensagem) {
    int valor;
    cout << mensagem;
    cin >> valor;
    limparBuffer();
    return valor;
}

int main() {
    ControladorDeTransito controlador;
    int opcao;
    
    cout << "Bem-vindo ao Sistema Controlador de Viagens!" << endl;
    cout << "Carregando dados salvos..." << endl;
    controlador.carregarDados();
    
    do {
        exibirMenu();
        cin >> opcao;
        limparBuffer();
        
        switch (opcao) {
            case 1: {
                // Cadastrar Cidade
                string nome = obterString("Nome da cidade: ");
                controlador.cadastrarCidade(nome);
                break;
            }
            
            case 2: {
                // Cadastrar Trajeto
                string origem = obterString("Cidade de origem: ");
                string destino = obterString("Cidade de destino: ");
                char tipo = obterChar("Tipo (A - Aquático, T - Terrestre): ");
                int distancia = obterInteiro("Distância (km): ");
                controlador.cadastrarTrajeto(origem, destino, tipo, distancia);
                break;
            }
            
            case 3: {
                // Cadastrar Transporte
                string nome = obterString("Nome do transporte: ");
                char tipo = obterChar("Tipo (A - Aquático, T - Terrestre): ");
                int capacidade = obterInteiro("Capacidade de passageiros: ");
                int velocidade = obterInteiro("Velocidade (km/h): ");
                int distDescanso = obterInteiro("Distância entre descansos (km): ");
                int tempoDescanso = obterInteiro("Tempo de descanso (horas): ");
                string local = obterString("Local atual (cidade): ");
                controlador.cadastrarTransporte(nome, tipo, capacidade, velocidade,
                                               distDescanso, tempoDescanso, local);
                break;
            }
            
            case 4: {
                // Cadastrar Passageiro
                string nome = obterString("Nome do passageiro: ");
                string local = obterString("Local atual (cidade): ");
                controlador.cadastrarPassageiro(nome, local);
                break;
            }
            
            case 5: {
                // Iniciar Viagem
                string transporte = obterString("Nome do transporte: ");
                string origem = obterString("Cidade de origem: ");
                string destino = obterString("Cidade de destino: ");
                
                cout << "Passageiros (digite os nomes separados por vírgula): ";
                string linhaPassageiros;
                getline(cin, linhaPassageiros);
                
                vector<string> passageiros;
                stringstream ss(linhaPassageiros);
                string nome;
                while (getline(ss, nome, ',')) {
                    nome.erase(0, nome.find_first_not_of(" \t"));
                    nome.erase(nome.find_last_not_of(" \t") + 1);
                    if (!nome.empty()) {
                        passageiros.push_back(nome);
                    }
                }
                
                controlador.iniciarViagem(transporte, passageiros, origem, destino);
                break;
            }
            
            case 6: {
                // Avançar Horas
                int horas = obterInteiro("Quantas horas avançar? ");
                controlador.avancarHoras(horas);
                break;
            }
            
            case 7:
                // Relatório Geral
                controlador.relatarEstadoGeral();
                break;
                
            case 8:
                // Localização dos Passageiros
                controlador.relatarLocalizacaoPessoas();
                break;
                
            case 9:
                // Localização dos Transportes
                controlador.relatarLocalizacaoTransportes();
                break;
                
            case 10:
                // Viagens em Andamento
                controlador.relatarViagensAndamento();
                break;
                
            case 11:
                // Cidades Mais Visitadas
                controlador.relatarCidadesMaisVisitadas();
                break;
                
            case 12:
                // Salvar Dados
                controlador.salvarDados();
                break;
                
            case 13:
                // Carregar Dados
                controlador.carregarDados();
                break;
                
            case 0:
                cout << "Saindo do sistema..." << endl;
                controlador.salvarDados();
                break;
                
            default:
                cout << "Opção inválida!" << endl;
                break;
        }
        
    } while (opcao != 0);
    
    return 0;
}