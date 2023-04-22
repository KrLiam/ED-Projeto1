#include <fstream>
#include <string>
#include <iostream>

#include "array_list.h"
#include "array_stack.h"
#include "parser.h"

using namespace structures;

std::string read_file() {
    char xmlfilename[100];

    std::cin >> xmlfilename;  // entrada
    
    std::ifstream t(xmlfilename);
    t.seekg(0, std::ios::end);
    size_t size = t.tellg();
    std::string buffer(size, ' ');
    t.seekg(0);
    t.read(&buffer[0], size);

    return buffer;
}

struct Cenario {
    std::string nome;
    int altura;
    int largura;
    int roboX;
    int roboY;
    bool* matriz;

    Cenario() {}

    Cenario(
        std::string nome,
        int altura,
        int largura,
        int roboX,
        int roboY,
        bool* matriz
    ) :
       nome(nome),
       altura(altura),
       largura(largura),
       roboX(roboX),
       roboY(roboY),
       matriz(matriz) {}
};


bool* parse_matriz(std::string& value) {
    StringReader r(value);

    return new bool[10];
}

ArrayList<Cenario>* parse(std::string& entrada) {
    StringReader r(entrada);

    auto cenarios = new ArrayList<Cenario>(200);
    Cenario cenario;

    ArrayStack<std::string> stack(100);

    while (r.peek()) {
        std::string contents = r.read_until('<');

        if (!r.peek()) break;
        StringReader::Tag tag = r.parse_tag();
        
        if (!tag.is_closing) {
            // opening
            stack.push(tag.key);

            if (tag.key == "cenario") {
                cenario = Cenario();
            }
        } else {
            // closing
            std::string current = stack.pop();
            if (current != tag.key) {
                throw parse_error("Unclosed tag.");
            }

            if (tag.key == "cenario") {
                cenarios->push_back(cenario);
            }
            else if (tag.key == "nome") {
                cenario.nome = contents;
            }
            else if (tag.key == "x") {
                cenario.roboX = std::stoi(contents);
            }
            else if (tag.key == "y") {
                cenario.roboX = std::stoi(contents);
            }
            else if (tag.key == "largura") {
                cenario.largura = std::stoi(contents);
            }
            else if (tag.key == "altura") {
                cenario.altura = std::stoi(contents);
            }
            else if (tag.key == "matriz") {
                cenario.matriz = parse_matriz(contents);
            }
        }
    }

    if (stack.size()) {
        throw parse_error("Unclosed tag.");
    }

    return cenarios;
}

int main() {
    std::string entrada = read_file();

    ArrayList<Cenario>* cenarios = parse(entrada);

    return 0;
}
