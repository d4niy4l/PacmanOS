#include "Globals.h"
struct qNode {
  int x;
  int y;
  string str;
};
string getShortestPath(int x, int y, int tx, int ty, string str = "") {
  vector<vector<int>> visited(dungeon.size() + 1, vector<int>(dungeon[0].size()+1, 0));
  queue<qNode> q;
  q.push({x, y, str});
  while (!q.empty()) {
    qNode n = q.front();
    q.pop();
    if (n.x == 0 || n.y == 0 || n.y == dungeon[0].size() || n.x == dungeon[0].size()) {
      continue;
    }

    visited[n.x][n.y] = 1;
    if (n.x == tx && n.y == ty) {
      return n.str;
    } else {
      if (dungeon[n.x][n.y] == 0) {
        continue;
      }
      
      if (!visited[n.x + 1][n.y])
        q.push({n.x + 1, n.y, n.str + 'd'});
      if (!visited[n.x - 1][n.y])
        q.push({n.x - 1, n.y, n.str + 'u'});
      if (!visited[n.x][n.y + 1])
        q.push({n.x, n.y + 1, n.str + 'r'});
      if (!visited[n.x][n.y - 1])
        q.push({n.x, n.y - 1, n.str + 'l'});
    }
  }
  return "";
}
bool can_change(char dir, bool rowAligned, bool colAligned, Maze& maze, int gridRow, int gridCol){
    if(dir == 'u'){
        return !(!colAligned || (rowAligned && (maze[gridRow - 1][gridCol] == 0)));
    }
    else if(dir == 'd'){
        return !(!colAligned|| (maze[gridRow + 1][gridCol] == 0));
    }
    else if(dir == 'l'){
        return !(!rowAligned || (colAligned && (maze[gridRow][gridCol - 1] == 0 )));
    }
    else if(dir == 'r'){
        return !(!rowAligned || (maze[gridRow][gridCol+1] == 0));
    }
    return true;
}
void moveGhost(Ghost& g, float dT) {
  
  float px = pacman.x;
  float py = pacman.y;
  int gridRow = int((py) / 25);
  int gridCol = int((px) / 25);
  string path = getShortestPath(g.y/25,g.x/25,gridRow,gridCol);
  char currdir = path[0];
  int currX = g.y / 25;
  int currY = g.x / 25;
  int gridX = currY * 25;
  int gridY = currX * 25;
  bool isRowAligned = abs(gridY - g.y) < 5;
  bool isColAligned = abs(gridX - g.x) < 5;
  bool canSwitch = can_change(currdir,isRowAligned,isColAligned,maze,currX,currY);
  if (!canSwitch) { 
    currdir = g.dir;
  }

  if (currdir == 'u') {
    if(!(!isColAligned || (isRowAligned && (maze[currX - 1][currY] == 0)))){
      g.sprite.setPosition(gridX + maze_offset_x, g.y + maze_offset_y);
      g.sprite.move(0, -1 * g.speed);
      g.y -= g.speed; 
    }
  }else if(currdir == 'd') {
    if(!(!isColAligned|| (maze[currX + 1][currY] == 0))){
      g.sprite.setPosition(gridX + maze_offset_x,g.y + maze_offset_y );
      g.sprite.move(0, 1 * g.speed);
      g.y += g.speed; 
    }
  }else if (currdir == 'l') {
    if(!(!isRowAligned || (isColAligned && (maze[currX][currY - 1] == 0)))) {
      g.sprite.setPosition(g.x + maze_offset_x, gridY + maze_offset_y);
      g.sprite.move(-1 * g.speed, 0);    
      g.x -= g.speed; 
    }
  }else if (currdir == 'r') {
    if(!(!isRowAligned || (maze[currX][currY+1] == 0))){      
      g.sprite.setPosition(g.x + maze_offset_x, gridY + maze_offset_y);
      g.sprite.move(1 * g.speed, 0);
      g.x += g.speed; 
    }
  }
  g.dir = currdir;
}
