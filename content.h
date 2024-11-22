#ifndef CONTENT_H
#define CONTENT_H
#include <vector>
#include <string>
#include <iostream>



class Content {
public:
    /**
     * @brief Construct a new Content object
     * 
     * @param id Content ID 
     * @param name Content name (spaces are fine)
     * @param nr Number of reviews
     * @param ts Total stars of all reviews
     * @param rating Parental control rating
     */
    Content(int id, std::string name, int nr, int ts, int rating);
    /**
     * @brief Destroy the Content object
     * 
     */
    virtual ~Content();

    /**
     * Accessors
     */
    int id() const;
    std::string name() const;
    int rating() const;
    const std::vector<std::string>& getViewers() const;

    /**
     * @brief Displays the Content item information (except for the viewers)
     *        in a nice visual form
     * 
     * @param ostr Stream to output to
     */
    virtual void display(
        std::ostream& ostr) const;

    /**
     * @brief Gives a review for this Content item with the given number 
     *        of stars
     * 
     * @param numStars Number of stars for the review
     * @throw std::invalid_argument if numStars is not between 0-5
     */
    void review(int numStars);

    /**
     * @brief Get the average number of stars over all the reviews
     * 
     * @return double review average
     */

    double getStarAverage() const;
    /**
     * @brief Adds the given username to the list of those who have viewed
     *        this content
     * 
     * @param username 
     */
    void addViewer(const std::string& username);

    /**
     * @brief Convenience function to check if a user has viewed this content
     * 
     * @param uname Username to find
     * @return true If the user HAS viewed this content
     * @return false otherwise
     */
    bool hasViewed(const std::string& uname) const;

protected:
    /**
     * @brief Returns an array of C-Strings that correspond to ratings
     *        to be displayed for a particular type of Content
     * 
     * @return const char* 
     */
    virtual const char** ratingStrings() const = 0;

private:
    int id_;
    std::string name_;
    int numReviews_;
    int totalStars_;
    int rating_;
    std::vector<std::string> usersWhoViewed_;
};

class Movie : public Content// update me 
{
public:
    /**
     * @brief Construct a new Movie object
     * 
     * @param id Content ID 
     * @param name Content name (spaces are fine)
     * @param nr Number of reviews
     * @param ts Total stars of all reviews
     * @param rating Parental control rating
     */
    Movie(int id, std::string name, int nr, int ts, int rating);
    /**
     * @brief Destroy the Movie object
     * 
     */
    ~Movie();

    void display(std::ostream& ostr) const;

protected:
    const char** ratingStrings() const;

private:

};

class Series : public Content {
public:
    /**
     * @brief Construct a new Series object
     * 
     * @param id Content ID 
     * @param name Content name (spaces are fine)
     * @param nr Number of reviews
     * @param ts Total stars of all reviews
     * @param rating Parental control rating
     * @param numEpisode Numer of episodes in the series
     */
    Series(int id, std::string name, int nr, int ts, int rating, int numEpisodes);
    /**
     * @brief Destroy the Series object
     * 
     */
    ~Series();

    /**
     * @brief Accessor / getter to return the number of episodes in the series
     * 
     * @return int 
     */
    int numEpisodes() const;

    void display(std::ostream& ostr) const;

protected:
    const char** ratingStrings() const;

private:
    int numEpisodes_;

};

#endif
