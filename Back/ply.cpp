#include "ply.h"

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

const cell_value& ply::get_player(){
	return _curr_turn;
}

const char& ply::get_node_id(){
	return node_id;
}

bool ply::make_move2(int index){
	//Checks to see if game is over
	if(_board.find_win()){
		return false;
	}
	//Check to see if spot is filled
	else if(!_board.get_cell(index).is_blank()){
		return false;
	}
	else {
		cout << endl << (int)_curr_turn;

		_board.depth(_board.depth() + 1);
		_board.change_cell(index,_curr_turn);
		//_board.print_board();
		//print_score(_board, _curr_turn);
		switch_player(_curr_turn);
		_board.player(SWAP_PLAYER(_curr_turn));
		return true;
	}
}

bool ply::make_move(int index, cell_value player){
	//Checks to see if game is over
	if(_board.find_win()){
		return false;
	}
	//Check to see if spot is filled
	else if(!_board.get_cell(index).is_blank()){
		return false;
	}
	else {
		cout << endl << (int)player;
		_board.change_cell(index,player);
		_board.print_board();
		print_score(_board, _curr_turn);
		switch_player(player);
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
		for (board& s : b->successors())
		{
			if(s.depth() < targetDepth)
				all_boards.push(&s);
		}
	}

	return gen_b;
}

#include "search_algo.h"
struct search_result ply::check_best_move(board* root)
{
	int alpha = NEG_INF, beta = POS_INF;
	int cutOffActualDepth = root->depth() + _cutoff_depth;
	return alpha_beta_search(root, cutOffActualDepth, _curr_turn, alpha, beta);
}

void ply::print_score(board& curr, cell_value player){
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
			default:
				score = eval_spot(curr,player);
				break;
		}
		//curr.value(score);
	 cout << "Score: " << score << endl << endl;
}

search_result ply::find_best_move(board b, cell_value player){
	int best_score = INT_MIN;
	int def_score = INT_MAX;
	int curr_score = 0;
	int max_board = 0;
	int eval_id = is_x(_curr_turn) ? _eval_id_x : _eval_id_o;


	//Kushal
	vector<board> successors = b.successors();
	//vector<board> successors = get_successors(b, player);

	for(unsigned i = 0; i < successors.size(); i++){
		if(eval_id == 2){
			curr_score = eval_defense(successors[i],_curr_turn);
			if(curr_score < def_score){
				best_score = curr_score;
				max_board = _board.get_available()[i];
			}
		}
		else{
			switch(eval_id){
				case 1:
					curr_score = default_eval(successors[i],_curr_turn);
					break;
				case 3:
					curr_score = eval_offense(successors[i],_curr_turn);
					break;
				case 4:
					curr_score = eval_spot(successors[i],_curr_turn);
					break;
			}
			if(curr_score > best_score){
				best_score = curr_score;
				max_board = _board.get_available()[i];
			}
		}
		//successors[i].set_value(curr_score);
	}
	return search_result(curr_score, max_board);
}

int ply::default_eval(board position, cell_value player){

	if(position.find_win()) {
		if(is_x(player)){
			return 27;
		}
		else if(is_o(player)){
			return 125;
		}
	}

	if(is_x(player)){
		return position.x_poss_win() - position.o_poss_win();
	}
	else if(is_o(player)){
		return position.o_poss_win() - position.x_poss_win();
	}
	return -1;
}

int ply::eval_defense(board position, cell_value player){
	if(position.find_win()) {
		if(is_x(player)){
			return 27;
		}
		else if(is_o(player)){
			return 125;
		}
	}
	if(is_x(player)){
		return position.o_poss_win();
	}
	else if(is_o(player)){
		return position.x_poss_win();
	}
	return -1;
}

int ply::eval_offense(board position, cell_value player){
	if(position.find_win()) {
		if(is_x(player)){
			return 27;
		}
		else if(is_o(player)){
			return 125;
		}
	}
	if(is_x(player)){
		return position.x_poss_win();
	}
	else if(is_o(player)){
		return position.o_poss_win();
	}
	return -1;
}

int ply::eval_spot(board position, cell_value player){
	if(position.find_win()) {
		if(is_x(player)){
			return 27;
		}
		else if(is_o(player)){
			return 125;
		}
	}

	return desired_spots(position, player);
}

