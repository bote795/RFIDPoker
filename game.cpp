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
	int stack;
	int bet_for_round;
	int in_hand;
	int in_game;
};

struct Table
{
	Player p[10];
	int total_players;
	int pot;
	int dealer_button;
};

Table t;

bool check_game_over()
{
	// count is number of players with a non zero stack size
	int count = 0;
	
	for(int i = 0; i < t.total_players; ++i)
	{
		if(t.p[i].stack != 0)
			++count;
	}

	return(if count == 1);
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
	
	run_Game();
}

void runGame()
{
	bool done = check_game_over()
	while(!done)
	{
		start_hand();
		done = check_game_over();
		
		// move dealer button over 1 spot
		t.dealer_button = (t.dealer_button+1) % t.total_players;
		while(t.p[(t.dealer_button+1) % t.total_players].in_game == 0)
		{
			t.dealer_button = (t.dealer_button+1) % t.total_players;
		}
	}
}

// return the index of the next player that is still in game from the base index
int next_player_game(int base_index)
{
	int next_index = base_index;
	while(t.p[base_index].in_game == 0)
	{
		base_index = (base_index+1) % t.total_players];
		next_index = base_index;
	}
	return next_index;
}

// return the index of the next player that is still in hand from the base index
int next_player_hand(int base_index)
{
	int next_index = base_index;
	while(t.p[base_index].in_hand == 0)
	{
		base_index = (base_index+1) % t.total_players];
		next_index = base_index;
	}
	return next_index;
}


void start_hand()
{
	
	// deduct blinds--------------------------------------------------
	
	// ************need to check to see if these players have enough for blinds*************
	

	
	int small_index = next_player_game(t.dealer_button);
	t.p[t.small_index].stack -= SMALL_BLIND;
	t.p[t.small_index].bet_for_round = SMALL_BLIND;
	
	int big_index = next_player_game(small_index);
	t.p[t.big_index].stack -= BIG_BLIND;
	t.p[t.big_index].bet_for_round = BIG_BLIND;
	
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
	
	// go around the table once
	for(int i = 0; i < total_players; ++i)
	{
		// find next player to bet who is in hand
		start = next_player_hand(start);
		
		// prompt player at index with amount to call
		// display the current players stack, amount to call, and current bet as they input it
		// return int of what value they put into numpad before pressing enter
		int bet = get_bet_amount(start, amount_to_call);
		
		if(bet == amount_to_call)
		{
			// the called the current max bet
		}
		else if(bet > amount_to_call)
		{
			// they raised the current max bet
		}
		else
		{
			// they folded
		}
	}
	
	// if everyone's bet in this round is the same, then we are done, else need to still loop thru the players still in game
	
}


int main()
{
	initializeGame();
	return 0;
}