#include "Control.h"

using namespace std;

int main()
{
  srand((unsigned int)time(NULL));
  try
  {
    initscr();
  }
  catch(const std::exception& e)
  {
    cout << "cant start game!!" << endl;
    std::cerr << e.what() << '\n';
  }

  //keypad setting
  keypad(stdscr, TRUE);

  //cursor setting
  curs_set(0);
  noecho();

  //refresh background
  resize_term(200, 200);
  refresh();

  //palette
  start_color();
  init_pair(1, COLOR_BLACK, COLOR_WHITE); // wall color
  init_pair(2, COLOR_RED, COLOR_BLACK); // game screen color
  init_pair(3, COLOR_WHITE, COLOR_BLACK); // background color
  init_pair(4, COLOR_YELLOW, COLOR_BLACK); // snake color
  init_pair(5,COLOR_BLUE,COLOR_BLACK);
  init_pair(6,COLOR_CYAN,COLOR_BLACK);


  // background window
  WINDOW *main_window;
  nodelay(stdscr, true);
  main_window = newwin(40, 125, 0, 0);
  wbkgd(main_window, COLOR_PAIR(3));
  wrefresh(main_window);

  // game setting
  Game_Handler start;

  Map m;
  int i=0;
  while(i!=-1){
    switch (i) {
      case 0:
          start.game_control(m.m1,i);
          break;
        case 1:
          start.game_control(m.m2,i);
          break;
        case 2:
          start.game_control(m.m3,i);
          break;
        case 3:
          start.game_control(m.m4,i);
          break;
        }


  }


  getch();
  endwin();

  return 0;
}
