#include "content.h"
#include <iostream>
#include <iomanip>

using namespace std;

Content::Content(int id, std::string name, int nr, int ts, int rating)
{
  id_ = id;
  name_ = name;
  numReviews_ = nr;
  totalStars_ = ts;
  rating_ = rating;
}

Content::~Content()
{

}

void Content::display(std::ostream& ostr) const
{
    ostr << "========" << endl;
    ostr << "ID: " << id_ << endl;
    ostr << "Name: " << name_ << endl;
    ostr << "Views: " << usersWhoViewed_.size() << endl;
    ostr << "Average review: " << setprecision(1) << fixed << getStarAverage() << endl;
    
    if(rating_ >= 0 && rating_ < 5) {
        const char** ratingMapping = this->ratingStrings();
        ostr << "Rating: " << ratingMapping[rating_] << endl;
    }
}

int Content::id() const
{
    return id_;
}

std::string Content::name() const
{
    return name_;
}

int Content::rating() const
{
    return rating_;
}

void Content::review(int numStars) 
{
    numReviews_++;
    totalStars_ += numStars;
}

void Content::addViewer(const std::string& username)
{
    usersWhoViewed_.push_back(username);
}

const std::vector<std::string>& Content::getViewers() const
{
    return usersWhoViewed_;
}

bool Content::hasViewed(const std::string& uname) const
{
    for(size_t m = 0; m < usersWhoViewed_.size(); m++){
        if(usersWhoViewed_[m] == uname){
            return true;
        }
    }
    return false;
}

const char** Movie::ratingStrings() const
{
    static const char* movierating[] = {"G", "G", "PG", "PG-13", "R"};
    return movierating;
}

const char** Series::ratingStrings() const
{
    // DEFAULT TO THE SERIES / TV Rating Mappings
    
    // Global variable of series rating mapping (0 = "TVY", 1 = "TVG", etc.)
    static const char* tvrating[] = {"TVY", "TVG", "TVPG", "TV14", "TVMA"};
    return tvrating;
}

double Content::getStarAverage() const 
{
  if (numReviews_ == 0) { // if there are 0 reviews
    return 0.0; // return avg. of 0 to prevent division by 0
  }
  else { // else return star average
    return static_cast<double>(totalStars_) / numReviews_;
  }
}

int Series::numEpisodes() const
{
  // Should return the number of episodes in the series
  return numEpisodes_;
}

Movie::Movie(int id, std::string name, int nr, int ts, int rating) :
Content(id, name, nr, ts, rating) {

}

Movie::~Movie() {
  
}

void Movie::display(std::ostream& ostr) const {
  ostr << "========" << endl;
  ostr << "ID: " << id() << endl;
  ostr << "Name: " << name() << endl;
  std::vector<std::string> numViewers = getViewers();
  ostr << "Views: " << numViewers.size() << endl;
  ostr << "Average review: " << setprecision(1) << fixed << getStarAverage() << endl;
    
  if(rating() >= 0 && rating() < 5) {
    const char** ratingMapping = this->ratingStrings();
    ostr << "Rating: " << ratingMapping[rating()] << endl;
  }
}

Series::Series(int id, std::string name, int nr, int ts, int rating, int numEpisodes) :
Content (id, name, nr, ts, rating) {
  numEpisodes_ = numEpisodes;
}

Series::~Series() {
  
}

void Series::display(std::ostream& ostr) const {
  ostr << "========" << endl;
  ostr << "ID: " << id() << endl;
  ostr << "Name: " << name() << endl;
  std::vector<std::string> numViewers = getViewers();
  ostr << "Views: " << numViewers.size() << endl;
  ostr << "Average review: " << setprecision(1) << fixed << getStarAverage() << endl;
    
  if(rating() >= 0 && rating() < 5) {
    const char** ratingMapping = this->ratingStrings();
    ostr << "Rating: " << ratingMapping[rating()] << endl;
  }

  ostr << "Episodes: " << numEpisodes() << endl;
}



