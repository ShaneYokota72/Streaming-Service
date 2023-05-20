#include "strmsrv.h"
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

// To do - Complete this function
StreamService::StreamService()
{
    // Update as needed
    cUser_ = NULL;
}

// To do - Complete this function
StreamService::~StreamService()
{
    for(unsigned int i=0; i<users_.size(); i++){
        delete users_[i];
    }
    for(unsigned int i=0; i<content_.size(); i++){
        delete content_[i];
    }
}

// Complete - Do not alter
void StreamService::readAndParseData(std::istream& is, Parser& p)
{
    p.parse(is, content_, users_);
    cout << "Read " << content_.size() << " content items." << endl;
    cout << "Read " << users_.size() << " users." << endl;
}

// To do - Complete this function
void StreamService::userLogin(const std::string& uname)
{  
    // if name does not exist or someone is alredy logged in, throw necessary exceptions
    // if no issues, change the current user to logged in user
  if(cUser_ != NULL){
    throw runtime_error("User logged in");
  } else if(getUserIndexByName(uname) == -1){
    throw invalid_argument("User Doesnt exist");
  } else {
    cUser_ = users_.at(getUserIndexByName(uname));
  }
}

// To do - Complete this function
void StreamService::userLogout()
{
    //change current user to NULL
    if(cUser_ == NULL){
        return;
    } else {
        cUser_ = NULL;
    }
}

// To do - Complete this function
std::vector<CID_T> StreamService::searchContent(const std::string& partial) const
{
    std::vector<CID_T> results;
    for(size_t i = 0; i < content_.size(); i++){
        // TO DO - modify this to also push back when the string 'partial'
        //  is contained in the name of the current content. Lookup the string
        //  class documentation to find an appropriate function that can be used
        //  to do this simply.
        if(partial == "*" /* update me */){
            results.push_back((*content_[i]).id()/*i*/);
        } else if(((*content_[i]).name()).find(partial) != std::string::npos){
            results.push_back((*content_[i]).id()/*i*/);
        }
    }
    return results;
}

// Complete - Do not alter
std::vector<CID_T> StreamService::getUserHistory() const
{
    throwIfNoCurrentUser();
    return cUser_->history;
}

// To do - Complete this function
void StreamService::watch(CID_T contentID)
{
    //getting the index in content_ where it has the pointer for content:contentID
    int contIDIdx = 0;
    for(unsigned int i=0; i<content_.size(); i++){
        if((*(content_[i])).id() == contentID){
            contIDIdx = i;
        }
    }

    // if no one is logged in, content does not exist, or rating limit is too high, throw neccesary exceptions
    // if not, add the content to user's history and add user to the content's viewed list
    if(cUser_ == NULL){
        throwIfNoCurrentUser();
    } else if(!isValidContentID(contentID)){
        throw range_error("content does not exist");
    } else if((*(content_[contIDIdx])).rating() > cUser_->ratingLimit){
        throw RatingLimitError("User not permitted to view this content");
    } else {
        (*cUser_).addToHistory(contentID);
        (*(content_[contIDIdx])).addViewer((*cUser_).uname);
    }
}

// To do - Complete this function
void StreamService::reviewShow(CID_T contentID, int numStars)
{
    //getting the index in content_ where it has the pointer for content:contentID
    int contIDIdx = 0;
    for(unsigned int i=0; i<content_.size(); i++){
        if((*(content_[i])).id() == contentID){
            contIDIdx = i;
        }
    }

    // if no one is logged in, content does not exist, or star is not in the range of [0,5], throw neccesary exceptions
    // if not, add the review to the content and also add the user to the content's view list
    if(cUser_ == NULL){
        throwIfNoCurrentUser();
    } else if(!isValidContentID(contentID)){
        throw range_error("content does not exist");
    } else if(numStars>5 || numStars<0){
        throw ReviewRangeError("Review should be between 0-5 stars");
    } else {
        (*(content_[contIDIdx])).review(numStars);
        (*(content_[contIDIdx])).addViewer((*cUser_).uname);
    }
}

