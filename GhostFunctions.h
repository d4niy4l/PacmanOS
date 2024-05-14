#include "Globals.h"
#include <cmath> 

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
    } 
	else {
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
  return ""
  ;
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

pair<int, int> getNearestPath(int currX, int currY) {
  struct Node{
	int x;
	int y;
  };
  vector<vector<int>> visited(maze.size(),
                              vector<int>(maze[0].size(), 0));

  queue<Node> q;
  q.push({currX, currY});
  while (!q.empty()) {
    Node n = q.front();
    q.pop();
    if (n.x < 0 || n.x > maze[0].size() || n.y < 0 || n.y > maze.size()) {
      continue;
    }
    if (visited[n.x][n.y]) {
      continue;
    }
    visited[n.x][n.y] = 1;
    if (maze[n.x][n.y] == 1 || maze[n.x][n.y] == 2 ||maze[n.x][n.y] == 3 ) {
      return pair<int, int>(n.x, n.y);
    } else {
      q.push({n.x + 1, n.y});
      q.push({n.x - 1, n.y});
      q.push({n.x, n.y + 1});
      q.push({n.x, n.y - 1});
    }
  }
  return pair<int, int>(1, 1);
}

pair<int,int> get_target(Ghost& g, int py, int px, char dir){
	int g_x = g.x/25;
	int g_y = g.y/25;
    switch(g.id){
        case blinky: //Blinky: Target Block is Pacs position
			if(g.chaseMode == true){
				return pair<int,int>(py,px);
			}
			else{
				if(g.scatter_targets.front().first == g_y && g.scatter_targets.front().second == g_x){
					pair<int,int> prev_target =  g.scatter_targets.front();
					g.scatter_targets.pop();
					g.scatter_targets.push(prev_target);
				}
			}
			return g.scatter_targets.front();
        case pinky: //Pinky: Target Block is 4 points infront of the direction Pac is facing
			if(g.chaseMode == true){
				int y_pos = py;
				int x_pos = px;
				if(dir == 'u'){
					if(y_pos - 4 > 0 && (maze[y_pos-4][x_pos]!=wall && maze[y_pos-4][x_pos] != hole)){
						y_pos -=4;
					}
				}
				else if(dir == 'd'){
					if(y_pos + 4 < maze.size() && (maze[y_pos+4][x_pos]!=wall  && maze[y_pos+4][x_pos] != hole)){
						y_pos += 4;
					}
				}
				else if(dir == 'l' ){
					if(x_pos - 4 > 0 && (maze[y_pos][x_pos-4]!=wall && maze[y_pos][x_pos-4] != hole)){
						x_pos -= 4;
					}
				}
				else if(dir == 'r'){
					if((x_pos + 4 < maze[0].size())  && (maze[y_pos][x_pos+4]!=wall && maze[y_pos][x_pos+4] != hole)){
						x_pos += 4;
					}
				}
				return pair<int,int>(y_pos, x_pos);
			}
			if(g.scatter_targets.front().first == g_y && g.scatter_targets.front().second == g_x){
				pair<int,int> prev_target =  g.scatter_targets.front();
				g.scatter_targets.pop();
				g.scatter_targets.push(prev_target);
			}
			return g.scatter_targets.front();
        
        case inky:	//	INKY: Target block is in 2 front of pacman

			//WARNING INKY CHASE MDOE NEEDS FIX, CAUSES SEGMENTATION FAULTS
			if(g.chaseMode == true){
				int y_pos = py;
				int x_pos = px;
				if(dir == 'u'){
					x_pos -=2;
				}
				else if(dir == 'd'){
					x_pos += 2;
				}
				else if(dir == 'l'){
					y_pos -= 2;
				}
				else if(dir == 'r'){
					y_pos += 2;
				}

				//	GETTING BLINKY'S COORDS
				Ghost& blinky = ghosts[0];
				int blinky_x = blinky.y /25;
				int blinky_y = blinky.x /25;

				int xDiff = x_pos - blinky_x;
				int yDiff = y_pos - blinky_y;

				x_pos += xDiff;
				y_pos += yDiff;

				if (x_pos < 0) {
					x_pos = 0;
				}
				if (x_pos > maze[0].size() - 1) {
					x_pos = maze[0].size() - 1;
				}
				if (y_pos < 0) {
					y_pos = 0;
				}
				if (y_pos > maze.size() - 1) {
					y_pos = maze.size() - 1;
				}
				pair<int,int> target = getNearestPath(x_pos, y_pos);				

				return target;
				//return pair<int,int>(yDiff,xDiff);
			}
			if(g.scatter_targets.front().first == g_y && g.scatter_targets.front().second == g_x){
			pair<int,int> prev_target =  g.scatter_targets.front();
			g.scatter_targets.pop();
			g.scatter_targets.push(prev_target);
			}
			return g.scatter_targets.front();
			case clyde: //Clyde: Target Block is Pac but goes into scatter mode when he comes near Pac (Lazyboi)
				if(g.chaseMode == true){
					/**/
					int p_x = px;
					int p_y = py;
					int g_x = g.x/25;
					int g_y = g.y/25;
					float d = pow(float(p_x - g_x), 2.0) + pow(float(p_y - g_y), 2.0);
					d =  sqrt(d);
					if (d > 3 && g.chaseMode == true && clyde_timer > 5) {
						return pair<int,int>(p_y,p_x);
						clyde_timer = 0;
						g.chaseMode = true;
					}
					else if(clyde_timer > 5){
						g.chaseMode = false;
						clyde_timer = 0;
					}
					else{
						g.chaseMode = false; 
					}
				}
				if(g.scatter_targets.front().first == g_y && g.scatter_targets.front().second == g_x){
				pair<int,int> prev_target =  g.scatter_targets.front();
				g.scatter_targets.pop();
				g.scatter_targets.push(prev_target);
			}
			return g.scatter_targets.front();
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
		if(g.dir == Up) return Down;
		else if(g.dir == Down) return Up;
		else if(g.dir == Left) return Right;
		else if(g.dir == Right) return Left;
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
	pthread_mutex_lock(&readincMutex);
	readcount += 1;
	if(readcount == 1){
		pthread_mutex_lock(&readWriteCords);
	}
	pthread_mutex_unlock(&readincMutex);
	int px = pacman.x/25;
	int py = pacman.y/25;
	char dir = pacman.dir;
	pthread_mutex_lock(&readincMutex);
	readcount -= 1;
	if(readcount == 0){
		pthread_mutex_unlock(&readWriteCords);
	}
	pthread_mutex_unlock(&readincMutex);
	pair<int,int> target = get_target(g,py,px,dir);
	int gridRow = target.first;
	int gridCol = target.second;
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