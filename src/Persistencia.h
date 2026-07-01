#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include <string>
#include <vector>
#include "ControladorDeTransito.h"

class Persistencia {
    private:
        static const std::string DIR_DADOS;
        
        static std::string getCaminhoArquivo(const std::string& nomeArquivo);
        
    public:
        static bool salvarDados(ControladorDeTransito& controlador);
        static bool carregarDados(ControladorDeTransito& controlador);
};

#endif