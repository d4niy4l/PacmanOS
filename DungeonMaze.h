#pragma once
#include <vector>
enum {
    wall = 0,
    empt = 1,
    food = 2,
    door = 4,
    powr = 3,
    hole = 5,
    hous = 6
};

const int maze_offset_x = 162;
const int maze_offset_y = 125;
std :: vector< std :: vector<int>> dungeon = {
{wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall},
{wall, powr, food, food, food, food, food, food, food, food, food, wall, food, food, food, food, food, food, food, food, food, powr, wall},
{wall, food, wall, wall, wall, food, wall, wall, wall, wall, food, wall, food, wall, wall, wall, wall, food, wall, wall, wall, food, wall},
{wall, food, wall, wall, wall, food, wall, wall, wall, wall, food, wall, food, wall, wall, wall, wall, food, wall, wall, wall, food, wall},
{wall, food, food, food, food, food, food, food, food, food, food, food, food, food, food, food, food, food, food, food, food, food, wall},
{wall, food, wall, wall, wall, food, wall, food, wall, wall, wall, wall, wall, wall, wall, food, wall, food, wall, wall, wall, food, wall},
{wall, food, food, food, food, food, wall, food, food, food, food, wall, food, food, food, food, wall, food, food, food, food, food, wall},
{wall, wall, wall, wall, wall, food, wall, wall, wall, wall, food, wall, food, wall, wall, wall, wall, food, wall, wall, wall, wall, wall},
{hole, hole, hole, hole, wall, food, wall, food, food, food, food, food, food, food, food, food, wall, food, wall, hole, hole, hole, hole},
{hole, hole, hole, hole, wall, food, wall, food, wall, wall, door, door, door, wall, wall, food, wall, food, wall, hole, hole, hole, hole},
{wall, wall, wall, wall, wall, food, wall, food, wall, hous, hous, hous, hous, hous, wall, food, wall, food, wall, wall, wall, wall, wall},
{empt, food, food, food, food, food, food, food, wall, hous, hous, hous, hous, hous, wall, food, food, food, food, food, food, food, empt}, // dungeon[11][11]
{wall, wall, wall, wall, wall, food, wall, food, wall, hous, hous, hous, hous, hous, wall, food, wall, food, wall, wall, wall, wall, wall},
{hole, hole, hole, hole, wall, food, wall, food, wall, wall, wall, wall, wall, wall, wall, food, wall, food, wall, hole, hole, hole, hole},
{hole, hole, hole, hole, wall, food, wall, food, food, food, food, empt, food, food, food, food, wall, food, wall, hole, hole, hole, hole},
{wall, wall, wall, wall, wall, food, wall, food, wall, wall, wall, wall, wall, wall, wall, food, wall, food, wall, wall, wall, wall, wall},
{wall, food, food, food, food, food, food, food, food, food, food, wall, food, food, food, food, food, food, food, food, food, food, wall},
{wall, food, wall, wall, wall, food, wall, wall, wall, wall, food, wall, food, wall, wall, wall, wall, food, wall, wall, wall, food, wall},
{wall, food, food, food, wall, food, food, food, food, food, food, food, food, food, food, food, food, food, wall, food, food, food, wall},
{wall, wall, wall, food, wall, food, wall, food, wall, wall, wall, wall, wall, wall, wall, food, wall, food, wall, food, wall, wall, wall},
{wall, food, food, food, food, food, wall, food, food, food, food, wall, food, food, food, food, wall, food, food, food, food, food, wall},
{wall, food, wall, wall, wall, wall, wall, wall, wall, wall, food, wall, food, wall, wall, wall, wall, wall, wall, wall, wall, food, wall},
{wall, powr, food, food, food, food, food, food, food, food, food, food, food, food, food, food, food, food, food, food, food, powr, wall},
{wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall}
};
