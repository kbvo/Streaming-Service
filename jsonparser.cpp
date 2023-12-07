#include "jsonparser.h"
#include "json.hpp"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

using json = nlohmann::json;

void JsonParser::parse(std::istream& is, std::vector<Content*>& content, std::vector<User*>& users)
{
    json j;
    is >> j;
    // Erase any old contents
    content.clear();
    users.clear();

    // TO DO - Add your code below.
    if(j.find("content") == j.end()){
        throw ParserError("can't find content section");
    }
    size_t numContent = j["content"].size();
    //cout << "numContent size " << numContent << endl;
    for(size_t c = 0; c < numContent; c++){
        auto& citem = j["content"][c];
        try {
            int id = citem["id"].get<int>();
            int type = citem["type"].get<int>();
            string name = citem["name"].get<std::string>();
            int nr = citem["reviews"].get<int>();
            int ts = citem["stars"].get<int>();
            int rating = citem["rating"].get<int>();
            int numEp = 0;
            if(type == 1){
                numEp = citem["episodes"].get<int>();
            }
            Content* newc = NULL;
            if(type == 0){
                newc = new Movie(id, name, nr, ts, rating);
            }
            else {
                newc = new Series(id, name, nr, ts, rating, numEp);
            }
            auto& viewers = citem["viewers"];
            for(size_t i = 0; i < viewers.size(); i++) {
                auto& viewer = viewers[i];
                auto x = viewer.get<std::string>();
                newc->addViewer(x);
            }
            content.push_back(newc);
        }
        catch(std::exception& e){
            throw ParserError("Couldn't parse content item ");
        }      
    }
    if(j.find("users") == j.end()){
        throw ParserError("can't find content section");
    }
    //size_t numUsers = j["users"].size();
    for(auto& uitem : j["users"]) {
        try {
            string uname = uitem["uname"].get<std::string>();
            int limit = uitem["limit"].get<int>();
            User* u = new User(uname, limit);
            auto& views = uitem["viewed"];
            for(auto& cid : views) {
                u->addToHistory(cid.get<int>());
            }
            users.push_back(u);
        }
        catch(std::exception& e){
            throw ParserError("Couldn't parse content user ");
        }      
    }

}