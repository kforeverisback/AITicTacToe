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

void comp_vs_comp_game() {
#if 1
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
		cout << "Score >>" << sr.value << endl;
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
