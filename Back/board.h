#pragma once

#include "stdafx.h"

using namespace std;
static  char* cell_value_chars[] = { "",""," ","X","","O" };
enum class cell_value
{
	BLANK = 2,
	X = 3, //MAX
	O = 5 //MIN
};

using Player = cell_value;

#define SWAP_PLAYER(_X_) (_X_ == cell_value::X)?cell_value::O:cell_value::X

class cell
{
private:
	cell_value _value = cell_value::BLANK;
	int _position = -1;
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
	//TEST
	Player _current_player;
	board* _parent = nullptr;
	vector<cell> _values;
	vector<board> _successors;
	bool _is_terminal = false;
	int _xposs_win = 0;
	int _oposs_win = 0;
	int _depth;
	int _last_position = -1;

public:
#if _DEBUG || DEBUG

	string _board_values;
	// 0 | 1 | 2 
	//============
	// 3 | 4 | 5
	//============
	// 6 | 7 | 8
	//

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
	string tag() { test(); return _board_values; }

	//Sets and gets the Parent
	board* parent() const { return _parent; }
	void parent(board* parent) { _parent = parent; }

	//Set player (MIN or MAX)
	void player(Player p) { _current_player = p; }
	Player player() const { return _current_player; }

	//Gets the last changed position
	int get_last_position() const { return _last_position; }


	/*int evaluate()
	{
		bool isX = _current_player == Player::X,
			isY = _current_player == Player::O;
		if (this->find_win()) {
			if (isX) {
				return 27;
			}
			else if (isY) {
				return -125;
			}
		}

		if (isX) {
			return x_poss_win() - o_poss_win();
		}
		else if (isY) {
			return -o_poss_win() - x_poss_win();
		}
		return -1;
	}*/
	int evaluate(Player p);

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
	int find_product(int c1, int c2, int c3);

	//Finds if the game is won by either player
	bool is_terminal(int product);
	bool is_terminal2()
	{
		if (_current_player == Player::X)
		{
			return  find_product(0, 1, 2) == 27 ||
				find_product(3, 4, 5) == 27 ||
				find_product(6, 7, 8) == 27 ||
				find_product(0, 4, 8) == 27 ||
				find_product(2, 4, 6) == 27 ||
				find_product(0, 3, 6) == 27 ||
				find_product(1, 4, 7) == 27 ||
				find_product(2, 5, 8) == 27;
		}
		else
		{
			return  find_product(0, 1, 2) == 125 ||
				find_product(3, 4, 5) == 125 ||
				find_product(6, 7, 8) == 125 ||
				find_product(0, 4, 8) == 125 ||
				find_product(2, 4, 6) == 125 ||
				find_product(0, 3, 6) == 125 ||
				find_product(1, 4, 7) == 125 ||
				find_product(2, 5, 8) == 125;
		}
	}
};
