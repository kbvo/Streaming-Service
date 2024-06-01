
#include "content.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>

using namespace std;
int main(int argc, char* argv[])
{
    Movie m(1111, "Movie Title", 2, 7, 3);
    m.addViewer("user1");
    m.addViewer("user2");
    m.display(cout);
    Series s(222222, "Series Title", 3, 14, 4, 6);
    s.addViewer("user1");
    s.display(cout);
    return 0;
}