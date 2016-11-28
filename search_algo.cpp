#include "stdafx.h"
#include "ply.h"

bool cutoff_reached(const board* node, int cutoff_depth)
{
	//Terminal Node: Run utility function or in this case return its value
	return node->is_game_over() || node->depth() == cutoff_depth;
}

struct search_result ply::alpha_beta_search(board* root_state, int cutoff_depth, Player firstPlayer, int alpha, int beta)
{
	LOG("Starting Alpha-Beta Search with Alpha:" << alpha << ", Beta:" << beta);
	return (firstPlayer == Player::X) ? max_value_search(root_state, firstPlayer, cutoff_depth, alpha, beta) : min_value_search(root_state, firstPlayer, cutoff_depth, alpha, beta);
}

struct search_result ply::max_value_search(board* state, Player player,int cutoff_depth, int alpha, int beta)
{
	LOG_D(state->depth(), state->tag() << ", MAX-VALUE function, Alpha:" << alpha << ", Beta:" << beta);
	struct search_result ret;
	if (cutoff_reached(state, cutoff_depth)) //Cuttoff Test, could be terminal or depth-limited
	{
		ret.node = state;
		ret.position = state->get_last_position();
		//ret.value = state->evaluate(player);
		ret.value = evaluate(*state, player);
		LOG_D(state->depth(), "Node:" << state->tag() << ", Eval:" << ret.value);
		return ret;
	}

	int value = NEG_INF;
	ret.value = value;
	for (board& n : state->successors())
	{
		increase_node_count();
		LOG_D(state->depth(), state->tag() << ", Succ:" << n.tag() << "->MIN-VALUE");
		struct search_result ret2 = min_value_search(&n, player, cutoff_depth, alpha, beta);
		if (value < ret2.value) //MAX(v, MIN-VALUE(RESULT(s,a),alpha,beta))
		{
			LOG_D(state->depth(), state->tag() << ",CVal: "<< value << ",Min res: " << ret2.value);
			value = ret2.value;
			ret = ret2;
		}
		if (value >= beta)
		{
			//LOG_D(state->depth(), "Returning Node: "<< state->tag() << ", as Value("<< value <<") >= Beta" );
			return ret;
		}
		alpha = std::max(alpha, value);
		LOG_D(state->depth(), state->tag() << ", Alpha: " << alpha);
	}
	return ret;
}


struct search_result ply::min_value_search(board* state, Player player, int cutoff_depth, int alpha, int beta)
{
	LOG_D(state->depth(),  state->tag() << ", MIN-VALUE function, Alpha:" << alpha << ", Beta:" << beta);
	struct search_result ret;
	if (cutoff_reached(state, cutoff_depth)) //Cuttoff Test, could be terminal or depth-limited
	{
		ret.node = state;
		ret.position = state->get_last_position();
		//ret.value = state->evaluate(player);
		ret.value = evaluate(*state, player);
		LOG_D(state->depth(), state->tag() << "Eval:" << ret.value);
		return ret;
	}

	int value = POS_INF;
	ret.value = value;
	for (board& n : state->successors())
	{
		increase_node_count();
		LOG_D(state->depth(), state->tag() << ", Succ:" << n.tag() << ", calling: MAX-VALUE");
		struct search_result ret2 = max_value_search(&n, player, cutoff_depth , alpha, beta);
		if (value > ret2.value) //MIN(v, MAX-VALUE(RESULT(s,a), alpha,beta))
		{
			LOG_D(state->depth(), state->tag() << ",CVal: "<< value << ",Max res: " << ret2.value);
			value = ret2.value;
			ret = ret2;
		}
		if (value <= alpha)
		{
			//LOG_D(state->depth(), "Returning Node: " << state->tag() << ", as Value(" << value << ") <= Alpha");
			return ret;
		}
		beta = std::min(beta, value);
		LOG_D(state->depth(), state->tag() << ", Beta: " << beta);
	}
	return ret;
}

struct search_result ply::minimax_ab(board* pos, int depth, Player player, int use_thres, int pass_thres)
{
	LOG_D(pos->depth(), "MinimaxAB()->Node:" << pos->tag() << ", depth:" << depth << ", player:" << PLAYER_NAMES[player == cell_value::X?0:1] << ", UT:" << use_thres << ", PT:" << pass_thres);
	struct search_result rs;
	if (cutoff_reached(pos, depth))
	{
		//This is absolutely necessary as MiniMaxAB won't work if the MIN nodes are not flipped negative
		rs.value = (is_x(pos->player())?(-1):1)*evaluate(*pos, player);
		//rs.value = evaluate(*pos, player);
		LOG_D(depth,  "Return-Terminal Node:" << pos->tag() << ",value:" << rs.value);
		rs.node = pos;
		rs.position = pos->get_last_position();
		return rs;
	}/*
	else
	{
		LOG_D(depth, "Deep-Enough false, node: " << pos->tag());
	}*/

	vector<board>& succ_list = pos->successors(); //MOVE_GEN
	LOG_D(pos->depth(),  "Node:" << pos->tag() << ", Succ Count:" << succ_list.size());
	//Currently we don't need to check for if the succ is empty or not coz we are defining deep_enough using zero succ
	//But if we are to implement imperfect real-world deep_enough the we have to add extra checking

	//So Succ isn't empty
	int new_value;
	for (board& succ : succ_list)
	{
		increase_node_count();
		Player swapped_player  = SWAP_PLAYER(player);
		struct search_result rs2 = minimax_ab(&succ, depth, swapped_player, -pass_thres, -use_thres);
		new_value = -rs2.value;
		LOG_D(pos->depth(),  pos->tag() << ", Succ:" << succ.tag() << ", Value:" << rs2.value << ", New-Value:"<<new_value);
		if (new_value > pass_thres) // found a succ which is better
		{
			pass_thres = new_value;
			rs.node = rs2.node;
			//rs.position = rs2.node->get_last_position();
			LOG_D(pos->depth(), pos->tag() <<", Succ:" << succ.tag() << ", NV>PT, PT:" << pass_thres << /*", BP:" << rs2.node->tag()*/);
		}

		//if (pass_thres < use_thres) //stop examining this branch
		//{
		//	//std::swap(pass_thres, use_thres);
		//	//??? We are supposed to Swap here but the example didn't swapped
		//	LOG_D(depth, "Node:"<<pos->tag() << ", Succ:" << succ->tag() << ",PT-UT, PT:" << pass_thres << ",UT:" << use_thres);
		//}
		if(pass_thres >= use_thres)
		{
			rs.value = pass_thres;
			LOG_D(pos->depth(), pos->tag() << ", Succ:" << succ.tag() << ", PT>=UT, Value:" << pass_thres );
			//rs.best_path_node is already set

		}
	}

	rs.value = pass_thres;
	//rs.best_path_node is already set
	LOG_D(pos->depth(), "Ret:" << pos->tag() << ", BP:" << rs.node->tag()<< ", Value:" << rs.value);
	return rs;
}