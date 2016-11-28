#pragma once

#include "stdafx.h"
#include "board.h"
#include "ply.h"

#define POS_INF INT_MAX
#define NEG_INF INT_MIN

struct search_result max_value_search(board* state, Player player, int depth, int alpha, int beta);
struct search_result min_value_search(board* state, Player player,int depth, int alpha, int beta);

bool cutoff_reached(board* node, int cutoff_depth)
{
	//Terminal Node: Run utility function or in this case return its value
	return node->is_terminal2() || node->depth() == cutoff_depth;
}

struct search_result alpha_beta_search(board* root_state, int cutoff_depth, Player firstPlayer, int alpha, int beta)
{
	LOG("Starting Alpha-Beta Search with Alpha:" << alpha << ", Beta:" << beta);
	return (firstPlayer == Player::X) ? max_value_search(root_state, firstPlayer, cutoff_depth, alpha, beta) : min_value_search(root_state, firstPlayer, cutoff_depth, alpha, beta);
}

struct search_result max_value_search(board* state, Player player,int cutoff_depth, int alpha, int beta)
{
	LOG_D(state->depth(), state->tag() << ", MAX-VALUE function, Alpha:" << alpha << ", Beta:" << beta);
	struct search_result ret;
	if (cutoff_reached(state, cutoff_depth)) //Cuttoff Test, could be terminal or depth-limited
	{
		ret.node = state;
		ret.position = state->get_last_position();
		ret.value = state->evaluate(player);
		LOG_D(state->depth(), "Node:" << state->tag() << ", Eval:" << ret.value);
		return ret;
	}

	int value = NEG_INF;
	ret.value = value;
	for (board& n : state->successors())
	{
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


struct search_result min_value_search(board* state, Player player, int cutoff_depth, int alpha, int beta)
{
	LOG_D(state->depth(),  state->tag() << ", MIN-VALUE function, Alpha:" << alpha << ", Beta:" << beta);
	struct search_result ret;
	if (cutoff_reached(state, cutoff_depth)) //Cuttoff Test, could be terminal or depth-limited
	{
		ret.node = state;
		ret.position = state->get_last_position();
		ret.value = state->evaluate(player);
		LOG_D(state->depth(), state->tag() << "Eval:" << ret.value);
		return ret;
	}

	int value = POS_INF;
	ret.value = value;
	for (board& n : state->successors())
	{
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