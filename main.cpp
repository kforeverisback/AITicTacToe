#include "ply.h"

int eval_choice(string name) {
	int choice;
	cout << "Enter evaluation desired for " + name + ": \n";
	cout << "----1. Default Evaluation\n";
	cout << "----2. Defense Evaluation\n";
	cout << "----3. Offense Evaluation\n";
	cout << "----4. Current Player Evaluation\n";
	cout << "----5. Spot Evaluation\n";
	cout << "Enter choice: ";
	cin >> choice;
	return choice;
}

bool search_choice(string name) {
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

const char* initials ="@startuml\\r\\n\n"
"scale 1224 width\\r\\n\n"
"skinparam defaultFontName Consolas\\r\\n\n"
"skinparam defaultFontSize 8\\r\\n\n"
"skinparam shadowing false\\r\\n\n"
"\n"
"title Flow Diagram for Minimax-AB(Player 1)\\r\\n\n"
"legend top right\\r\\n\n"
"<color:red>X</color> is First Player\\r\\n\n"
"<color:blue>O</color> is Second Player\\r\\n\n"
"endlegend\\r\\n\n"
"(*)-->\"_|_|_\\n_|__|_\\n | | \"\\r\\n\n";

const char* startpoint = "(*)-->\"%s\"";
const char* point = "\"%s\"-->\"%s\"";
const char* endpoint = "\"%s\"-->\"%s\n(%d)\"";
const char* endpoint_selected = "\"%s\"-->\"%s\n<color:DarkMagenta>(__%d__)</color>\"";

void write_to_file(string content, string fileName)
{
	ofstream ss;
	ss.open(fileName);
	ss << initials << content << endl;
	ss.close();

}

string puml_board_string(board& b)
{
	string ss;
	for (int i = 0; i < 9; i++) {
		if (i % 3 == 0 && i != 0) {
			ss += "\\n";
		}
		switch (b.get_cell(i).value())
		{
		case cell_value::BLANK:
			ss += "_";
			break;
		case cell_value::X:
			ss += "__<color:red>X</color>__";
			break;
		case cell_value::O:
			ss += "__<color:blue>O</color>__";
			break;
		}

		if (i != 2 && i != 5 && i != 8)
			ss += "|";
	}
	return ss;
}

string generate_puml_graph(ply& game, board& b, int depth)
{
	if (b.successors().size() == 0 || depth == 0)
		return "";

	stringstream ss;
	char buf[1024] = { 0 };
	//snprintf(buf, 1024, point, puml_board_string(b).c_str());

	//ss << initials << endl << buf;
	string m(puml_board_string(b).c_str());
	for (board& b2 : b.successors())
	{
		string m2(puml_board_string(b2).c_str());

		cout << "B2 Succ:: " << b2.successors().size() << endl;
		if (b2.successors().size() == 0)
		{
			int eval = game.evaluate(b2, game.get_current_player());
			memset(buf, 0, 1024);
			snprintf(buf, 1024, endpoint, m.c_str(), m2.c_str(), eval);
			ss << buf << endl;
		}
		else
		{
			memset(buf, 0, 1024);
			snprintf(buf, 1024, point, m.c_str(), m2.c_str());
			ss << generate_puml_graph(game, b2, depth - 1) << endl;
		}

	}

	return ss.str();
}

void comp_vs_comp_game() {
#if 0
	int choice_1P_X = eval_choice("player 1");
	bool x_ab_search = search_choice("player 1");
	int choice_2P_O = eval_choice("player 2");
	bool o_ab_search = search_choice("player 2");
	int depth = depth_choice();
#else
	int choice_1P_X = 4;
	bool x_ab_search = true;
	int choice_2P_O = 4;
	bool o_ab_search = true;
	int depth = 4;
#endif
	cout << "==========================" << endl;
	char again = 'Y';
	ply game = ply(choice_1P_X, choice_2P_O, depth, x_ab_search, o_ab_search);
	//Kushal
	board* node_after_root = nullptr;
	int count = 1;
	while (!game.get_borad().is_game_over())
	{
		auto root = game.generate_ply_depth(depth);
		auto sr = game.check_best_move(root.get());
		//string content = generate_puml_graph(game, *root, depth);
		//char buf[16] = { 00 };
		//write_to_file(content, string(itoa(count++, buf, 10))+".txt");
		board* node_after_root = sr.node;
		while (node_after_root->parent() != root.get())
		{
			node_after_root = node_after_root->parent();
		}
		game.make_move(node_after_root->get_last_position());
		game.get_borad().print_board();
		cout << "Score >>" << sr.value << endl;
		//cout << "Puml:" << endl << puml_board_string(game.get_borad());
	}


	cout << "X Node Count: " << game.get_node_count_x() << endl << "O Node Count: " << game.get_node_count_o() << endl;
	cout << "Another game? (Y/N)";
	cin >> again;
	if (toupper(again) == 'Y') {
		comp_vs_comp_game();
	}
}

int main() {
	//start_klog();
	comp_vs_comp_game();
}
