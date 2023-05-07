#include <fstream>
#include <string>
#include <iostream>
#include <memory>
#include <cctype>
#include <cstring>
#include <exception>

#include "array_list.h"
#include "array_stack.h"

using namespace structures;


class parse_error : std::exception {
    std::string message;

 public:
    parse_error() : message("") {}
    parse_error(std::string msg) : message(msg) {}

    virtual const char* what() const throw() {
        return message.c_str();
    }
};

class StringReader {
public:
    StringReader(const std::string& string) : str(string) {}

    char peek() {
        return str[i];
    }

    char read_char(char ch = 0) {
        char p = peek();
        
        if (!ch || ch == p) {
            i++;
            return p;
        }
        return 0;
    }

    std::string read_word() {
        int start = i;
        char ch = peek();

        while(ch && !isspace(ch) && ch != '<' && ch != '>') {
            i++;
            ch = peek();
        }
        return str.substr(start, i - start);
    }

    std::string read_until(char ch) {
        int start = i;
        while(peek() && peek() != ch) {
            i++;
        }
        return str.substr(start, i - start);
    }

    void consume_space() {
        while (peek() && isspace(peek())) {
            i++;
        }
    }

    char expect(char ch) {
        char result = read_char(ch);
        if (!result) {
            throw parse_error(std::string("Expected character '") + ch + "'. Got '" + peek() + "'.");
        }
        return result;
    }

    StringReader& operator++() {
        ++i;
        return *this;
    }

private:
    const std::string& str;
    int i = 0;
};


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


struct Tag {
    std::string key = "";
    bool is_closing = false;

    Tag(std::string key, bool is_closing) : key(key), is_closing(is_closing) {}
};

Tag parse_tag(StringReader& r) {
    r.expect('<');
    bool is_closing = r.read_char('/');
    r.consume_space();

    std::string key = r.read_word();

    r.consume_space();
    r.expect('>');

    return Tag(key, is_closing);
}

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
        // conversão do código ascii para valor 0-1.
        matriz[i] = trimmed[i] - 48;
    }

    return matriz;
}

void parse(const std::string& entrada, ArrayList<Cenario>& result) {
    StringReader r(entrada);
    ArrayStack<std::string> stack(100);
    Cenario cenario;

    while (r.peek()) {
        std::string contents = r.read_until('<');

        if (!r.peek()) break;
        Tag tag = parse_tag(r);
        
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


// função helper para testar se um elemento T está
// presente na lista.
template <typename T>
bool contains(ArrayList<T>* lista, T& elemento) {
    return lista->find(elemento) != lista->size();
}

int calculate(Cenario& c) {
    auto remaining = new ArrayList<Position>(c.largura * c.altura);
    int count = 0;

    // adicionar posiçao inicial do robo se é uma posição com 1
    if (c.get(c.roboX, c.roboY)) {
        remaining->push_back(Position(c.roboX, c.roboY));
    }

    // enquanto há posições para limpar
    while (!remaining->empty()) {
        // pega a próxima posição
        Position p = remaining->pop_front();
        // se a posição precisa ser limpa
        if (c.get(p)) {
            // limpa região e acrescenta contagem
            c.clear(p);
            count++;
        }

        // para cada posição adjacente
        Position p1(p.i - 1, p.j);
        Position p2(p.i + 1, p.j);
        Position p3(p.i, p.j - 1);
        Position p4(p.i, p.j + 1);

        // adiciona posição se precisar ser limpa
        // e ainda não foi adicionada a lista de posições
        if (c.get(p1) && !contains(remaining, p1)) {
            remaining->push_back(p1);
        }
        if (c.get(p2) && !contains(remaining, p2)) {
            remaining->push_back(p2);
        }
        if (c.get(p3) && !contains(remaining, p3)) {
            remaining->push_back(p3);
        }
        if (c.get(p4) && !contains(remaining, p4)) {
            remaining->push_back(p4);
        }
    }

    delete remaining;
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
