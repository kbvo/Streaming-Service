// Complete me
#ifndef CUSTOMPARSER_H
#define CUSTOMPARSER_H
#include "parser.h"

class CustomFormatParser : public Parser {
public:
    /**
     * @brief Reads and parses the contents of the given input stream
     *        to create dynamically allocated derived Content objects and Users,
     *        filling in the referenced vectors. The vectors will be cleared before
     *        adding the new objects.
     * 
     * @param is Input stream (ifstream, stringstream, etc.) to read in the data from 
     * @param content Vector of Content pointers to be filled
     * @param users Vector of User pointers to be filled
     *
     * @throw ParserError if there is an error in extracting the expected data/format
     */
    void parse(
        std::istream& is, 
        std::vector<Content*>& content, 
        std::vector<User*>& users);
};

#endif