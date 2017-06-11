#include "board.h"

bool is_x(cell_value player) {
	return player == cell_value::X;
}

//Find if player is o
bool is_o(cell_value player) {
	return (player == cell_value::O);
}

/*      Board Methods       */
board::board() :board(0) {
}

board::board(int depth) : _depth(depth), _values(9) {}

board::board(int depth, Player p) : _current_player(p), _depth(depth), _values(9) {}

board::board(int depth, Player p, vector<cell> values) : _current_player(p), _values(values), _depth(depth) {}

vector<cell>& board::get_board() {
	return _values;
}

vector<int> board::get_available() {
	vector<int> currently_available;
	for (int i = 0; i < 9; i++) {
		if (get_cell(i).value() == cell_value::BLANK) {
			currently_available.push_back(i);
		}
	}
	return currently_available;
}

cell board::get_cell(int index) {
	return _values[index];
}

#if _DEBUG || DEBUG || !NDEBUG
string board::test()
{
	string ss;
	for (int i = 0; i < 9; i++) {
		if (i % 3 == 0 && i != 0) {
			ss += "*";
		}
		switch (this->get_cell(i).value())
		{
		case cell_value::BLANK:
			ss += " |";
			break;
		case cell_value::X:
			ss += "X|";
			break;
		case cell_value::O:
			ss += "O|";
			break;
		}
	}
	_board_values = ss;
	return ss;
}
string board::test2()
{
	string ss;
	for (int i = 0; i < 9; i++) {

		switch (this->get_cell(i).value())
		{
		case cell_value::BLANK:
			ss += " ";
			break;
		case cell_value::X:
			ss += "X";
			break;
		case cell_value::O:
			ss += "O";
			break;
		}
	}
	return ss;
}
#endif

void board::change_cell(int position, cell_value value) {
	_values[position].value(value);
	_last_position = position;
#if _DEBUG || DEBUG || !NDEBUG
	//TEST, for debug PURPOSE
	_board_values = test();
#endif
}

vector<board>& board::successors()
{
	if (_successors.size() > 0 || this->is_game_over())
	{
		return _successors;
	}

	Player swapped_player = SWAP_PLAYER(_current_player);
	for (int i = 0; i < 9; i++)
	{
		board b(_depth + 1, swapped_player, _values);
		if (!b.get_cell(i).is_blank()) continue;
		b.change_cell(i, swapped_player);
		b.parent(this);
		_successors.push_back(b);
	}

	return _successors;
}

void board::print_board() {
	cout << endl;
	for (int i = 0; i < 9; i++) {
		if (i % 3 == 0 && i != 0) {
			cout << "\n===========\n";
		}
		cout << get_cell(i).print_value();
		if (i == 0 || i == 1 || i == 3 || i == 4 || i == 6 || i == 7) {
			cout << " || ";
		}
	}
	cout << endl;
}

bool board::find_win() {
	_xposs_win = 0;
	_oposs_win = 0;
	return is_terminal(find_product(0, 1, 2)) ||
		is_terminal(find_product(3, 4, 5)) ||
		is_terminal(find_product(6, 7, 8)) ||
		is_terminal(find_product(0, 4, 8)) ||
		is_terminal(find_product(2, 4, 6)) ||
		is_terminal(find_product(0, 3, 6)) ||
		is_terminal(find_product(1, 4, 7)) ||
		is_terminal(find_product(2, 5, 8));
}



int board::find_product(int c1, int c2, int c3) const {
	return (int)_values[c1].value() * (int)_values[c2].value() * (int)_values[c3].value();
}

bool board::is_terminal(int product) {
	switch (product) {
		//If x is in row with two blanks
	case 12:
		_xposs_win++;
		break;
		//If o is in row with two blanks
	case 20:
		_oposs_win++;
		break;
		//If two x's in row with blanks
	case 18:
		_xposs_win += 10;
		break;
		//If two o's in row with blanks
	case 50:
		_oposs_win += 10;
		break;
	case 27:
		//cout << "\nX wins!\n";
		return true;
	case 125:
		//cout << "\nO wins!\n";
		return true;
	}
	return false;
}

bool board::is_game_over() const
{
	int cell_count = 9;
	for (auto c : _values)
	{
		cell_count -= c.value() == cell_value::BLANK;
	}
	//indicates final move
	if (cell_count >= 9)
	{
		return true;
	}
	else if (_current_player == Player::X)
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
