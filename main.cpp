#include <fstream>
#include <string>
#include <iostream>

#include "array_list.h"
#include "array_stack.h"
#include "parser.h"

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


void parse_cenario(StringReader& r) {

}

void parse(std::string& entrada) {
    StringReader r(entrada);

    auto cenarios = new structures::ArrayList<Cenario>(200);

    structures::ArrayStack<std::string> stack(100);

    while (r.peek()) {
        std::string contents = r.read_until('<');

        if (!r.peek()) break;
        StringReader::Tag tag = r.parse_tag();
        
        if (tag.is_closing) {
            std::string current = stack.pop();
            if (current != tag.key) {
                throw parse_error("Unclosed tag.");
            }
        } else {
            stack.push(tag.key);
        }
    }

    if (stack.size()) {
        throw parse_error("Unclosed tag.");
    }
}

int main() {
    std::string entrada = read_file();

    parse(entrada);

    return 0;
}
