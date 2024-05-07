#include "Globals.h"
#include <math.h>

enum{
	Left = 'l',
	Right = 'r',
	Up = 'u',
	Down = 'd'
};

struct qNode {
  int x;
  int y;
  string str;
};
string getShortestPath(int x, int y, int tx, int ty, string str = "") {
  vector<vector<bool>> visited(maze.size() + 1, vector<bool>(maze[0].size()+1, false));
  queue<qNode> q;
  q.push({x, y, str});
  while (!q.empty()) {
    qNode n = q.front();
    q.pop();
    if (n.x == 0 || n.y == 0 || n.y == maze[0].size() - 1 || n.x == maze.size() - 1) {
      continue;
    }

    visited[n.x][n.y] = 1;
    if (n.x == tx && n.y == ty) {
      return n.str;
    } else {
      if (maze[n.x][n.y] == wall || maze[n.x][n.y] == hole) {
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
        case blinky: //Blinky: Target Block is Pacs position
			return (g.chaseMode == true) ? pair<int,int>((int)pacman.y/25, (int)pacman.x/25) :
			pair<int,int>(1,1);
        case pinky: //Pinky: Target Block is 4 points infront of the direction Pac is facing
			if(g.chaseMode == true){
				int y_pos = (int)pacman.y/25;
				int x_pos = (int)pacman.x/25;
				if(pacman.dir == 'u'){
					if(y_pos - 4 > 0 && (maze[y_pos-4][x_pos]!=wall && maze[y_pos-4][x_pos] != hole)){
						y_pos -=4;
					}
				}
				else if(pacman.dir == 'd'){
					if(y_pos + 4 < maze.size() && (maze[y_pos+4][x_pos]!=wall  && maze[y_pos+4][x_pos] != hole)){
						y_pos += 4;
					}
				}
				else if(pacman.dir == 'l' ){
					if(x_pos - 4 > 0 && (maze[y_pos][x_pos-4]!=wall && maze[y_pos][x_pos-4] != hole)){
						x_pos -= 4;
					}
				}
				else if(pacman.dir == 'r'){
					if((x_pos + 4 < maze[0].size())  && (maze[y_pos][x_pos+4]!=wall && maze[y_pos][x_pos+4] != hole)){
						x_pos += 4;
					}
				}
				return pair<int,int>(y_pos, x_pos);
			}
			return pair<int,int>(1,maze[1].size() - 2);
        
        case inky:	//	INKY: Target block is in 2 front of pacman
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
				if(x_pos < 0){
					x_pos *= -1;
					x_pos %= maze[0].size();
				}
				if(y_pos < 0){
					y_pos *= -1;
					y_pos %= maze.size();
				}
				if((maze[y_pos][x_pos] == wall || maze[y_pos][x_pos] == hole)){
					return pair<int,int>(p_y,p_x);
				}
				return pair<int,int>(y_pos,x_pos);
			}
            	return pair<int,int>(maze.size() - 2,1);
			case clyde: //Clyde: Target Block is Pac but goes into scatter mode when he comes near Pac (Lazyboi)
				if(g.chaseMode == true){
					/**/
					int p_x = pacman.x/25;
					int p_y = pacman.y/25;
					int g_x = g.x/25;
					int g_y = g.y/25;
					float d = pow(float(p_x - g_x), 2.0) + pow(float(p_y - g_y), 2.0);
					d =  sqrt(d);
					if (d > 3) {
						return pair<int,int>(p_y,p_x);
					}
					else{
						g.chaseMode = false;
						g.chaseTimer = 4;
					}
				}
			return pair<int,int>(maze.size() - 2,maze[1].size() - 2);
    }
    return pair<int,int>(0,0); //just so i dont get a warning
}


char junctionDecision(Ghost& g){
	string choice;
	int gridX = g.x/25;
	int gridY = g.y/25;
	switch(g.dir){
		case Up:
			if(gridY >= 0 && maze[gridY-1][gridX] != wall){ 
				choice += 'u';
			}
			if(gridX >= 0 && maze[gridY][gridX-1] != wall){
				choice += 'l';
			}
			if(gridX < maze[0].size() && maze[gridY][gridX+1] != wall){
				choice += 'r';
			}
		break;
		case Down:
			if(gridY < maze.size() && maze[gridY+1][gridX] != wall){ 
				choice += 'd';
			}
			if(gridX >= 0 && maze[gridY][gridX-1] != wall){
				choice += 'l';
			}
			if(gridX < maze[0].size() && maze[gridY][gridX+1] != wall){
				choice += 'r';
			}
		break;
		case Left:
			if(gridX >= 0 && maze[gridY][gridX-1] != wall){
				choice += 'l';
			}
			if(gridY >= 0 && maze[gridY-1][gridX] != wall){
				choice += 'u';
			}
			if(gridY < maze.size() && maze[gridY+1][gridX] != wall){ 
				choice += 'd';
			}
		break;
		case Right:
			if(gridX < maze[0].size() && maze[gridY][gridX+1] != wall){
				choice += 'r';
			}
			if(gridY >= 0 && maze[gridY-1][gridX] != wall){
				choice += 'u';
			}
			if(gridY < maze.size() && maze[gridY+1][gridX] != wall){ 
				choice += 'd';
			}
		break;
	}
	if(choice.size() == 0){
		return g.dir;
	}
	return choice[rand()%choice.size()];
}

void checkAndMove(Ghost& g, char currdir){
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


void moveGhost(Ghost& g) {
	int px = pacman.x/25;
	int py = pacman.y/25;
	pair<int,int> target = get_target(g);
	int gridRow = target.first;
	int gridCol = target.second;
	//int gridRow = py/25;
	//int gridCol = px/25;
	
	char currdir; 
	if(g.isScared == true){
		currdir = junctionDecision(g);
	}
	else{
		if(g.isEaten == true){
			gridRow = g.spawn_row;
			gridCol = g.spawn_col;
		}
		string path = getShortestPath(g.y/25,g.x/25,gridRow,gridCol);
		currdir = path[0];
		if(gridRow == (int)g.y/25 && gridCol == (int)g.x/25){
			if(g.isEaten == true){
				g.isEaten = false;
				g.chaseMode = true;
				g.toggle_sprite();
			}
			return;
		}
	}
	
	checkAndMove(g,currdir);
}


// switch(g.id){
		// 	case blinky:
		// 		if(px < maze[0].size()/2 && py < maze.size()/2){
		// 			gridRow = 8;
		// 			gridCol = 11;
		// 		} 
		// 		else{
		// 			gridRow = 1;
		// 			gridCol = 1;
		// 		}
		// 	break;
		// 	case pinky:
		// 		if(px >= maze[0].size()/2 && py < maze.size()/2){
		// 			gridRow = 8;
		// 			gridCol = 11;
		// 		} 
		// 		else{
		// 			gridRow = 1;
		// 			gridCol = maze[0].size() - 2;
		// 		}
		// 	break;
		// 	case inky:
		// 		if(px < maze[0].size()/2 && py >= maze.size()/2){
		// 			gridRow = 8;
		// 			gridCol = 11;
		// 		} 
		// 		else{
		// 			gridRow = maze.size()-2;
		// 			gridCol = 1;
		// 		}
		// 	break;
		// 	case clyde:
		// 		if(px >= maze[0].size()/2 && py >= maze.size()/2){
		// 			gridRow = 8;
		// 			gridCol = 11;
		// 		} 
		// 		else{
		// 			gridRow = maze.size()-2;
		// 			gridCol = maze[0].size()-2;
		// 		}
		// 	break;