// To do - Complete this function
CID_T StreamService::suggestBestSimilarContent(CID_T contentID) const
{
    //checking usernotloggedinerror
    //cout << "contentID: " << contentID << endl;

    //check for cases to throw a exception
    if(cUser_ == NULL){
        throwIfNoCurrentUser();
        return -1;
    } else if(!isValidContentID(contentID)){
        throw range_error("content out of range");
        return -1;
    }
    
    // find a user that is not the current user and have watched the content:contentID
    // then add the content that was wached by the other users who watched the content:contentID to the vector<CID_T> contentssum
    // then find the most repeated contentID in the vector:contentssum
    vector<CID_T> contentssum;
    vector<int> freq;
    if(isValidContentID(contentID) && (cUser_!=NULL)){
        // find the content that is eligible for the suggesting algorithm
        // then add content to the contentssum vector
        for(unsigned int i=0; i<users_.size(); i++){
            if(users_.at(i) != cUser_ && (*(users_.at(i))).haveWatched(contentID)){
                for(unsigned int k=0; k<(users_.at(i)->history).size(); k++){
                    if((users_.at(i)->history).at(k) != contentID){
                        contentssum.push_back((users_.at(i)->history).at(k));
                    }
                }
            }
        }
        
        // if theres nothing as the candidate of the suggesting content, return -1
        if(contentssum.size() == 0){
          return -1;
        }
        for(int i=0; i<contentssum.size(); i++){
            // cout << contentssum.at(i) << endl;
        }
        // calculate the frequency, and determine which one to suggest
        for(unsigned int i=0; i<contentssum.size(); i++){
            int frequency = 0;
            for(unsigned int j=0; j<contentssum.size(); j++){
                if(contentssum.at(i) == contentssum.at(j)){
                    frequency++;
                }
            }
            freq.push_back(frequency);
        }

        // from the frequency vector, find the most repeated contentID
        int maxidx = 0;
        for(unsigned int i=0; i<freq.size(); i++){
            if(freq.at(i) >= freq.at(maxidx)){
                maxidx = i;
            }
        }

        // check if the content have been watched
        bool havewatched = false;
        for(unsigned int i=0; i<cUser_->history.size(); i++){
            if(cUser_->history.at(i) == contentssum.at(maxidx)){
                havewatched = true;
            }
        }

        // if the suggeseted content is already viewed by the currentuser, go to the next suggested content
        if(havewatched){
            for(int i = freq.at(maxidx)-1; i>= 0; i--){
                for(unsigned int j=0; j<freq.size(); j++){
                    if(freq.at(j) == i && !(cUser_->haveWatched(contentssum.at(j)))){
                        return contentssum.at(j);
                    }
                }
            }
            return -1;
        } else {
            return contentssum.at(maxidx);
        }
    } else {
        return -1;
    }
}

// To do - Complete this function
void StreamService::displayContentInfo(CID_T contentID) const
{
    // Do not alter this
    if(! isValidContentID(contentID)){
        throw std::invalid_argument("Watch: invalid contentID");
    }

    // Call the display abitlity of the appropriate content object
    for(unsigned int i=0; i<content_.size(); i++){
        if((*(content_.at(i))).id() == contentID){
            (*(content_.at(i))).display(cout);
        }
    }


}

// Complete - Do not alter
bool StreamService::isValidContentID(CID_T contentID) const
{
    return (contentID >= 0) && (contentID < (int) content_.size());
}

// Complete - Do not alter
void StreamService::throwIfNoCurrentUser() const
{
    if(cUser_ == NULL){
        throw UserNotLoggedInError("No user is logged in");
    }
}

// Complete - Do not alter
int StreamService::getUserIndexByName(const std::string& uname) const
{
    for(size_t i = 0; i < users_.size(); i++){
        if(uname == users_[i]->uname) {
            return (int)i;
        }
    }
    return -1;
}
