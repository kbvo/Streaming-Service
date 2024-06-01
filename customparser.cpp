// Complete me
#include "customparser.h"
#include <iostream>
#include <sstream>

using namespace std;

const char* error_msg_1 = "Cannot read integer n";
const char* error_msg_2 = "Error in content parsing";
const char* error_msg_3 = "Error in user parsing";

// To Do - Complete this function
void CustomFormatParser::parse(std::istream& is, std::vector<Content*>& content, std::vector<User*>& users)
{
    // Erase any old contents
    content.clear();
    users.clear();

    // TO DO
    int numContents;
    is >> numContents;
    if (is.fail()) { // if can't read number of contents
      throw ParserError(error_msg_1);
    }

    // parse content
    for (int i = 0; i < numContents; i++) {
      // content parameters
      int id, contentType, numReviews, totalStars, rating;
      std::string name;
      
      is >> id;
      if (is.fail()) { // if can't read id
        throw ParserError (error_msg_2);
      }
      is >> contentType;
      if (is.fail()) { // if can't read content type
        throw ParserError (error_msg_2);
      }
      is >> std::ws; // clears '\n' and whitespace
      std::getline(is, name); // read line with name
      if (name.empty()) { // if no name
        throw ParserError(error_msg_2);
      }
      is >> numReviews; 
      if (is.fail()) { // if can't read number of reviews
        throw ParserError (error_msg_2);
      }
      is >> totalStars;
      if (is.fail()) { // if can't read total stars
          throw ParserError (error_msg_2);
      } 
      is >> rating;
      if (is.fail()) { // if can't read rating
        throw ParserError (error_msg_2);
      }

      // pointer to content that will be a movie or a series
      Content* newContent = nullptr;

      // if content is a movie
      if (contentType == 0) {
        // newContent points to movie with appropriate parameters
        newContent = new Movie(id, name, numReviews, totalStars, rating);
      }
      else if (contentType == 1) { // if content is series
        int numEpisodes; // get number of episodes
        is >> numEpisodes;
        if (is.fail()) { // if can't get number of episodes
          throw ParserError(error_msg_2);
        }
        // newContent points to series with appropriate parameters
        newContent = new Series(id, name, numReviews, totalStars, rating, numEpisodes);
      }

      std::string viewersLine;
      std::getline(is, viewersLine); // clears '\n'
      std::getline(is, viewersLine); // read line with viewers
      if (!viewersLine.empty()) { // if there are viewers
        // parse viewers
        stringstream ss1;
        ss1 << viewersLine;
        std::string contentViewer;
        while (ss1 >> contentViewer) {
          newContent->addViewer(contentViewer); // add viewer to content's viewers
        }
      }
      content.push_back(newContent); // add new content to content
    }

    // parse users

    // user parameters
    std::string uName;
    int ratingLimit;

    while (is >> uName) { // while there are users to be read
      is >> ratingLimit;
      if (is.fail()) { // if can't read user's limit
        throw ParserError (error_msg_3); 
      }

      // create user with appropriate parameters
      User* newUser = new User(uName, ratingLimit);
      
      std::string viewedLine;
      std::getline(is, viewedLine); // clear '\n"
      std::getline(is, viewedLine); // read line with watched content ids
      if (!viewedLine.empty()) { // if user has watched some content
        // parse watched content ids
        stringstream ss2;
        ss2 << viewedLine;
        int contentID;
        while (ss2 >> contentID) {
          newUser->addToHistory(contentID); // add content to user history
        }
      }
      users.push_back(newUser); // add user to users
    }
}
