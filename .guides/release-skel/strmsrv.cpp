#include "strmsrv.h"
#include <iostream>
#include <fstream>
using namespace std;

// To do - Complete this function
StreamService::StreamService()

{
    // Update as needed

}

// To do - Complete this function
StreamService::~StreamService()
{


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


}

// To do - Complete this function
void StreamService::userLogout()
{

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
            results.push_back(i);
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


}

// To do - Complete this function
void StreamService::reviewShow(CID_T contentID, int numStars)
{


}

// To do - Complete this function
CID_T StreamService::suggestBestSimilarContent(CID_T contentID) const
{
    // Change this when you are ready, but for now it is a 
    // dummy implementation
    return -1;

}

// To do - Complete this function
void StreamService::displayContentInfo(CID_T contentID) const
{
    // Do not alter this
    if(! isValidContentID(contentID)){
        throw std::invalid_argument("Watch: invalid contentID");
    }

    // Call the display abitlity of the appropriate content object



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
