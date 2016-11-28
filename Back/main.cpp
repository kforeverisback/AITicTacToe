#include "ply.h"

int eval_choice(string name) {
	int choice;
	cout << "Enter evaluation desired for " + name + ": \n";
	cout << "----1. Default Evaluation\n";
	cout << "----2. Defense Evaluation\n";
	cout << "----3. Offense Evaluation\n";
	cout << "----4. Spot Evaluation\n";
	cout << "Enter choice: ";
	cin >> choice;
	return choice;
}

int search_choice(string name) {
	int choice;
	cout << "Enter algorithm desired for " + name + ": \n";
	cout << "----1. Alpha Beta Searching\n";
	cout << "----2. MinMaxAB Searching\n";
	cout << "Enter choice: ";
	cin >> choice;
	return choice == 1;
}

int depth_choice() {
	int depth;
	cout << "Depth of searching: ";
	cin >> depth;
	return depth;
}

void comp_vs_comp_game() {
#if 0
	int choice1 = eval_choice("player 1");
	bool x_search = search_choice("player 1");
	int choice2 = eval_choice("player 2");
	bool o_search = search_choice("player 2");
	int depth = depth_choice();
#else
	int choice1 = 1;
	bool x_search = 1;
	int choice2 = 1;
	bool o_search = 1;
	int depth = 2;
#endif
	cout << "==========================" << endl;
	board test;
	char again = 'Y';
	ply _game = ply(choice1, choice2, depth, x_search, o_search);
	//Kushal
	auto root = _game.generate_ply_depth(2);
	auto sr = _game.check_best_move(root.get());
	board* node_after_root = sr.node;
	while (node_after_root->parent() != root.get())
	{
		node_after_root = node_after_root->parent();
	}
	_game.make_move2(node_after_root->get_last_position());
	_game.get_borad().print_board();
	cout << "Score >>" << sr.value << endl;

	root = _game.generate_ply_depth(2);
	sr = _game.check_best_move(root.get());
	node_after_root = sr.node;
	while (node_after_root->parent() != root.get())
	{
		node_after_root = node_after_root->parent();
	}
	_game.make_move2(node_after_root->get_last_position());
	_game.get_borad().print_board();
	cout << "Score >>" << sr.value << endl;

	root = _game.generate_ply_depth(2);
	sr = _game.check_best_move(root.get());
	node_after_root = sr.node;
	while (node_after_root->parent() != root.get())
	{
		node_after_root = node_after_root->parent();
	}
	_game.make_move2(node_after_root->get_last_position());
	_game.get_borad().print_board();
	cout << "Score >>" << sr.value << endl;

	root = _game.generate_ply_depth(2);
	for (board& vv : root->successors())
	{
		cout << vv.tag() << endl;
		for (board& vv2 : vv.successors())
		{
			cout << "|---" << vv2.tag() << ":" << vv2.evaluate(_game.get_player()) << endl ;
		}
	}
	sr = _game.check_best_move(root.get());
	node_after_root = sr.node;
	while (node_after_root->parent() != root.get())
	{
		node_after_root = node_after_root->parent();
	}
_game.make_move2(node_after_root->get_last_position());
	_game.get_borad().print_board();
	cout << "Score >>" << sr.value << endl;

	root = _game.generate_ply_depth(2);
	sr = _game.check_best_move(root.get());
	node_after_root = sr.node;
	while (node_after_root->parent() != root.get())
	{
		node_after_root = node_after_root->parent();
	}
	_game.make_move2(node_after_root->get_last_position());
	_game.get_borad().print_board();
	cout << "Score >>" << sr.value << endl;

	root = _game.generate_ply_depth(2);
	sr = _game.check_best_move(root.get());
	node_after_root = sr.node;
	while (node_after_root->parent() != root.get())
	{
		node_after_root = node_after_root->parent();
	}
	_game.make_move2(node_after_root->get_last_position());
	_game.get_borad().print_board();
	cout << "Score >>" << sr.value << endl;

	root = _game.generate_ply_depth(2);
	sr = _game.check_best_move(root.get());
	node_after_root = sr.node;
	while (node_after_root->parent() != root.get())
	{
		node_after_root = node_after_root->parent();
	}
	_game.make_move2(node_after_root->get_last_position());
	_game.get_borad().print_board();
	cout << "Score >>" << sr.value << endl;

	root = _game.generate_ply_depth(2);
	sr = _game.check_best_move(root.get());
	node_after_root = sr.node;
	while (node_after_root->parent() != root.get())
	{
		node_after_root = node_after_root->parent();
	}
	_game.make_move2(node_after_root->get_last_position());
	_game.get_borad().print_board();
	cout << "Score >>" << sr.value << endl;

	int move_num = _game.gen_move();

	/*while(_game.make_move(move_num, _game.get_player())){
		if(_game.get_successors(test, cell_value::X).size() < 0){break;}
		move_num = _game.gen_move();
	}*/
	cout << "X Nodes: " << _game.get_node_count_x() << endl;
	cout << "O Nodes: " << _game.get_node_count_o() << endl;
	cout << "Another game? (Y/N)";
	cin >> again;
	if (toupper(again) == 'Y') {
		comp_vs_comp_game();
	}
}
//#define Possible_Wins 8
//const int Three_in_a_Row[Possible_Wins][3] = {
//  { 0, 1, 2 },
//  { 3, 4, 5 },
//  { 6, 7, 8 },
//  { 0, 3, 6 },
//  { 1, 4, 7 },
//  { 2, 5, 8 },
//  { 0, 4, 8 },
//  { 2, 4, 6 }
//};
//const int Heuristic_Array[4][4] = {
//  {     0,   -10,  -100, -1000 },
//  {    10,     0,     0,     0 },
//  {   100,     0,     0,     0 },
//  {  1000,     0,     0,     0 }
//};
//
//int evaluatePosition(char board[9], char player) {
//	char opponent = (player == 'X') ? 'O' : 'X', piece;
//	int players, others, t = 0, i, j;
//
//	for (i = 0; i < 8; i++) {
//		players = others = 0;
//		for (j = 0; j < 3; j++) {
//			piece = board[Three_in_a_Row[i][j]];
//			if (piece == player)
//				players++;
//			else if (piece == opponent)
//				others++;
//		}
//		t += Heuristic_Array[players][others];
//	}
//	return t;
//}
//void print_board(const char* b) {
//	cout << endl;
//	for (int i = 0; i < 9; i++) {
//		if (i % 3 == 0 && i != 0) {
//			cout << "\n===========\n";
//		}
//		cout << b[i];
//		if (i == 0 || i == 1 || i == 3 || i == 4 || i == 6 || i == 7) {
//			cout << " || ";
//		}
//	}
//	cout << endl;
//}
int main() {
	//{
	//	char board[9] = { 'X',' ',' ',' ',' ','O',' ',' ',' ' };
	//	print_board(board);
	//	int ev = evaluatePosition(board, 'X');
	//	cout << "X: " << ev << endl;
	//	ev = evaluatePosition(board, 'O');
	//	cout << "O: " << ev << endl;
	//}
	//{
	//	char board[9] = { ' ',' ','X',/**/' ',' ',' ',/**/'O',' ','O'  };
	//	print_board(board);
	//	int ev = evaluatePosition(board, 'X');
	//	cout << "X: " << ev << endl;
	//	ev = evaluatePosition(board, 'O');
	//	cout << "O: " << ev << endl;
	//}
	//{
	//	char board[9] = { 'X',' ',' ',/**/'X',' ',' ',/**/' ',' ','O'  };
	//	print_board(board);
	//	int ev = evaluatePosition(board, 'X');
	//	cout << "X: " << ev << endl;
	//	ev = evaluatePosition(board, 'O');
	//	cout << "O: " << ev << endl;
	//}
	//{
	//	char board[9] = { 'X',' ',' ',/**/' ',' ','O',/**/'X',' ',' '  };
	//	print_board(board);
	//	int ev = evaluatePosition(board, 'X');
	//	cout << "X: " << ev << endl;
	//	ev = evaluatePosition(board, 'O');
	//	cout << "O: " << ev << endl;
	//}
	//{
	//	char board[9] = { 'X',' ',' ',/**/' ',' ','O',/**/' ',' ','X'  };
	//	print_board(board);
	//	int ev = evaluatePosition(board, 'X');
	//	cout << "X: " << ev << endl;
	//	ev = evaluatePosition(board, 'O');
	//	cout << "O: " << ev << endl;
	//}
	//start_klog();
	comp_vs_comp_game();
}
