#ifndef __CONTROL__
#define __CONTROL__

#include <iostream>
#include <ncurses.h>
#include <signal.h>
#include <cstdlib>
#include <ctime>
#include <clocale>
#include <unistd.h>
#include <vector>
#include <string>

#include "Snake.h"
#include "Util.h"
#include "Map.h"

using namespace std;

extern bool _signal;

extern int snake_size_goal;
extern int plus_item_goal;
extern int minus_item_goal;
extern int gate_item_goal;

// when receive alarm, set _signal true
void sig_alrm(int signum);

class Item
{
public:
    int x;
    int y;
    bool itemType;
    int lifeTime;
    Item(int x, int y, bool itemType, int lifeTime = 5) : x(x), y(y), itemType(itemType), lifeTime(lifeTime){};
};

class Board_Handler
{
public:
    WINDOW *main_board, *score_board, *mission_board;
    Board_Handler()
    {
        start_color();

        // set main board
        main_board = newwin(30, 60, 5, 10);
        wbkgd(main_board, COLOR_PAIR(2));

        // set score board
        score_board = newwin(15, 40, 5, 73);
        wbkgd(score_board, COLOR_PAIR(1));
        wattron(score_board, COLOR_PAIR(1));
        mvwprintw(score_board, 0, 14, "Score Board");

        // set mission board
        mission_board = newwin(12, 40, 23, 73);
        wbkgd(mission_board, COLOR_PAIR(1));
        wattron(mission_board, COLOR_PAIR(1));
        mvwprintw(mission_board, 0, 14, "Mission Board");
    };
};

class Gate_Handler
{
public:
    Map &map_of_gate;
    Snake &snake;
    vector<pair<int, int>> gateList;
    pair<int, int> &gate1;
    pair<int, int> &gate2;
    int &command;
    int &gate_item_count;
    Board_Handler &board;
    int &gate_item_goal;
    string gate_item_mission_str = "gate:" + to_string(gate_item_goal) + " (V)";
    Gate_Handler(Map &map, Snake &snake, int &command, int &gate_item_count, Board_Handler &board, pair<int, int> &gate1, pair<int, int> &gate2, int &gate_item_goal) : map_of_gate(map), snake(snake), command(command), gate_item_count(gate_item_count), board(board), gate1(gate1), gate2(gate2), gate_item_goal(gate_item_goal){};

    void syncMap();

    void gateRefresh();

    void gateSensor();
};

class Game_Handler
{
public:
    friend class Item;
    vector<Item> itemContainer;
    Map map;
    int count = 0;
    Snake snake;
    Board_Handler board;
    Gate_Handler gate;
    int speed = 100000;
    int snake_color = 4;
    int keyIn, command = 2, gate_count = 0, item_count = 0;
    pair<int, int> gate1 = make_pair(0, 0);
    pair<int, int> gate2 = make_pair(0, 0);
    int plus_item_count = 0, minus_item_count = 0, gate_item_count = 0;

    string snake_size_score = "size:" + to_string(snake.body.size());
    string plus_item_score = "+:" + to_string(plus_item_count);
    string minus_item_score = "-:" + to_string(minus_item_count);
    string score_gateCount = "gate:" + to_string(gate_item_count);
    string snake_size_mission_str = "size:" + to_string(snake_size_goal) + " ( )";
    string plus_item_mission_str = "+:" + to_string(plus_item_goal) + " ( )";
    string minus_item_mission_str = "-:" + to_string(minus_item_goal) + " ( )";
    string gate_item_mission_str = "gate:" + to_string(gate_item_goal) + " ( )";

    Game_Handler() : board(), snake(), gate(map, snake, command, gate_item_count, board, gate1, gate2, gate_item_goal){};

    void game_control(int arr[][60], int &i);

    void run_game(int m4[][60], int &i);

    void eat_item();

    void item_setting();

    // refresh map array every 1sec to rendering map
    void render_map();
    // erase snake before rendering snake inside map
    void clear_map();

    // set snake in the map array
    void set_snake();

    // return keyboard input
    int keyboard_input();
    void set_item(Item item);

    void remove_item(Item item);

    void manage_item(vector<Item> &v);
    bool is_empty(int x, int y);
};

#endif // __CONTROL__