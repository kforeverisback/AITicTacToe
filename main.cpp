#include "stdafx.h"
#include "ply.h"

int eval_choice(string name) {
	int choice;
	cout << "Enter evaluation desired for " + name + ": \n";
	cout << "----1. Default Evaluation\n";
	cout << "----2. Defense Evaluation\n";
	cout << "----3. Offense Evaluation\n";
	cout << "----4. Defensive-Attack Evaluation\n";
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

const char* initials = "@startuml\n"
"skinparam defaultFontName Consolas\n"
"skinparam defaultFontSize 14\n"
"'skinparam shadowing false\n"
"\n";
//"title Flow Diagram for Minimax-AB(Player 1)\n"
//"legend top right\n"
//"<color:red>X</color> is First Player\n"
//"<color:blue>O</color> is Second Player\n"
//"endlegend\n\n"
//"(*)-->\"_|_|_\\n_|_|_\\n_|_|_\"\n";

const char* startpoint = "(*)-->\"%s\"";
const char* point = "\"%s\"-->\"%s\"";
const char* endpoint = "\"%s\"-->\"%s\n(%d)\"";
const char* endpoint_selected = "\"%s\"-->\"%s\n<color:DarkMagenta>Sel:(__%d__)</color>\"";

void write_to_file(string content, string fileName)
{
	ofstream ss;
	ss.open(fileName);
	ss << initials << content << "@enduml" << endl;
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
			/*if (i < 6)
				ss += "_";
			else
				ss += " ";*/
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

string generate_puml_graph(ply& game, board& b, board& selected, int depth)
{
	//if (b.successors().size() == 0 || depth == 0)
		//return "";

	stringstream ss;
	char buf[1024] = { 0 };
	//snprintf(buf, 1024, point, puml_board_string(b).c_str());

	//ss << initials << endl << buf;
	string m(puml_board_string(b).c_str());
	int max_count = 0;
	for (board& b2 : b.successors())
	{
		string m2(puml_board_string(b2).c_str());

		//cout << "B2 Succ:: " << b2.successors().size() << endl;
		if (0 == depth - 1)
		{
			const char* format = (b2.tag().compare(selected.tag()) != 0) ? endpoint : endpoint_selected;
			int eval = game.evaluate(b2, game.get_current_player());
			memset(buf, 0, 1024);
			snprintf(buf, 1024, format, m.c_str(), m2.c_str(), eval);
			ss << buf << endl;
		}
		else
		{
			memset(buf, 0, 1024);
			snprintf(buf, 1024, point, m.c_str(), m2.c_str());
			ss << buf << endl << generate_puml_graph(game, b2, selected, depth - 1) << endl;
			if (b.parent() == nullptr && ++max_count >= 3)
			{
				max_count = 0;
				ss << "@enduml" << endl << initials << endl;
			}
		}

	}

	return ss.str();
}

void generate_graph(ply& game, board& b, board& result, int depth)
{
	cout << "Graph file:";
	static int count = 1;
	string content = generate_puml_graph(game, b, result, depth);
	char buf[32] = { 00 };
#if !defined(__GNUC__)
	snprintf(buf, 32, "graph/%d.txt", count++);
#else
	snprintf(buf, 32, "graph/%d.txt");
#endif
	write_to_file(content, buf);
	cout << buf << endl;
}


void comp_vs_comp_game() {
#if 1
	int choice_1P_X = eval_choice("player 1");
	bool x_ab_search = search_choice("player 1");
	int choice_2P_O = eval_choice("player 2");
	bool o_ab_search = search_choice("player 2");
	int depth = depth_choice();
	
	bool gen_graph = false; 
	if (depth > 2)
	{
		cout << "Graph can't be generated for depth > 2" << endl;
	}
	else
	{
		char gg; cout << "Generate Graph files?(Y/N):"; cin >> gg;
		gen_graph =  gg == 'Y' || gg == 'y';
	}
#else
	int choice_1P_X = 4;
	bool x_ab_search = true;
	int choice_2P_O = 4;
	bool o_ab_search = true;
	int depth = 2;
#endif
	cout << "==========================" << endl;
	char again = 'Y';
	ply game = ply(choice_1P_X, choice_2P_O, depth, x_ab_search, o_ab_search);
	//Kushal
	board* node_after_root = nullptr;
	while (!game.get_borad().is_game_over())
	{
		auto root = game.generate_ply_depth(depth);
		auto sr = game.check_best_move(root.get());
		board* node_after_root = sr.node;
		while (node_after_root->parent() != root.get())
		{
			node_after_root = node_after_root->parent();
		}
		game.make_move(node_after_root->get_last_position());
		game.get_borad().print_board();
		cout << "Score >> " << sr.value << endl;
		//For generating graphs
		if(gen_graph)
			generate_graph(game, *root, *sr.node, depth);
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
