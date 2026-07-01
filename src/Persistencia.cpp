#include "Persistencia.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

const std::string Persistencia::DIR_DADOS = "data/";

std::string Persistencia::getCaminhoArquivo(const std::string& nomeArquivo) {
    return DIR_DADOS + nomeArquivo;
}

bool Persistencia::salvarDados(ControladorDeTransito& controlador) {

    try {
        std::filesystem::create_directories(DIR_DADOS);
    } catch (const std::exception& e) {
        std::cerr << "Erro ao criar diretório: " << e.what() << std::endl;
        return false;
    }
    
    // TODO: Implementar salvamento dos dados
    
    std::cout << "Dados salvos com sucesso!" << std::endl;
    return true;
}

bool Persistencia::carregarDados(ControladorDeTransito& controlador) {
    // TODO: Implementar carregamento dos dados
    
    std::cout << "Dados carregados com sucesso!" << std::endl;
    return true;
}