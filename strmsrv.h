#ifndef STRMSRV_H
#define STRMSRV_H
#include <vector>
#include <string>
#include <stdexcept>
#include "parser.h"
#include "user.h"

/**
 * @brief Exception to be thrown when an operation is called
 *        that expects a current user (to be logged in), but
 *        none exists.
 * 
 */
struct UserNotLoggedInError : public std::runtime_error
{
    UserNotLoggedInError(const char* what) : 
        std::runtime_error(what)
    {}
};

/**
 * @brief Exception to be thrown when a User attempts to watch
 *        content above their rating limit
 * 
 */
struct RatingLimitError : public std::runtime_error
{
    RatingLimitError(const char* what) : std::runtime_error(what) 
    {}
};

/**
 * @brief Exception to be thrown when a User gives a 
 *        Contente a review with a number of starts outside 0-5
 * 
 */
struct ReviewRangeError : public std::range_error
{
    ReviewRangeError(const char* what) : std::range_error(what) 
    {}
};

class StreamService {
public:
    /**
     * @brief Default constructor
     * 
     */
    StreamService();
    /**
     * @brief Destroy the Stream Service object and its
     *        Content and Users
     * 
     */
    ~StreamService();
    /**
     * @brief Reads Content and User data from the specified file
     *        using the given parser.
     * 
     * @param is input stream (usually ifstream) containing the input data
     * @param p Parser object to use to read/parse the file
     * @throw ParserException if an error is encountered in parsing the file
     */
    void readAndParseData(std::istream& is, Parser& p);

    /**
     * @brief Sets the current user to the given username
     * 
     * @param uname username that will be the current user
     * @throw runtime_error if another user is already logged in (i.e. they
     *                      should logout first)
     * @throw invalid_argument if the username is not valid
     */
    void userLogin(const std::string& uname);

    /**
     * @brief Sets the appropriate state to indicate no user is
     *        logged in. If no user was logged in, just do nothing (return)
     * 
     */
    void userLogout();

    /**
     * @brief Search for all content that "partially" matches the given input 
     *        string. Note: Does not require a user to be logged in currently
     * 
     * @param partial "*" should return ALL content while any other string
     *                should be used to return only the Content items whose
     *                name contains that exact substring.
     * @return std::vector<CID_T> IDs of content that match the search criteria
     */
    std::vector<CID_T> searchContent(const std::string& partial) const;

    /**
     * @brief Get the viewing history of the User as a vector of content IDs
     * 
     * @return std::vector<CID_T> IDs of content this User has viewed
     * @throw UserNotLoggedInError if no User is currently logged in.
     */
    std::vector<CID_T> getUserHistory() const;

    /**
     * @brief Updates data structures to indicate the current User has watched
     *        this content
     * 
     * @param contentID ID of the content to watch
     * @throw UserNotLoggedInError if no User is currently logged in.
     * @throw range_error      if the contentID is not valid
     * @throw RatingLimitError if the content's rating is above the User's 
     *                         rating limit
     */
    void watch(CID_T contentID);

    /**
     * @brief Add a review with the given number of stars to the specified
     *        content. We will NOT worry about a user reviewing content
     *        more than once.
     * 
     * @param contentID ContentID to review
     * @param numStars Number of stars for the review (must be 0-5)
     * @throw UserNotLoggedInError if no User is currently logged in.
     * @throw ReviewRangeError if the contentID is not valid or the 
     *                    number of stars is out of the range 0-5
     */
    void reviewShow(CID_T contentID, int numStars);

    /**
     * @brief Returns the ID of the chosen content suggestion
     *  
     * For the given content, Cp, and current user, Uq, consider the set of 
     * Users, Uj, (Uj /= Uq) who also watched content Cp and find the single, 
     * content item, Ck, (Ck /= Cp) that was viewed the most times by users
     * in the set, Uj.
     * 
     * @param contentID ID of the content for which we want suggested similar
     *                  content
     * @return CID_T of the suggestion.
     * @throw UserNotLoggedInError if no User is currently logged in.
     * @throw range_error      if the contentID is not valid
     */
    CID_T suggestBestSimilarContent(CID_T contentID) const; // returns content ID

    /**
     * @brief Used to display information about a Content item.  The return
     *        result of searchContent and getUserHistory can call this function
     *        for each item it returned to display the appropriate Content 
     *        information.
     * 
     * @param contentID 
     */
    void displayContentInfo(CID_T contentID) const;

private:
    /**
     * @brief Helper function to throw UserNotLoggedInError if there is 
     * no current user
     * 
     * @throw UserNotLoggedInError
     */
    void throwIfNoCurrentUser() const;

    /**
     * @brief Returns true if the content ID is valid (in range)
     * 
     */
    bool isValidContentID(CID_T contentID) const;

    /**
     * @brief Returns the index in the users_ vector of the given User
     * 
     * @param uname Username to search for
     * @return int Index of the given User or -1 if the User doesn't exist
     */
    int getUserIndexByName(const std::string& uname) const;

    /// @brief Data members
    std::vector<User*> users_;
    std::vector<Content*> content_;
    User* cUser_;  // current User;
};

#endif
