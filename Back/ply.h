#ifndef PLY_H
#define PLY_H
#include <cstdlib>
#include <limits>
#include "board.h"

#if !defined(INT_MIN)
#define INT_MIN std::numeric_limits<int>::min()
#endif
#if !defined(INT_MAX)
#define INT_MAX std::numeric_limits<int>::max()
#endif

struct search_result {
   int value;
   int position;
   board* node = nullptr;

   search_result() {}
   search_result(int v, int p) : value(v), position(p) {}
};

class ply
{
	private:
		board _board;
		cell_value _curr_turn;
		char node_id;
		int _cutoff_depth;
		int _eval_id_x;
		int _eval_id_o;
		int _node_count_x;
		int _node_count_o;
		bool _x_ab;
		bool _o_ab;

		//board* _current_board;

		//Finding the best move from successors
		search_result find_best_move(board b, cell_value player);
		search_result _min_max_search(board b, int depth, bool maximizing);
		search_result _alpha_beta_search(board b, int alpha, int beta, int depth, bool maximizing);
	public:
		board& get_borad() { return _board; }
		//Clone the root board, and generate ply of count 'depth'
		shared_ptr<board> generate_ply_depth(int depth);
		search_result check_best_move(board* root);

		//Start the game by specifying the evaluation method desired:
		//----1: default
		//----2: defense
		//----3: offense
		//----4: spot
		ply(int eval_id_x, int eval_id_o, int depth, bool x_ab, bool o_ab);
		//~ply();
		//Changes the value of specified cell, if not already filled
		bool make_move(int index, cell_value player);
		bool make_move2(int index);
		//Print current player
		const cell_value& get_player();
		//evaluation that uses the magic square to take the current player's score versus the opponent's
		int default_eval(board position, cell_value player);
		//evaluation that has the current player only think about defense (blocking the other players moves)
		int eval_defense(board position, cell_value player);
		//evaluation that has the current player only think about offense (not caring what the other player is doing)
		int eval_offense(board position, cell_value player);
		//evaluation that has the current player first try corner, center, and then non-corner pieces first.
		int eval_spot(board position, cell_value player);
		//generates next move based in search strategy
		int gen_move();
		//Accessor for the board's successors
		//vector<board> get_successors(board board, cell_value player);
		//Get the node id
		const char& get_node_id();
		//Switch to the next player
		cell_value switch_player(cell_value player);
		//Print out the successors
		void print_successors();
		//Print the current score for the current player
		void print_score(board& curr, cell_value player);
		//Find if the board is terminal
		bool is_terminal();
		//Find if player is x
		bool is_x(cell_value player);
		//Find if player is o
		bool is_o(cell_value player);
		//Count up desired spots
		int desired_spots(board current, cell_value _value);
		//Get node count
		int get_node_count_x();
		int get_node_count_o();
};

#endif // PLY_H
