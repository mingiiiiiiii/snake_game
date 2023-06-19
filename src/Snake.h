#ifndef SNAKE
#define SNAKE
#include <vector>

using namespace std;

class Snake
{
public:
    vector<vector<int>> body;
    vector<int> empty_body{15, 30};

    // Constructor
    Snake()
    {
        vector<int> head{2, 30};
        vector<int> head2{2, 29};
        vector<int> head3{2, 28};
        body.push_back(head);
        body.push_back(head2);
        body.push_back(head3);
    };

    // size up
    void size_up();
    void size_down();
    // move snake
    void move(int command);
};
#endif