int ply::gen_move() {
   bool max = false;
   bool ab_search = false;
   if (is_x(_curr_turn)) {
	  max = true;
	  ab_search = _x_ab;
   }
   else
	  ab_search = _o_ab;

   if (ab_search)
	  return _alpha_beta_search(_board, INT_MIN, INT_MAX, _cutoff_depth, max).position;
   return _min_max_search(_board, _cutoff_depth, max).position;
}

//vector<board> ply::get_successors(board b, cell_value curr_turn) {
//	vector<board> successors;
//
//	for(int i: b.get_available()){
//		board temp = _board;
//		temp.depth(temp.depth() + 1);
//		temp.change_cell(i, curr_turn);
//		successors.push_back(temp);
//	}
//
//	return successors;
//}

/*void ply::print_successors(){
	cout << "Successors: ";
	for(board: get_successors(board, _curr_turn)){
		b.print_board();
	}
	cout << endl;
}*/

cell_value ply::switch_player(cell_value player){
	if(is_x(player)){
		_curr_turn = cell_value::O;
	}
	else if(is_o(player)){
		_curr_turn = cell_value::X;
	}
	return _curr_turn;
}

bool ply::is_terminal(){
	return _board.find_win();
}

bool ply::is_x(cell_value player){
	return player == cell_value::X;
}

bool ply::is_o(cell_value player){
	return (player == cell_value::O);
}

int ply::desired_spots(board current, cell_value _value){
	int score = 0;
	for(int i = 0; i < 9; i++){
		if (current.get_cell(i).value() == _value){
			if(i == 4){
				score += 10;
			}
			else if(i == 0 || i == 2 || i == 6 || i == 8)
			{
				score += 5;
			}
			else{
				score += 1;
			}
		}
	}
	return score;
}

search_result ply::_alpha_beta_search(board b, int alpha, int beta, int depth, bool maximizing) {
   cell_value player = maximizing ? cell_value::X : cell_value::O;
   //Kushal
	vector<board> boards = b.successors();
	//vector<board> boards = get_successors(b, player);
   if (depth < 1 ||  boards.size() < 1) {
	  return find_best_move(b, player);
   }

   if (maximizing) {
	  search_result best_path(alpha, 0); // Initially INT_MIN
	  for (unsigned i = 0; i < boards.size(); i++) {
		 _node_count_x++;
		 search_result succ_path = _alpha_beta_search(boards[i], alpha, beta, depth - 1, false);
		 if ( best_path.value < succ_path.value) {
			best_path = succ_path;
			alpha = best_path.value;
		 }
		 if (beta <= alpha)
			return best_path;
	  }
	  return best_path;
   }
   else {
	  search_result best_path(beta, 0); // Initially INT_MAX
	  for (unsigned i = 0; i < boards.size(); i++) {
		 _node_count_o++;
		 search_result succ_path = _alpha_beta_search(boards[i], alpha, beta, depth - 1, true);
		 if (best_path.value > succ_path.value) {
			best_path = succ_path;
			beta = best_path.value;
		 }
		 if (beta > alpha)
			return best_path;
	  }
	  return best_path;
   }
}

search_result ply::_min_max_search(board b, int depth, bool maximizing) {

   cell_value player = maximizing ? cell_value::X : cell_value::O;
   //Kushal
	vector<board> boards = b.successors();
	//vector<board> boards = get_successors(b, player);
   if (depth < 1 ||  boards.size() < 1) {
	  return find_best_move(b, player);
   }

   if (maximizing) {
	  search_result best_path(INT_MIN, 0);
	  for (unsigned i = 0; i < boards.size(); i++) {
		 _node_count_x++;
		 search_result succ_path = _min_max_search(boards[i], depth - 1, false);
		 if (best_path.value < succ_path.value)
			best_path = succ_path;
	  }
	  return best_path;
   }
   else {
	  search_result best_path(INT_MAX, 0);
	  for (unsigned i = 0; i < boards.size(); i++) {
		 _node_count_o++;
		 search_result succ_path = _min_max_search(boards[i], depth - 1, true);
		 if (best_path.value > succ_path.value)
			best_path = succ_path;
	  }
	  return best_path;
   }
}

int ply::get_node_count_x() {
   return _node_count_x;
}

int ply::get_node_count_o() {
   return _node_count_o;
}
