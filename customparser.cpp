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

    // TO DO - Add your code below.

    //get contentnumber to set the for loop
    int contentnum = 0;
    string getcontentnum;
    is >> contentnum;
    if(is.fail()){
        throw ParserError(error_msg_1);
    }
    // make sure that I am in the next line
    while(is.peek() == '\t' || is.peek() == '\n' || is.peek() == ' '){
        is.get();
    }
    // run contentnum time to get the exact amount of content
    for(int i=0; i<contentnum; i++){
        int id, type, NoR, TotalS, RatingLim, episode;
        string title;
        string IDnTYPE;
        episode = -1;
        string alluser;
        // make sure that I am in the next line
        while(is.peek() == '\t' || is.peek() == '\n' || is.peek() == ' '){
            is.get();
        }
        //get ID and Type
        getline(is, IDnTYPE);
        stringstream ss;
        ss << IDnTYPE;
        ss >> id >> type;
        // make sure that I am in the next line
        while(is.peek() == '\t' || is.peek() == '\n'){
            is.get();
        }
        // get title
        getline(is, title);
        if(is.fail()){
            throw ParserError(error_msg_2);
        }

        // depending on the type, make Movie or Series
        if(type == 0){
            // Movie

            // take all the required information to construct a Movie
            stringstream ssmovie;
            for(int i=0; i<3; i++){
                while(is.peek() == '\n' || is.peek() == '\t' || is.peek() == ' '){
                    is.get();
                }
                string temp;
                is >> temp;
                if(is.fail()){
                    throw ParserError(error_msg_2);
                }
                ssmovie << temp << " ";
            }
            ssmovie >> NoR >> TotalS >> RatingLim;
            if(ssmovie.fail()){
                throw ParserError(error_msg_2);
            }

            // dynamically allocate a Movie
            Movie* temp = new Movie(id, title, NoR, TotalS, RatingLim);

            // get users that have viewed the Movie, and add it to the viewer history
            getline(is, alluser);
            getline(is, alluser);
            if(is.fail()){
                throw ParserError(error_msg_2);
            }
            stringstream ss(alluser);
            string user;
            while(ss >> user){
                temp->addViewer(user);
            }

            // when all the things are done, insert the content(movie) to the content pointer vector
            content.push_back(temp);
        } else if (type == 1){
            // series

            // take all the required information to construct a Series
            stringstream ssseries;
            for(int i=0; i<4; i++){
                while(is.peek() == '\n' || is.peek() == '\t' || is.peek() == ' '){
                    is.get();
                }
                string temp;
                is >> temp;
                if(is.fail()){
                    throw ParserError(error_msg_2);
                }
                ssseries << temp << " ";
            }
            ssseries >> NoR >> TotalS >> RatingLim >> episode;
            if(ssseries.fail()){
                throw ParserError(error_msg_2);
            }

            // dynamically allocate a Movie
            Series* temp = new Series(id, title, NoR, TotalS, RatingLim, episode);

            // get users that have viewed the Movie, and add it to the viewer history
            getline(is, alluser);
            getline(is, alluser);
            if(is.fail()){
                throw ParserError(error_msg_2);
            }
            stringstream ss2(alluser);
            string user;
            while(ss2 >> user){
                temp->addViewer(user);
            }

            // when all the things are done, insert the content(movie) to the content pointer vector
            content.push_back(temp);
        }
    }

    // make sure I am in the next line (line with the username, ratinglimit...)
    while(is.peek() == '\n' || is.peek() == '\t' || is.peek() == ' '){
        is.get();
    }

    string parseuser;
    int ratinglim;
    // while I am able to get user, keep going
    while( is >> parseuser ){
        //get ratinglim value
        is >> ratinglim;
        if(is.fail()){
            throw ParserError(error_msg_3);
        }

        //dynamically allocate the user
        User* temporary = new User(parseuser, ratinglim);
        users.push_back(temporary);
        //cout << "adding user " << parseuser << " to user vector" << endl;
        
        //get the viewed content and add it to user's history
        string contentviewed;
        int contentID;
        getline(is, contentviewed);
        getline(is, contentviewed);
        stringstream ss3(contentviewed);
        while(ss3 >> contentID){
            //cout << parseuser << " viewed" << contentID << endl;
            temporary->addToHistory(contentID);
        }
    }
}