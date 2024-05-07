#include "Globals.h"
#include <math.h>
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

pair<int,int> get_target(Ghost& g){
    switch(g.id){
        case 0: //Blinky: Target Block is Pacs position
			return g.chaseMode == true ? pair<int,int>((int)pacman.x/25, (int)pacman.y/25) :
			pair<int,int>(1,1);
        case 1: //Pinky: Target Block is 4 points infront of the direction Pac is facing
			if(g.chaseMode == true){
				int x_pos = pacman.x/25;
				int y_pos = pacman.y/25;
				if(pacman.dir == 'u'){
					if(y_pos - 4 < maze.size() - 1){
						y_pos -=4;
					}
				}
				else if(pacman.dir == 'd'){
					if(y_pos + 4 > 0){
						y_pos += 4;
					}
				}
				else if(pacman.dir == 'l'){
					if(x_pos - 4 > 0){
						x_pos -= 4;
					}
				}
				else if(pacman.dir == 'r'){
					if(x_pos + 4 < maze[0].size() - 1){
						x_pos += 4;
					}
				}
				return pair<int,int>(y_pos, x_pos);
			}
			return pair<int,int>(1,maze[1].size() - 2);
        case 2: //Clyde: Target Block is Pac but goes into scatter mode when he comes near Pac (Lazyboi)
			if(g.chaseMode == true){
				/**/
				int p_x = pacman.x/25;
				int p_y = pacman.y/25;
				int g_x = g.x/25;
				int g_y = g.y/25;
				float d = pow(float(p_x - g_x), 2.0) + pow(float(p_y - g_y), 2.0);
				d =  sqrt(d);
				if (d > 8) {
					return pair<int,int>(p_x,p_y);
				}
			}
            return pair<int,int>(maze.size() - 2,maze[1].size() - 2);
        case 3:	//	INKY: Target block is in 2 front of pacman
			if(g.chaseMode == true){
				int g_x = g.x/25;
				int g_y = g.y/25;
				int p_x = pacman.x / 25;
				int p_y = pacman.y / 25;
				int y_pos = p_y;
				int x_pos = p_x;
				if(pacman.dir == 'u'){
					if(y_pos - 2 < maze.size() - 1){
						y_pos -=2;
					}
				}
				else if(pacman.dir == 'd'){
					if(y_pos + 2 > 0){
						y_pos += 2;
					}
				}
				else if(pacman.dir == 'l'){
					if(x_pos - 2 > 0){
						x_pos -= 2;
					}
				}
				else if(pacman.dir == 'r'){
					if(x_pos + 2 < maze[0].size() - 1){
						x_pos += 2;
					}
				}
				Ghost& blinky = ghosts[0];
				int yDiff = blinky.y - y_pos;
				int xDiff = blinky.x - x_pos;
				yDiff *= -1;
				xDiff *= -1;
				y_pos += yDiff;
				x_pos += xDiff;
				/*
				if(abs(g_x - p_x) < 4 && abs(g_y - p_y) < 4){
					g.chaseMode = false;
					g.chaseTimer -= 3;
					return pair<int,int>(maze.size() - 2,1);
				}
				*/
				return pair<int,int>(x_pos,y_pos);
			}
            return pair<int,int>(maze.size() - 2,1);
    }
    return pair<int,int>(0,0); //just so i dont get a warning
}

void moveGhost(Ghost& g) {
	float px;
	float py;
	px = pacman.x;
	py = pacman.y;
	pair<int,int> target = get_target(g);
	int gridRow = target.first;
	int gridCol = target.second;
	if(gridRow == (int)g.y/25 && gridCol == (int)g.x/25){
		g.chaseMode = false;
		g.chaseTimer = 0;
		return;
	}

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
