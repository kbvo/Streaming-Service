#ifndef PARSER_H
#define PARSER_H
#include <iostream>
#include <stdexcept>
#include <vector>
#include "user.h"
#include "content.h"

/**
 * @brief Exception to be thrown when an unexpected parser error occurs
 * 
 */
struct ParserError : public std::runtime_error
{
    ParserError(const char* what) : std::runtime_error(what) {}
};

class Parser {
public:
    virtual ~Parser() {}
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
    virtual void parse(
        std::istream& is, 
        std::vector<Content*>& content, 
        std::vector<User*>& users) = 0;
};

#endif