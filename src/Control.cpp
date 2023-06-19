#include "Control.h"

bool _signal = true;

int snake_size_goal;    
int plus_item_goal;
int minus_item_goal;
int gate_item_goal = 2; // 게이트 미션 고정

// when receive alarm, set _signal true
void sig_alrm(int signum)
{
    _signal = false;
}

void Game_Handler::game_control(int arr[][60], int &i)
{
    switch (i)
    {
    case 0:
        snake_color = 4;
        speed = 100000;
        snake_size_goal = 5;
        plus_item_goal = 4;
        minus_item_goal = 4;
        break;
    case 1:
        snake_color = 5;
        speed = 70000;
        snake_size_goal = 5;
        plus_item_goal = 3;
        minus_item_goal = 3;
        break;
    case 2:
        snake_color = 6;
        speed = 40000;
        snake_size_goal = 5;
        plus_item_goal = 2;
        minus_item_goal = 2;
        break;
    case 3:
        snake_color = 4;
        speed = 20000;
        snake_size_goal = 5;
        plus_item_goal = 1;
        minus_item_goal = 1;
        break;
    }

    snake_size_score = "size:3";
    plus_item_score = "+:0";
    minus_item_score = "-:0";

    plus_item_count = 0;
    minus_item_count = 0;
    gate_item_count = 0;

    snake = Snake();
    signal(SIGALRM, sig_alrm);
    command = 2;
    run_game(arr, i);
}

void Game_Handler::run_game(int m4[][60], int &i)
{
    string level = "level " + to_string(i + 1);
    mvwprintw(board.score_board,3,5,level.c_str());
    mvwprintw(board.score_board, 5, 5, snake_size_score.c_str());
    mvwprintw(board.score_board, 6, 5, plus_item_score.c_str());
    mvwprintw(board.score_board, 7, 5, minus_item_score.c_str());
    mvwprintw(board.score_board, 8, 5, score_gateCount.c_str());

    mvwprintw(board.mission_board, 5, 5, snake_size_mission_str.c_str());
    mvwprintw(board.mission_board, 6, 5, plus_item_mission_str.c_str());
    mvwprintw(board.mission_board, 7, 5, minus_item_mission_str.c_str());
    mvwprintw(board.mission_board, 8, 5, gate_item_mission_str.c_str());

    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            map.m[i][j] = m4[i][j];
        }
    }
    gate.syncMap();
    gate.gateRefresh(); // gateRefresh
    while (true)
    {
        // gate controller
        if (gate_count == GATE_CYCLE)
        {
            gate_count = -50;
            gate.gateRefresh();
        }

        // item controller
        item_setting();

        _signal = true;
        ualarm(speed, 0); // set alarm timer to 1sec
        while (_signal)
        { // receive keyboard input until alarm ringing
            keyIn = keyboard_input();
            command = keyIn != 0 ? keyIn : command; // if no keyboard input received, do prev action
        }
        snake.move(command);

        // snake head hit body
        if (map.m[snake.body[0][0]][snake.body[0][1]] == SNAKE_BODY)
        {
            break;
        }

        // snake head wall
        if (map.m[snake.body[0][0]][snake.body[0][1]] == WALL)
        {
            break;
        }

        // if snake eat item
        eat_item();

        gate.gateSensor();

        if (snake.body.size() < 3 || snake.body.size() > 10)
        {
            break;
        }

        render_map();

        if (snake.body.size() >= snake_size_goal && plus_item_count >= plus_item_goal && minus_item_count >= minus_item_goal && gate_item_count >= gate_item_goal)
        {
            i++;
            return;
        }
    }
    int temp;

    refresh();
    WINDOW *ending_board3 = newwin(30, 60, 5, 10);
    wbkgd(ending_board3, COLOR_PAIR(1));
    wattron(ending_board3, COLOR_PAIR(1));
    mvwprintw(ending_board3, 13, 27, "Game Over");
    mvwprintw(ending_board3, 15, 18, "any press Arrow key for restart...");

    wrefresh(ending_board3);
    refresh();
    int tmp = command;
    command = 999;
    while (command == 999)
    {
        keyIn = keyboard_input();
        command = keyIn != 0 ? keyIn : command;
    }
    command = tmp;

    i = 0;
    count = 0;
}

