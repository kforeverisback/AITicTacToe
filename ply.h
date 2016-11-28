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

#define POS_INF INT_MAX
#define NEG_INF INT_MIN

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
		//char node_id;
		int _cutoff_depth;
		int _eval_id_x;
		int _eval_id_o;
		int _node_count_x;
		int _node_count_o;
		bool _x_ab;
		bool _o_ab;

		//board* _current_board;

		//search_result _min_max_search(board b, int depth, bool maximizing);
		//search_result _alpha_beta_search(board b, int alpha, int beta, int depth, bool maximizing);
		struct search_result max_value_search(board* state, Player player, int cutoff_depth, int alpha, int beta);
		struct search_result min_value_search(board* state, Player player, int cutoff_depth, int alpha, int beta);
		struct search_result minimax_ab(board* pos, int depth, Player player, int use_thres, int pass_thres);
	public:
		struct search_result alpha_beta_search(board* root_state, int cutoff_depth, Player firstPlayer, int alpha, int beta);
		board& get_borad() { return _board; }
		//Clone the current board, and generate ply of count 'depth', not saving the boards
		shared_ptr<board> generate_ply_depth(int depth);
		search_result check_best_move(board* root);

		//Start the game by specifying the evaluation method desired:
		//----1: default
		//----2: defense
		//----3: offense
		//----4: Defensive Attack
		//----5: spot
		int evaluate(board& b, Player);
		ply(int eval_id_x, int eval_id_o, int depth, bool x_ab, bool o_ab);
		//~ply();
		//Changes the value of specified cell, if not already filled
		bool make_move(int index);
		//Print current player
		const cell_value& get_current_player();
		
		//Accessor for the board's successors
		//vector<board> get_successors(board board, cell_value player);
		//Switch to the next player
		cell_value switch_player(cell_value player);
		//Print out the successors
		//void print_successors();
		
		//Get node count
		int get_node_count_x();
		int get_node_count_o();

		void increase_node_count()
		{
			if (is_x(_curr_turn))
				_node_count_x++;
			else
				_node_count_o++;
		}
};

#endif // PLY_H
