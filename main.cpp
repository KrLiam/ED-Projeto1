#include <fstream>
#include <string>
#include <iostream>
#include <memory>

#include "array_list.h"
#include "array_stack.h"
#include "parser.h"

using namespace structures;

std::string read_file(char* xmlfilename) {    
    std::ifstream t(xmlfilename);
    t.seekg(0, std::ios::end);
    size_t size = t.tellg();
    std::string buffer(size, ' ');
    t.seekg(0);
    t.read(&buffer[0], size);

    return buffer;
}

struct Position {
    short i, j;

    Position() : i(0), j(0) {}
    Position(int i, int j) : i(i), j(j) {}

    bool operator==(Position p) {
        return i == p.i && j == p.j;
    }
};

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
    

    bool get(Position p) {
        return get(p.i, p.j);
    }
    bool get(int i, int j) {
        if (i < 0 || i >= altura) return 0;
        if (j < 0 || j >= largura) return 0;
        return matriz[i*largura + j];
    }

    void clear(Position p) {
        return clear(p.i, p.j);
    }
    void clear(int i, int j) {
        matriz[i*largura + j] = 0;
    }
};


bool* parse_matriz(std::string& value) {
    StringReader r(value);

    std::string trimmed = "";

    while (char ch = r.peek()) {
        if (!std::isspace(ch)) {
            trimmed += ch;
        }
        ++r;
    };

    int length = trimmed.length();
    bool* matriz = new bool[length];

    for (int i = 0; i < length; i++) {
        matriz[i] = trimmed[i] - 48;
    }

    return matriz;
}

void parse(const std::string& entrada, ArrayList<Cenario>& result) {
    StringReader r(entrada);
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
                result.push_back(cenario);
            }
            else if (tag.key == "nome") {
                cenario.nome = contents;
            }
            else if (tag.key == "x") {
                cenario.roboX = std::stoi(contents);
            }
            else if (tag.key == "y") {
                cenario.roboY = std::stoi(contents);
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
}



int calculate(Cenario& c) {
    auto remaining = std::make_unique<ArrayList<Position>>(c.largura * c.altura);
    int count = 0;

    if (c.get(c.roboX, c.roboY)) {
        remaining->push_back(Position(c.roboX, c.roboY));
    }

    while (!remaining->empty()) {
        Position p = remaining->pop_front();
        c.clear(p);
        count++;

        // std::cout << p.i << " " << p.j << std::endl;

        Position p1(p.i - 1, p.j);
        Position p2(p.i + 1, p.j);
        Position p3(p.i, p.j - 1);
        Position p4(p.i, p.j + 1);

        if (c.get(p1) && remaining->find(p1) == remaining->size()) {
            remaining->push_back(p1);
        }
        if (c.get(p2) && remaining->find(p2) == remaining->size()) {
            remaining->push_back(p2);
        }
        if (c.get(p3) && remaining->find(p3) == remaining->size()) {
            remaining->push_back(p3);
        }
        if (c.get(p4) && remaining->find(p4) == remaining->size()) {
            remaining->push_back(p4);
        }
    }

    return count;
}

int main() {
    char xmlfilename[100];
    std::cin >> xmlfilename;  // entrada
    std::string entrada = read_file(xmlfilename);

    ArrayList<Cenario>* cenarios = new ArrayList<Cenario>(200);

    try {
        parse(entrada, *cenarios);
    }
    catch (...) {
        std::cout << "erro" << std::endl;
        return 0;
    }

    for (int i = 0; i < cenarios->size(); i++) {
        Cenario& cenario = cenarios->at(i);
        int count = calculate(cenario);
        std::cout << cenario.nome << " " << count << std::endl;
    }

    return 0;
}
