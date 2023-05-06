#include <cctype>
#include <string>
#include <cstring>
#include <iostream>
#include <exception>

#include "array_stack.h"


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

    struct Tag {
        std::string key = "";
        bool is_closing = false;
    };
    Tag parse_tag() {
        expect('<');
        bool is_closing = read_char('/');
        consume_space();

        std::string key = read_word();

        consume_space();
        expect('>');

        return {key, is_closing};
    }

    StringReader& operator++() {
        ++i;
        return *this;
    }

private:
    const std::string& str;
    int i = 0;
};