void Game_Handler::eat_item()
{
    if (map.m[snake.body[0][0]][snake.body[0][1]] == PLUSITEM || map.m[snake.body[0][0]][snake.body[0][1]] == MINUSITEM)
    {
        if (map.m[snake.body[0][0]][snake.body[0][1]] == PLUSITEM)
        {
            snake.size_up();
            plus_item_count++;
        }
        else
        {
            snake.size_down();
            minus_item_count++;
        }
        map.m[snake.body[0][0]][snake.body[0][1]] = EMPTY_SPACE;
        snake_size_score = "size:" + to_string(snake.body.size());
        plus_item_score = "+:" + to_string(plus_item_count);
        minus_item_score = "-:" + to_string(minus_item_count);

        mvwprintw(board.score_board, 5, 5, snake_size_score.c_str());
        mvwprintw(board.score_board, 6, 5, plus_item_score.c_str());
        mvwprintw(board.score_board, 7, 5, minus_item_score.c_str());
    }

    if (snake.body.size() >= snake_size_goal)
    {
        snake_size_mission_str = "size:" + to_string(snake_size_goal) + " (V)";
        mvwprintw(board.mission_board, 5, 5, snake_size_mission_str.c_str());
    }
    else
    {
        snake_size_mission_str = "size:" + to_string(snake_size_goal) + " ( )";
        mvwprintw(board.mission_board, 5, 5, snake_size_mission_str.c_str());
    }

    if (plus_item_count >= plus_item_goal)
    {
        plus_item_mission_str = "+:" + to_string(plus_item_goal) + " (V)";
        mvwprintw(board.mission_board, 6, 5, plus_item_mission_str.c_str());
    }
    else
    {
        plus_item_mission_str = "+:" + to_string(plus_item_goal) + " ( )";
        mvwprintw(board.mission_board, 6, 5, plus_item_mission_str.c_str());
    }

    if (minus_item_count >= minus_item_goal)
    {
        minus_item_mission_str = "-:" + to_string(minus_item_goal) + " (V)";
        mvwprintw(board.mission_board, 7, 5, minus_item_mission_str.c_str());
    }
    else
    {
        minus_item_mission_str = "-:" + to_string(minus_item_goal) + " ( )";
        mvwprintw(board.mission_board, 7, 5, minus_item_mission_str.c_str());
    }
}

void Game_Handler::item_setting()
{
    if (item_count == ITEM_CYCLE)
    {
        bool randomItem = rand() % 2 ? true : false;
        int rand_x = rand() % HEIGHT;
        int rand_y = rand() % WIDTH;

        while (!is_empty(rand_x, rand_y))
        {
            rand_x = rand() % HEIGHT;
            rand_y = rand() % WIDTH;
        }
        Item item(rand_x, rand_y, randomItem);
        set_item(item);
        manage_item(itemContainer);
        itemContainer.push_back(item);
        item_count = 0;
    }
    gate_count++;
    item_count++;
}

void Game_Handler::render_map()
{
    clear_map();
    set_snake();
    wrefresh(board.main_board);
    wrefresh(board.score_board);
    wrefresh(board.mission_board);
    for (size_t i = 0; i < HEIGHT; i++)
    {
        for (size_t j = 0; j < WIDTH; j++)
        {
            switch (map.m[i][j])
            {
            case EMPTY_SPACE:
                wattron(board.main_board, COLOR_PAIR(2));
                mvwprintw(board.main_board, i, j, " ");
                wattroff(board.main_board, COLOR_PAIR(2));
                break;
            case WALL:
                wattron(board.main_board, COLOR_PAIR(1));
                mvwprintw(board.main_board, i, j, " ");
                wattroff(board.main_board, COLOR_PAIR(1));
                break;
            case IMMUNE_WALL:
                wattron(board.main_board, COLOR_PAIR(1));
                mvwprintw(board.main_board, i, j, " ");
                wattroff(board.main_board, COLOR_PAIR(1));
                break;
            case SNAKE_BODY: // 3 ,4 ,
                wattron(board.main_board, COLOR_PAIR(snake_color));
                mvwprintw(board.main_board, i, j, "X");
                wattroff(board.main_board, COLOR_PAIR(snake_color));
                break;
            case SNAKE_HEAD:
                wattron(board.main_board, COLOR_PAIR(snake_color));
                mvwprintw(board.main_board, i, j, "O");
                wattroff(board.main_board, COLOR_PAIR(snake_color));
                break;
            case GATE:
                wattron(board.main_board, COLOR_PAIR(2));
                mvwprintw(board.main_board, i, j, "G");
                wattroff(board.main_board, COLOR_PAIR(2));
                break;
            case PLUSITEM:
                wattron(board.main_board, COLOR_PAIR(2));
                mvwprintw(board.main_board, i, j, "+");
                wattroff(board.main_board, COLOR_PAIR(2));
                break;
            case MINUSITEM:
                wattron(board.main_board, COLOR_PAIR(2));
                mvwprintw(board.main_board, i, j, "-");
                wattroff(board.main_board, COLOR_PAIR(2));
                break;
            }
        }

        refresh();
        wrefresh(board.main_board);
        wrefresh(board.score_board);
        wrefresh(board.mission_board);
    };
}

// erase snake before rendering snake inside map
void Game_Handler::clear_map()
{

    for (size_t i = 0; i < HEIGHT; i++)
        for (size_t j = 0; j < WIDTH; j++)
        {
            if (map.m[i][j] == SNAKE_BODY)
            {
                if (j == 0 || j == (WIDTH - 1) || i == 0 || i == (HEIGHT - 1) || (i == gate1.first && j == gate1.second) || (i == gate2.first && j == gate2.second))
                {
                    map.m[i][j] = WALL;
                }
                else
                {
                    map.m[i][j] = EMPTY_SPACE;
                }
            }
            if (gate_count == 10 && map.m[i][j] == GATE)
            {
                map.m[i][j] = WALL;
            }
        }
}

