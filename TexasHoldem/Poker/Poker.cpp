// Poker.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


// run game
	// how many players
	// initialize player stacks
	// assign player 0 to small blind player 1 to big blind
	// pre-flop
		// amount to call is big blind
		// player 1 starts betting
		// while
			// prompt player 2 to bet, show bet amount and their stack
			// if they fold, remove them from current hand,
			// else if they bet, do they call or raise
				// if raise set current max bet to stay in to their raise
				// keep track of each players bet (array of bets, index corresponds to player)
		// end loop
		// if there is a raise, need to continue betting round till end
		
#define INITIAL_STACK 	500
#define SMALL_BLIND 	10
#define BIG_BLIND 		20

struct Player
{
	int stack;				// current amount of chips a player has
	int bet_for_round;		// total bet a player has for this betting round
	int in_hand;			// is the player still in the current hand
	int in_game;			// is the player still in the game
};

struct Table
{
	Player p[10];			// array of all players
	int total_players;		// number of total players
	int pot;				// current amount of chips in the pot
	int dealer_button;		// player p index of which player has the dealer chip
};

Table t;

// start is the player index in table, amount_to_call is the current max bet
// returns the amount the player bet, 0 if check/fold
int get_bet_amount(int start, int amount_to_call)
{
	printf("--------Table---------\n");
	printf("Pot size: %d\n", t.pot);
	printf("--------Player %d-------\n", start+1);
	printf("Stack: %d\n", t.p[start].stack);
	printf("Current Max Bet: %d\n", amount_to_call);
	printf("Bet for round: %d\n", t.p[start].bet_for_round);
	printf("Amount to call: %d\n", amount_to_call-t.p[start].bet_for_round);
	printf("*Enter bet: ");
	int bet = 0;
	std::cin >> bet;
	printf("\n");
	return bet;
}

int get_total_players()
{
	printf("*Input the total number of players (n<10): ");
	int total = 0;
	std::cin >> total;
	printf("\n");
	return total;
}

bool check_game_over()
{
	// count is number of players with a non zero stack size
	int count = 0;
	
	for(int i = 0; i < t.total_players; ++i)
	{
		if(t.p[i].stack != 0)
			++count;
	}

	return (count == 1);
}

// return the index of the next player that is still in game from the base index
int next_player_game(int base_index)
{
	int next_index = (base_index+1) % t.total_players;
	while(t.p[next_index].in_game == 0)
	{
		base_index = (base_index+1) % t.total_players;
		next_index = base_index;
	}
	return next_index;
}

// return the index of the next player that is still in hand from the base index
int next_player_hand(int base_index)
{
	int next_index = (base_index+1) % t.total_players;
	while(t.p[next_index].in_hand == 0)
	{
		base_index = (base_index+1) % t.total_players;
		next_index = base_index;
	}
	return next_index;
}


void betting_round(int start, int max_bet)
{
	// the player index with the max bet
	int max_index = start;

	// finds out the number of players still in the hand
	int total_players_hand = 0;
	for(int i = 0; i < t.total_players; ++i)
	{
		if(t.p[i].in_hand)
			++total_players_hand;
	}

	// go around the table once
	for(int i = 0; i < total_players_hand; ++i)
	{
		
		// find next player to bet who is in hand
		start = next_player_hand(start);
		
		// prompt player at index with amount to call
		// display the current players stack, amount to call, and current bet as they input it
		// return int of what value they put into numpad before pressing enter
		int bet = get_bet_amount(start, max_bet);
		t.p[start].bet_for_round += bet;
		t.p[start].stack -= bet;


		if(t.p[start].bet_for_round == max_bet)
		{
			// the called the current max bet
			t.pot += bet;
		}
		else if(t.p[start].bet_for_round > max_bet)
		{
			// they raised the current max bet
			max_bet = t.p[start].bet_for_round;
			t.pot += bet;
			max_index = start;
		}
		else
		{
			// they folded
			t.p[start].in_hand = 0;
			t.p[start].stack += bet;
			t.p[start].bet_for_round -= bet;
		}
	}
	
	bool done = true;
	int bet = -1;
	for(int i = 0; i < t.total_players; ++i)
	{
		if(t.p[i].in_hand == 1)
		{
			if(bet == -1)
				bet = t.p[i].bet_for_round;
			else
			{
				if(bet != t.p[i].bet_for_round)
				{
					done = false;
					break;
				}
			}
		}
	}

	if(!done)
		betting_round(max_index, max_bet);

	// if everyone's bet in this round is the same, then we are done, else need to still loop thru the players still in game
}

void start_hand()
{
	
	printf("Hand is starting!\n");

	// deduct blinds--------------------------------------------------
	
	// ************need to check to see if these players have enough for blinds*************
	
	// set everyone who is in game to be in hand as well
	for(int i = 0; i < t.total_players; ++i)
	{
		t.p[i].bet_for_round = 0;
	}

	printf("Player %d has the dealer chip.\n", t.dealer_button+1);
	
	int small_index = next_player_game(t.dealer_button);
	t.p[small_index].stack -= SMALL_BLIND;
	t.p[small_index].bet_for_round = SMALL_BLIND;
	
	printf("Deducting small blind of %d from player %d.\n", SMALL_BLIND, small_index+1);

	int big_index = next_player_game(small_index);
	t.p[big_index].stack -= BIG_BLIND;
	t.p[big_index].bet_for_round = BIG_BLIND;

	printf("Deducting big blind of %d from player %d.\n", BIG_BLIND, big_index+1);
	
	t.pot = BIG_BLIND + SMALL_BLIND;
	//---------------------------------------------------------------
	
	// set everyone who is in game to be in hand as well
	for(int i = 0; i < t.total_players; ++i)
	{
		if(t.p[i].in_game == 1)
		{
			t.p[i].in_hand = 1;
		}
	}

	// cards are then dealt, blinds set, now betting round
	int amount_to_call = BIG_BLIND;
	
	// start is the current index of the player who needs to bet
	int start = big_index;
	
	betting_round(start, amount_to_call);
	
}

void flop()
{
	printf("The flop is starting!\n");

}

void turn()
{
	printf("The turn is starting!\n");

}

void river()
{
	printf("The river is starting!\n");

}

void run_Game()
{
	bool done = check_game_over();
	while(!done)
	{

		start_hand();

		flop();

		turn();

		river();

		done = check_game_over();
		
		// move dealer button over 1 spot
		t.dealer_button = (t.dealer_button+1) % t.total_players;
		while(t.p[(t.dealer_button+1) % t.total_players].in_game == 0)
		{
			t.dealer_button = (t.dealer_button+1) % t.total_players;
		}
	}
}

void initializeGame()
{
	// call to the interface to prompt and read numpad input, want an int
	t.total_players = get_total_players();
	t.dealer_button = 0;
	
	for(int i = 0; i < t.total_players; ++i)
	{
		Player a;
		a.stack = INITIAL_STACK;
		a.in_game = 1;
		a.in_hand = 0;
		t.p[i] = a;
	}
	printf("Time to start the game!\n");
	run_Game();
}

int main()
{
	printf("Welcome to Texas Holdem!\n");
	initializeGame();
	return 0;
}

