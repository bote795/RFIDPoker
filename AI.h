struct Card
{
	int number;
	char suit;
};

struct AI
{
	Card c1;
	Card c2;
};

AI a;

void AI_bet()
{
// pre-flop strategy
	// based on two first cards and the bets from others
	// if Ace suited with .... this value is assigned to it
	// if king suit do this ... and so on for all combos
	// bet is based on value of cards, amount to call? /raise, current pot, how much i bet already
	
// post-flop
	// current value of hand (is it a pair, a 3 of kind, flush, straight, full house, four of kind)
	// probability of getting one of these hand types, and the value of the hand type
	// bet is based on value of cards, amount to call? /raise, current pot, how much I bet already
	// along with current value of hand with table hands, how much each person put into the hand, probability of getting/not getting a better hand, probability of opponents of other hands
	
}

void preflop_value()
{
	char s1 = a.c1.suit;
	char s2 = a.c2.suit;
	int n1 = a.c1.number;
	int n2 = a.c2.number;
	
	int value = 0;
	
	if(n1 == n2)													// pocket pair
	{
		if(n1 == 14 || n1 == 13)									// A-A, K-K
		{
			value = 100;
		}
		else if(n1 == 12 || n1 == 11)								// Q-Q, J-J
		{
			value = 95;
		}
		else if(n1 == 10)											// 10-10
		{
			value = 90;
		}
		else if(n1 > 6)											// 7:9-7:9 pair
		{
			value = 80;
		}
		else														// 2:6-2:6 pair
		{
			value = 60;
		}
	}
	else if(n1 == 14 || n2 == 14)									// exactly 1 Ace
	{
		if(s1 == s2)												// suited
		{
			if(n1 == 13 || n2 == 13)								// A-K
			{
				value = 90;
			}
			else if(n1 == 12 || n2 == 12 || n1 == 11 || n2 == 11)	// A-Q, A-J
			{
				value = 80;
			}
			else if(n1 == 10 || n2 == 10)							// A-10
			{
				value = 70;
			}
			else if(n1 > 6 || n2 > 6)								// A-7:9
			{
				value = 65;
			}
			else													// A-2:6
			{
				value = 55;
			}
		}
		else														// unsuited
		{
			if(n1 == 13 || n2 == 13)								// A-K
			{
				value = 85;
			}
			else if(n1 == 12 || n2 == 12 || n1 == 11 || n2 == 11)	// A-Q, A-J
			{
				value = 75;
			}
			else if(n1 == 10 || n2 == 10)							// A-10
			{
				value = 65;
			}
			else if(n1 > 6 || n2 > 6)								// A-7:9
			{
				value = 60;
			}
			else													// A-2:6
			{
				value = 50;
			}
		}
	}
	else if(n1 >= 11 && n2 >= 11)
	{
		if(s1 == s2)												// suited
		{
			if((n1 == 13 || n2 == 13) && (n1 >= 11 || n2 >= 11))	// K-Q, K-J
			{
				value = 80;
			}
			else if((n1 == 12 || n2 == 12) && (n1 == 11 || n2 == 11))	// Q-J
			{
				value = 75;
			}
			else													// K:J-2:10
			{
				if((n1 >= 11 || n2 >= 11) && (n1 == 10 || n2 == 10))
					value = 55;
				else
					value = 45;
			}
		}
		else														// unsuited
		{
			if((n1 == 13 || n2 == 13) && (n1 >= 11 || n2 >= 11))	// K-Q, K-J
			{
				value = 75;
			}
			else if((n1 == 12 || n2 == 12) && (n1 == 11 || n2 == 11))	// Q-J
			{
				value = 70;
			}
			else
			{
				if((n1 >= 11 || n2 >= 11) && (n1 == 10 || n2 == 10))
					value = 50;
				else
					value = 40;
			}
		}
	}
	else if((n1 == 10 || n2 == 10) || (n1 == 9 || n2 == 9))			// one 10 or one 9
	{
		if(s1 == s2)												// suited
		{
			value = 30;
		}
		else														// unsuited
		{
			value = 25;
		}
	}
	else
	{
		value = 0
	}
	
	// check for sequential
	if((n1 == (n2+1)) || (n1 == (n2-1)))						// sequential
	{
		if(s1 == s2)												// suited
		{
			if((n1 == 9) || (n2 == 9) || (n1 == 8) || (n2 == 8))	// 7-8, 8-9, 9-10
			{
				value += 20;
			}
			else if(!((n1 >= 12) || (n2 >= 12)))						// not face card
			{
				value += 10;
			}
		}
		else
		{
			if((n1 == 9) || (n2 == 9) || (n1 == 8) || (n2 == 8))	// 7-8, 8-9, 9-10
			{
				value += 15;
			}
			else if(!((n1 >= 12) || (n2 >= 12)))						// not face card
			{
				value += 5;
			}
		}
	}
	
}


void postflop_value()
{
	// n1 & n2 == amything on table, ++ count for number
	// 
}
