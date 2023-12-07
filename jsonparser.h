#ifndef JSONPARSER_H
#define JSONPARSER_H
#include "parser.h"

class JsonParser : public Parser {
public:
    void parse(
        std::istream& is, 
        std::vector<Content*>& content, 
        std::vector<User*>& users);
};
#endif