#include "ply.h"
#include "evaluations.h"

ply::ply(int eval_id_x, int eval_id_o, int depth, bool x_ab, bool o_ab)
{
	_cutoff_depth = depth;
	
	_curr_turn = cell_value::X;
	_board = board(0, SWAP_PLAYER(_curr_turn));
	//Kushal
	//_current_board = &_board;

	_node_count_x = 0;
	_node_count_o = 0;
	_eval_id_x = eval_id_x;
	_eval_id_o = eval_id_o;
	_x_ab = x_ab;
	_o_ab = o_ab;
}

const cell_value& ply::get_current_player(){
	return _curr_turn;
}

bool ply::make_move(int index){
	//Checks to see if game is over
	if(_board.is_game_over()){
		return false;
	}
	//Check to see if spot is filled
	else if(!_board.get_cell(index).is_blank()){
		return false;
	}
	else {
		//cout << endl << (int)_curr_turn;

		_board.depth(_board.depth() + 1);
		_board.change_cell(index,_curr_turn);
		//_board.print_board();
		//print_score(_board, _curr_turn);
		switch_player(_curr_turn);
		_board.player(SWAP_PLAYER(_curr_turn));
		return true;
	}
}


shared_ptr<board> ply::generate_ply_depth(int depth)
{
	shared_ptr<board> gen_b(new board(_board));
	int targetDepth = _board.depth() + depth;
	queue<board*> all_boards;
	all_boards.push(gen_b.get());
	for (; all_boards.size() != 0; )
	{
		board* b = all_boards.front();
		all_boards.pop();
		vector<board>& succ = b->successors();
		if (b->depth() >= targetDepth-1)
			continue;
		for (board& s : b->successors())
		{
			//if(s.depth() < targetDepth)
			all_boards.push(&s);
		}
	}

	return gen_b;
}

struct search_result ply::check_best_move(board* root)
{
	int alpha_pass_thres = NEG_INF, beta_user_thres = POS_INF;
	int cutOffActualDepth = root->depth() + _cutoff_depth;
	
	if (is_x(_curr_turn)&_x_ab || is_o(_curr_turn)&_o_ab)
	{
		return alpha_beta_search(root, cutOffActualDepth, _curr_turn, alpha_pass_thres, beta_user_thres);
	}
	return minimax_ab(root, cutOffActualDepth, _curr_turn, beta_user_thres, alpha_pass_thres);
	/*if(is_x(_curr_turn))
		return _x_ab?alpha_beta_search(root, cutOffActualDepth, _curr_turn, alpha_pass_thres, beta_user_thres):minimax_ab(root, cutOffActualDepth, _curr_turn, alpha_pass_thres, beta_user_thres);
	else
		return _o_ab?alpha_beta_search(root, cutOffActualDepth, _curr_turn, alpha_pass_thres, beta_user_thres):minimax_ab(root, cutOffActualDepth, _curr_turn, alpha_pass_thres, beta_user_thres);*/
}

int ply::evaluate(board& curr, Player player)
{
	int score = 0;
	int eval_id = is_x(player) ? _eval_id_x : _eval_id_o;
	switch(eval_id){
			case 1:
				score = default_eval(curr,player);
				break;
			case 2:
				score = eval_defense(curr,player);
				break;
			case 3:
				score = eval_offense(curr,player);
				break;
			case 4:
				score = defensive_attack_eval(curr, player);
				break;
			default:
				score = eval_spot(curr,player);
				break;
		}
	return score;
}

cell_value ply::switch_player(cell_value player){
	_curr_turn = is_x(_curr_turn) ? cell_value::O : cell_value::X;
	return _curr_turn;
}

int ply::get_node_count_x() {
   return _node_count_x;
}

int ply::get_node_count_o() {
   return _node_count_o;
}
