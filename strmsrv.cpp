#include "strmsrv.h"
#include <iostream>
#include <fstream>
using namespace std;

StreamService::StreamService()
{
    cUser_ = nullptr;
}

StreamService::~StreamService()
{
  for (size_t i = 0; i < users_.size(); i++) {
    delete users_[i];
  }
  for (size_t i = 0; i < content_.size(); i++) {
    delete content_[i];
  }
}

void StreamService::readAndParseData(std::istream& is, Parser& p)
{
    p.parse(is, content_, users_);
    cout << "Read " << content_.size() << " content items." << endl;
    cout << "Read " << users_.size() << " users." << endl;
}

void StreamService::userLogin(const std::string& uname)
{
  if (cUser_ != nullptr) { // if another user is already logged in
    throw std::runtime_error("Another user is already logged in. Please logout first.");
  }
  int userIndex = getUserIndexByName(uname);
  if (userIndex == -1) { // if invalid name
    throw std::invalid_argument("Invalid username provided");
  }
  cUser_ = users_[userIndex]; // user logs in
}

void StreamService::userLogout()
{
  cUser_ = nullptr; // user logs out
}


std::vector<CID_T> StreamService::searchContent(const std::string& partial) const
{
    std::vector<CID_T> results;
    for(size_t i = 0; i < content_.size(); i++){
      if(partial == "*"){ // if string argument is "*"
        results.push_back(content_[i]->id()); // add all IDs
      }
      else {
        std::string contentName = content_[i]->name(); // get name of content
        if (contentName.find(partial) != std::string::npos) { // if string argument is in content name
          results.push_back(content_[i]->id()); // add ID of content
        }
      }
    }
    return results;
}

std::vector<CID_T> StreamService::getUserHistory() const
{
    throwIfNoCurrentUser();
    return cUser_->history;
}

void StreamService::watch(CID_T contentID)
{
  throwIfNoCurrentUser(); // if no current user
  if (!isValidContentID(contentID)) { // if not valid ID
    throw std::range_error("Invalid contentID");
  }
  // find content with correct ID
  Content* foundContent = nullptr;
  for (size_t i = 0; i < content_.size(); i++) {
    if (content_[i]->id() == contentID) {
      foundContent = content_[i];
      break;
    }
  }
  if (foundContent->rating() > cUser_->ratingLimit) {
    throw RatingLimitError("Content rating exceeds user's limit");
  }
  cUser_->addToHistory(contentID); // add content ID to user's watch history
  foundContent->addViewer(cUser_->uname); // add viewer's username to content's viewers
}

void StreamService::reviewShow(CID_T contentID, int numStars)
{
  throwIfNoCurrentUser(); // if no current user
  // if not valid content ID or not valid review (too few or many stars)
  if (!isValidContentID(contentID) || numStars < 0 || numStars > 5) {
    throw ReviewRangeError("Invalid contentID or number of stars");
  }
  // find content with correct ID
  Content* foundContent = nullptr;
  for (size_t i = 0; i < content_.size(); i++) {
    if (content_[i]->id() == contentID) {
      foundContent = content_[i];
      break;
    }
  }
  foundContent->review(numStars); // add review to content
}

CID_T StreamService::suggestBestSimilarContent(CID_T contentID) const
{
    throwIfNoCurrentUser(); // if no current user

    if(!isValidContentID(contentID)) { // if invalid contentID
      throw std::range_error("Invalid contentID");
    }

    // stores other users who have watched particular content into similarUsers
    std::vector<User*> similarUsers;
    for (size_t i = 0; i < users_.size(); i++) { // iterate through all users
      // if user isn't the one logged in and user has watched particular content 
      if (users_[i] != cUser_ && users_[i]->haveWatched(contentID)) {
        similarUsers.push_back(users_[i]); // add user to similarUsers
      }
    }

    // stores all watched content IDs of similar users to similarContentIDs
    // if content ID is not of the particular content and logged-in user has not watched it
    
    // NOTE:
    // there will be duplicates of a similarContentID added
    // if more than 1 similar user watches the same piece of content
    // that fits the criteria of being a similarContentID, but
    // we can still find correct ID based on number of views later

    std::vector<CID_T> similarContentIDs;
    // iterate through similar users
    for (size_t i = 0; i < similarUsers.size(); i++) {
      // iterate through the watch history of each similar user
      for (size_t j = 0; j < similarUsers[i]->history.size(); j++) {
        // if content is not the particular content and original user has not watched it
        if (similarUsers[i]->history[j] != contentID && !cUser_->haveWatched(similarUsers[i]->history[j])) {
          // add content ID to similarContentIDs
          similarContentIDs.push_back(similarUsers[i]->history[j]);
        }
      }
    }

    // if no content in similar users' histories fit criteria to be similarContentIDs
    if (similarContentIDs.empty()) {
      return -1;
    }
    
    // stores the number of views for each similar content
    // in similarContentViews relative to index of similarContentIDs
    std::vector<int> similarContentViews;
    
    // iterate through similarContentIDs
    // NOTE: ensures index of similarContentViews will
    // correlate with index of similarContentIDs
    for (size_t i = 0; i < similarContentIDs.size(); i++) {
      int contentViews = 0; // intializes similar content's views to 0
      // iterate through similar users for each similar content ID
      for (size_t j = 0; j < similarUsers.size(); j++) {
        // if similar user has watched similar content
        if (similarUsers[j]->haveWatched(similarContentIDs[i])) {
          contentViews++; // add view to similar content's views
        }
      }
      // store total views of similar content to similarContentViews at correct index
      similarContentViews.push_back(contentViews);
    }
    
    // keeps track of highest amount of views 
    int maxViews = 0;
    
    // index of similar content with highest amount of views in similarContentIDs 
    int suggestedContentIndex = 0;

    // iterate through each similar contents' amount of views
    for (size_t i = 0; i < similarContentViews.size(); i++) {
      // if similar content's amount of views is the highest amount of views so far
      if (similarContentViews[i] > maxViews) {
        // highest amount of views is similar content's amount of views
        maxViews = similarContentViews[i];
        // index of similar content with highest amount of views is at this index (i)
        suggestedContentIndex = i;
      }
    }

    // return similarContentID with highest amount of views at suggestedContentIndex
    return similarContentIDs[suggestedContentIndex];
}

void StreamService::displayContentInfo(CID_T contentID) const
{
    if(! isValidContentID(contentID)){
        throw std::invalid_argument("Watch: invalid contentID");
    }
}

bool StreamService::isValidContentID(CID_T contentID) const
{
    return (contentID >= 0) && (contentID < (int) content_.size());
}

void StreamService::throwIfNoCurrentUser() const
{
    if(cUser_ == NULL){
        throw UserNotLoggedInError("No user is logged in");
    }
}

int StreamService::getUserIndexByName(const std::string& uname) const
{
    for(size_t i = 0; i < users_.size(); i++){
        if(uname == users_[i]->uname) {
            return (int)i;
        }
    }
    return -1;
}
