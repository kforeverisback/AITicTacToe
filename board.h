#pragma once

#include "stdafx.h"

using namespace std;
enum class cell_value
{
	BLANK = 2,
	X = 3, //MAX
	O = 5 //MIN
};

using Player = cell_value;

//SWAP PLAYER MACRO
#define SWAP_PLAYER(_X_) (_X_ == cell_value::X)?cell_value::O:cell_value::X
//Find if player is x
bool is_x(cell_value player);
//Find if player is o
bool is_o(cell_value player);

class cell
{
private:
	cell_value _value = cell_value::BLANK;
	int _position = -1;
	const char* cell_value_chars[6] = { "",""," ","X","","O" };
public:
	cell() {}
	//The default value of a cell is blank
	cell(int position, cell_value v) :_value(v), _position(position) {}

	//Accessors of instance variables
	const cell_value value() const
	{
		return _value;
	}
	void value(cell_value cv)
	{
		_value = cv;
	}

	//Returns if the cell is currently filled or blank
	bool is_blank() const
	{
		return _value == cell_value::BLANK;
	}

	//Prints out the value of the cell
	string print_value() {
		return string(cell_value_chars[(int)_value]);
	}

	//Compare cells
	bool equals(cell c) const
	{
		return c.value() == _value;
	}
};

class board
{
private:
#if DEBUG|_DEBUG|!NDEBUG
	// 0 | 1 | 2
	//============
	// 3 | 4 | 5
	//============
	// 6 | 7 | 8
	//
	//For Debugging purpose
	string _board_values;
#endif
	Player _current_player;
	board* _parent = nullptr;
	vector<cell> _values;
	vector<board> _successors;
	int _xposs_win = 0;
	int _oposs_win = 0;
	int _depth = 0;
	int _last_position = -1;

public:
#if _DEBUG || DEBUG || !NDEBUG
	string test();
	string test2();
#endif
	board();
	board(int depth);
	board(int depth, Player p);
	board(int depth, Player p, vector<cell> values);

	//Accessors
	vector<cell>& get_board();
	vector<board>& successors();
	int x_poss_win() { return _xposs_win; }
	int o_poss_win() { return _oposs_win; };

	//Sets and gets the depth of the board
	int depth() const { return _depth; }
	void depth(int depth) { _depth = depth; }
#if DEBUG || _DEBUG || !NDEBUG
	string tag() { test(); return _board_values; }
#else
	string tag() { return ""; }
#endif
	//Sets and gets the Parent
	board* parent() const { return _parent; }
	void parent(board* parent) { _parent = parent; }

	//Set player (MIN or MAX)
	void player(Player p) { _current_player = p; }
	Player player() const { return _current_player; }

	//Gets the last changed position
	int get_last_position() const { return _last_position; }

	cell get_cell(int position);
	vector<cell> values() const { return _values; }
	vector<int> get_available();

	//Prints the Board in a formatted fashion
	void print_board();

	//Checks each row combination to see if there is a possible win or not
	bool find_win();

	//Change cells
	void change_cell(int position, cell_value value);

	//Finds the product of each row
	int find_product(int c1, int c2, int c3) const;

	//Finds if the game is won by either player and calculates the score
	bool is_terminal(int product);

	//Finds if the game is won or tie by either player for this board
	bool is_game_over() const;
};
