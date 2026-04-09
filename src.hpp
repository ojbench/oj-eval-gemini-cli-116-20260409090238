#ifndef SRC_HPP
#define SRC_HPP
#include <iostream>
#include <utility>
#include <vector>
#include <deque>
#include <string>

struct Map;
enum class instruction{UP, DOWN, LEFT, RIGHT, NONE};
//the instruction of the snake

bool is_food(Map* map, int x, int y);
//return true if the cell at (x,y) is food

bool is_wall(Map* map, int x, int y);
//return true if the cell at (x,y) is wall

void eat_food(Map* map, int x, int y);
//remove the food at (x,y)

int get_height(Map* map);
//return the height of the map

int get_width(Map* map);
//return the width of the map


struct Snake{
  std::deque<std::pair<int, int>> body;
  instruction current_dir;

  void initialize(int x, int y, instruction ins){
    body.clear();
    body.push_back({x, y});
    current_dir = ins;
  }
  int get_length(){
    return body.size();
  }
  bool move(Map* map, instruction ins){
    if (ins == instruction::NONE) {
        ins = current_dir;
    }
    if ((current_dir == instruction::UP && ins == instruction::DOWN) ||
        (current_dir == instruction::DOWN && ins == instruction::UP) ||
        (current_dir == instruction::LEFT && ins == instruction::RIGHT) ||
        (current_dir == instruction::RIGHT && ins == instruction::LEFT)) {
        return false;
    }
    current_dir = ins;

    int nx = body.front().first;
    int ny = body.front().second;
    if (ins == instruction::UP) nx--;
    else if (ins == instruction::DOWN) nx++;
    else if (ins == instruction::LEFT) ny--;
    else if (ins == instruction::RIGHT) ny++;

    if (nx < 0 || nx >= get_height(map) || ny < 0 || ny >= get_width(map)) {
        return false;
    }
    if (is_wall(map, nx, ny)) {
        return false;
    }

    bool ate_food = is_food(map, nx, ny);

    if (!ate_food) {
        body.pop_back();
    }

    for (auto p : body) {
        if (p.first == nx && p.second == ny) {
            return false;
        }
    }

    body.push_front({nx, ny});

    if (ate_food) {
        eat_food(map, nx, ny);
    }

    return true;
  }
  std::pair<int, std::pair<int,int>*> get_snake(){
    int len = body.size();
    std::pair<int, int>* arr = new std::pair<int, int>[len];
    for (int i = 0; i < len; ++i) {
        arr[i] = body[i];
    }
    return {len, arr};
  }
};

const int MaxWidth = 20;
struct Map{
  bool wall[MaxWidth][MaxWidth];
  bool food[MaxWidth][MaxWidth];
  int width, height;
  int get_height(){
    return height;
  }
  int get_width(){
    return width;
  }
  bool is_food(int x, int y){
    return food[x][y];
  }
  void eat_food(int x, int y){
    food[x][y] = false;
  }
  bool is_wall(int x, int y){
    return wall[x][y];
  }

  void initialize(Snake *snake){
    std::cin >> height >> width;
    std::string str;
    int head_x = -1 , head_y = -1;
    instruction ins;
    for (int i = 0; i < height; ++i) {
        std::cin >> str;
        for (int j = 0; j < width; ++j) {
            if (str[j] == '#') {
                wall[i][j] = true;
                food[i][j] = false;
            } else if (str[j] == '*') {
                wall[i][j] = false;
                food[i][j] = true;
            } else if (str[j] == '@') {
                wall[i][j] = false;
                food[i][j] = false;
                head_x = i;
                head_y = j;
            } else {
                wall[i][j] = false;
                food[i][j] = false;
            }
        }
    }
    char dir;
    std::cin >> dir;
    if (dir == 'U') ins = instruction::UP;
    else if (dir == 'D') ins = instruction::DOWN;
    else if (dir == 'L') ins = instruction::LEFT;
    else if (dir == 'R') ins = instruction::RIGHT;
    else ins = instruction::NONE;

    snake->initialize(head_x, head_y, ins);
  }
  void print(Snake *snake){
    auto snake_body = snake->get_snake();
    int len = snake_body.first;
    std::pair<int, int>* body = snake_body.second;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (i == body[0].first && j == body[0].second) {
                std::cout << '@';
            } else {
                bool is_body = false;
                for (int k = 1; k < len; ++k) {
                    if (i == body[k].first && j == body[k].second) {
                        is_body = true;
                        break;
                    }
                }
                if (is_body) {
                    std::cout << 'o';
                } else if (wall[i][j]) {
                    std::cout << '#';
                } else if (food[i][j]) {
                    std::cout << '*';
                } else {
                    std::cout << '.';
                }
            }
        }
        std::cout << std::endl;
    }
    delete[] body;
  }
};

struct Game{
  Map *map;
  Snake *snake;
  int score;
  int round;
  ~Game() {
    delete map;
    delete snake;
  }
  void initialize(){
    map = new Map();
    snake = new Snake();
    map->initialize(snake);
    score = 0;
    round = 0;
  }
  bool step()
  {
    char str[MaxWidth];
    if (!(std::cin >> str)) return false;
    instruction ins;
    switch(str[0]){
      case 'U':
        ins = instruction::UP;
        break;
      case 'D':
        ins = instruction::DOWN;
        break;
      case 'L':
        ins = instruction::LEFT;
        break;
      case 'R':
        ins = instruction::RIGHT;
        break;
      default:
        ins = instruction::NONE;
    }
    if(snake->move(map, ins)){
      score++;
    }else{
      return false;
    }
    return true;
  }
  void print(){
    std::cout<< "Round: " << round << std::endl;
    map->print(snake);
  }
  void play()
  {
    while(step()){
      round++;
      print();
    }
    score += snake->get_length() ;
    std::cout << "Game Over" << std::endl;
    std::cout << "Score: " << score << std::endl;
  }
};
#endif
