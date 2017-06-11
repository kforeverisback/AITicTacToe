#pragma once
#include "board.h"

static int def_attk_eval_score(int product, Player p)
{
	switch (product) {
		//If x is in row with two blanks
	case 12:
		return is_x(p) ? 10 : -10;
		//If o is in row with two blanks
	case 20:
		return is_o(p) ? 10 : -10;
		break;
		//If two x's in row with blanks
	case 18:
		return is_x(p) ? 120 : -100;
		break;
		//If two o's in row with blanks
	case 50:
		return is_o(p) ? 120 : -100;
		break;
	case 27:
		return is_x(p) ? 1000 : -1000;
	case 125:
		return is_o(p) ? 1000 : -1000;
	}

	return 0;
}

static int defensive_attack_eval(board& position, cell_value player)
{
	int total_score = 0;
	total_score += def_attk_eval_score(position.find_product(0, 1, 2), player);
	total_score += def_attk_eval_score(position.find_product(3, 4, 5), player);
	total_score += def_attk_eval_score(position.find_product(6, 7, 8), player);
	total_score += def_attk_eval_score(position.find_product(0, 4, 8), player);
	total_score += def_attk_eval_score(position.find_product(2, 4, 6), player);
	total_score += def_attk_eval_score(position.find_product(0, 3, 6), player);
	total_score += def_attk_eval_score(position.find_product(1, 4, 7), player);
	total_score += def_attk_eval_score(position.find_product(2, 5, 8), player);

	//Extra mark for corner and middle position
	if (is_x(player))
	{
		total_score += (position.get_cell(0).value() == cell_value::X) ? 2 : 0 +
			(position.get_cell(2).value() == cell_value::X) ? 2 : 0 +
			(position.get_cell(4).value() == cell_value::X) ? 5 : 0 +
			(position.get_cell(6).value() == cell_value::X) ? 2 : 0 +
			(position.get_cell(8).value() == cell_value::X) ? 2 : 0;
	}
	else
	{
		total_score += (position.get_cell(0).value() == cell_value::O) ? 2 : 0 +
			(position.get_cell(2).value() == cell_value::O) ? 2 : 0 +
			(position.get_cell(4).value() == cell_value::O) ? 5 : 0 +
			(position.get_cell(6).value() == cell_value::O) ? 2 : 0 +
			(position.get_cell(8).value() == cell_value::O) ? 2 : 0;
	}

	//For MIN case the values have to be reverted
	if (is_o(player))
		total_score = -total_score;

	return total_score;
}

//evaluation that uses the magic square to take the current player's score versus the opponent's
static int default_eval(board& position, cell_value player) {

	if (position.find_win()) {
		if (is_x(player)) {
			return 27;
		}
		else if (is_o(player)) {
			return 125;
		}
	}

	if (is_x(player)) {
		return position.x_poss_win() - position.o_poss_win();
	}
	else if (is_o(player)) {
		return position.o_poss_win() - position.x_poss_win();
	}
	return -1;
}

//evaluation that has the current player only think about defense (blocking the other players moves)
static int eval_defense(board& position, cell_value player) {
	if (position.find_win()) {
		if (is_x(player)) {
			return 27;
		}
		else if (is_o(player)) {
			return 125;
		}
	}
	if (is_x(player)) {
		return position.o_poss_win();
	}
	else if (is_o(player)) {
		return position.x_poss_win();
	}
	return -1;
}

//evaluation that has the current player only think about offense (not caring what the other player is doing)
static int eval_offense(board& position, cell_value player) {
	if (position.find_win()) {
		if (is_x(player)) {
			return 27;
		}
		else if (is_o(player)) {
			return 125;
		}
	}
	if (is_x(player)) {
		return position.x_poss_win();
	}
	else if (is_o(player)) {
		return position.o_poss_win();
	}
	return -1;
}

//Count up desired spots
static int desired_spots(board current, cell_value _value) {
	int score = 0;
	for (int i = 0; i < 9; i++) {
		if (current.get_cell(i).value() == _value) {
			if (i == 4) {
				score += 10;
			}
			else if (i == 0 || i == 2 || i == 6 || i == 8)
			{
				score += 5;
			}
			else {
				score += 1;
			}
		}
	}
	return score;
}

static int eval_spot(board& position, cell_value player) {
	if (position.find_win()) {
		if (is_x(player)) {
			return 27;
		}
		else if (is_o(player)) {
			return 125;
		}
	}

	return desired_spots(position, player);
}