
#include "content.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>

using namespace std;
int main(int argc, char* argv[])
{
    int option = atoi(argv[1]);
    if(option == 1) {
        Movie m(1111, "Movie Title", 2, 7, 3);
        m.addViewer("user1");
        m.addViewer("user2");
        m.display(cout);
        Series s(222222, "Series Title", 3, 14, 4, 6);
        s.addViewer("user1");
        s.display(cout);
    }
    else {
        Movie m(19182, "Another Movie", 3, 10, 2);
        m.addViewer("user1");
        m.display(cout);
        Series s(67261, "Another Series", 5, 17, 1, 3);
        s.addViewer("user1");
        s.addViewer("user2");
        s.addViewer("user3");
        s.display(cout);
    }
    return 0;
}