// set snake in the map array
void Game_Handler::set_snake()
{
    int tmp_x = snake.body[0][0];
    int tmp_y = snake.body[0][1];
    map.m[tmp_x][tmp_y] = SNAKE_HEAD;

    for (vector<vector<int>>::iterator i = snake.body.begin() + 1; i != snake.body.end(); i++)
    {
        tmp_x = (*i)[0];
        tmp_y = (*i)[1];
        map.m[tmp_x][tmp_y] = SNAKE_BODY;
    }
}

// return keyboard input
int Game_Handler::keyboard_input()
{
    int input = getch();
    switch (input)
    {
    case UP:
        return 1;
    case RIGHT:
        return 2;
    case LEFT:
        return 3;
    case DOWN:
        return 4;
    case KEY_BACKSPACE:
        return 5;
    }
    return 0;
}

void Game_Handler::set_item(Item item)
{
    map.m[item.x][item.y] = item.itemType ? PLUSITEM : MINUSITEM;
}

void Game_Handler::remove_item(Item item)
{
    map.m[item.x][item.y] = EMPTY_SPACE;
}

void Game_Handler::manage_item(vector<Item> &v)
{
    for (vector<Item>::iterator it = v.begin(); it != v.end(); it++)
    {
        it->lifeTime++;
    }

    for (vector<Item>::iterator it = v.begin(); it != v.end(); it++)
    {
        if (it->lifeTime == ITEM_LIFE_TIME)
        {
            remove_item(*it);
            v.erase(it);
        }
    }
}

bool Game_Handler::is_empty(int x, int y)
{
    return map.m[x][y] == EMPTY_SPACE;
}

void Gate_Handler::syncMap()
{
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if (map_of_gate.m[i][j] == 1)
            {
                gateList.push_back(pair<int, int>(i, j));
            }
        }
    }
}

void Gate_Handler::gateRefresh()
{
    int gateChoice1, gateChoice2;

    gateChoice1 = rand() % (gateList.size());
    gateChoice2 = rand() % (gateList.size());

    while (gateChoice1 == gateChoice2)
    {
        gateChoice2 = rand() % (gateList.size());
    }

    gate1 = make_pair(gateList[gateChoice1].first, gateList[gateChoice1].second);
    gate2 = make_pair(gateList[gateChoice2].first, gateList[gateChoice2].second);

    map_of_gate.m[gate1.first][gate1.second] = GATE_CYCLE;
    map_of_gate.m[gate2.first][gate2.second] = GATE_CYCLE;
}

void Gate_Handler::gateSensor()
{
    if (snake.body[0][0] == gate1.first && snake.body[0][1] == gate1.second)
    {
        gate_item_count++;
        if (gate2.first == 0)
        {
            command = 4; // down
        }
        else if (gate2.first == 29)
        {
            command = 1; // up
        }
        else if (gate2.second == 0)
        {
            command = 2; // right
        }
        else if (gate2.second == 59)
        {
            command = 3; // left
        }
        else
        {
            if (gate1.first != gate2.first && gate1.second != gate2.second)
            {
                if (command == 3)
                {
                    command = 1;
                }
                else if (command == 2)
                {
                    command = 4;
                }
                else if (command == 1)
                {
                    command = 2;
                }
                else if (command == 4)
                {
                    command = 3;
                }
            }
        }
        snake.body[0][0] = gate2.first;
        snake.body[0][1] = gate2.second;
        string state4 = "gate:" + to_string(gate_item_count);

        mvwprintw(board.score_board, 8, 5, state4.c_str());
        if (gate_item_count >= gate_item_goal)
        {
            mvwprintw(board.mission_board, 8, 5, gate_item_mission_str.c_str());
        }
    }
    else if (snake.body[0][0] == gate2.first && snake.body[0][1] == gate2.second)
    {
        gate_item_count++;
        if (gate1.first == 0)
        {
            command = 4; // down
        }
        else if (gate1.first == 29)
        {
            command = 1; // up
        }
        else if (gate1.second == 0)
        {
            command = 2; // right
        }
        else if (gate1.second == 59)
        {
            command = 3; // left
        }
        else
        {
            if (gate1.first != gate2.first && gate1.second != gate2.second)
            {
                if (command == 3)
                {
                    command = 1;
                }
                else if (command == 2)
                {
                    command = 4;
                }
                else if (command == 1)
                {
                    command = 2;
                }
                else if (command == 4)
                {
                    command = 3;
                }
            }
        }
        snake.body[0][0] = gate1.first;
        snake.body[0][1] = gate1.second;

        string state4 = "gate:" + to_string(gate_item_count);
        mvwprintw(board.score_board, 8, 5, state4.c_str());
        if (gate_item_count >= gate_item_goal)
        {
            mvwprintw(board.mission_board, 8, 5, gate_item_mission_str.c_str());
        }
    }
}