// Poker.cpp : Defines the entry point for the console application.
//

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

#include "stdafx.h"
#include "AI.h"

#define INITIAL_STACK   500
#define SMALL_BLIND     10
#define BIG_BLIND       20

struct Player
{
	int stack;        // current amount of chips a player has
	int bet_for_round;    // total bet a player has for this betting round
	int in_hand;      // is the player still in the current hand
	int in_game;      // is the player still in the game
};

struct Table
{
	Player p[10];     // array of all players
	int total_players;    // number of total players
	int pot;        // current amount of chips in the pot
	int dealer_button;    // player p index of which player has the dealer chip
	int AI_index;     // index of AI player
	int is_AI;
	int split[10];
	int is_all_in;			// if someone is ALL IN
};

Table t;


/********************************************************************************

// This function steps through both newTag and one of the known
// tags. If there is a mismatch anywhere in the tag, it will return 0,
// but if every character in the tag is the same, it returns 1
int checkTag(char nTag[], char oTag[]) {
	for (int i = 0; i < idLen; i++) {
		if (nTag[i] != oTag[i]) {
			return 0;
		}
	}
	return 1;
}

void clearBuffer()
{
	for (int i = 0; i < 20; ++i)
		buffer[i] = ' ';
}

//text = string to be printed to lcd
//row = row to be printed out to
void  writeToLCD(String text, int row)
{
	lcd.setCursor(0, row);
	lcd.print("                    ");
	lcd.setCursor(0, row);
	Serial.println(text);
	lcd.print(text);
	clearBuffer();
}
void clearScreen()
{
	lcd.clear();
}

//function used to read from keyPad
//# = used for enter
//* =  used for backspace
long readFromKeypad(int printLine)
{
	int returnVal = -1;
	char input[6];
	while (returnVal == -1) {
		int maxNums = 6;
		long i = 0;
		char key = keypad.getKey();
		if (key)
		{
			while (key != '#')
			{
				if (key)
				{
					if (key == '*')
					{
						if (i > 0)    //check for going under index
						{
							i--;
						}
						input[i] = '\0';
					}
					else
					{
						if (i < maxNums - 1)  //check for going over
						{
							input[i] = key;
							i++;

						}
					}
					lcd.setCursor(0, printLine);
					lcd.print("                    ");
					lcd.setCursor(0, printLine);
					lcd.print(atol(input));
					Serial.println(atol(input));
					//lcd.print(atol(input));

				}
				key = keypad.getKey();
			}
			returnVal = 0;
		}
		else
			returnVal = -1;
	}


	return atol(input);
}

Card findCards()
{
	// Counter for the newTag array
	int i = 0;
	// Variable to hold each byte read from the serial buffer
	int readByte;
	// Flag so we know when a tag is over
	boolean tag = false;
	// This makes sure the whole tag is in the serial buffer before
	// reading, the Arduino can read faster than the ID module can deliver
	// Serial.println(rSerial.available()); 
	Card temp;
	while (!tag){
		if (rSerial.available() == tagLen) {
			tag = true;
			Serial.println("avilable tag");
		}
	}
	while (rSerial.available()) {
		// Take each byte out of the serial buffer, one at a time
		readByte = rSerial.read();
		Serial.println("reading tag");
		// This will skip the first byte (2, STX, start of text) and the last three,
		//ASCII 13, CR/carriage return, ASCII 10, LF/linefeed, and ASCII 3, ETX/end of
		//text, leaving only the unique part of the tag string. It puts the byte into
		//the first space in the array, then steps ahead one spot
		if (readByte != 2 && readByte != 13 && readByte != 10 && readByte != 3) {
			newTag[i] = readByte;
			i++;
		}

		// If we see ASCII 3, ETX, the tag is over
		if (readByte == 3) {
			break;
		}

	}


	// don't do anything if the newTag array is full of zeroes
	if (strlen(newTag) == 0) {
		temp.number = -1;
		return temp;
	}

	else {
		int total = 0;
		int ct = 0;
		Serial.println("looking for tag");
		for (ct = 0; ct < kTags; ct++){
			total += checkTag(newTag, knownTags[ct]);
			if (total > 0)
				break;
		}

		// If newTag matched any of the tags
		// we checked against, total will be 1
		if (total > 0) {
			char num[3];
			num[0] = keyTags[ct][1];
			num[1] = keyTags[ct][2];
			// Put the action of your choice here

			// set the cursor to column 0, line 1
			// (note: line 1 is the second row, since counting begins with 0):

			temp.number = atoi(num);
			temp.suit = keyTags[ct][0];
			writeToLCD("Card read", 1);
			delay(1000);
			Serial.println("Success");

		}
		else {
			// This prints out unknown cards so you can add them to your knownTags as needed
			Serial.print("Unknown tag");
			Serial.print(newTag);
			Serial.println();
			clearScreen();
			writeToLCD("Retry scanning card", 0);
			delay(1000);
			return findCards();
		}
	}

	// Once newTag has been checked, fill it with zeroes
	// to get ready for the next tag read
	for (int c = 0; c < idLen; c++) {
		newTag[c] = 0;
	}
	Serial.println("We are out");
	return temp;

}

*************************************************************************************/

// start is the player index in table, amount_to_call is the current max bet
// returns the amount the player bet, 0 if check/fold
int get_bet_amount(int start, int amount_to_call)
{

/*
	clearScreen();
	sprintf(buffer, "Player %d enter bet", start + 1);
	writeToLCD(buffer, 0);
	sprintf(buffer, "Bet for round: %d", t.p[start].bet_for_round);
	writeToLCD(buffer, 1);
	sprintf(buffer, "Amount to call: %d", amount_to_call - t.p[start].bet_for_round);
	writeToLCD(buffer, 2);

	int bet = readFromKeypad(3);

	return bet;
*/

	printf("******************\nPlayer %d enter bet\nBet for round: %d\nAmount to call: %d\n", start + 1, t.p[start].bet_for_round, amount_to_call - t.p[start].bet_for_round);
	int bet;
	std::cin >> bet;
	return bet;
}

int get_AI_bet(int start, int amount_to_call)
{

/*

	clearScreen();
	writeToLCD("AI is calulating bet...", 0);
	sprintf(buffer, "Bet for round: %d", t.p[start].bet_for_round);
	writeToLCD(buffer, 1);
	sprintf(buffer, "Amount to call: %d", amount_to_call - t.p[start].bet_for_round);
	writeToLCD(buffer, 2);
	delay(2000);

	int bet = AI_bet(amount_to_call - t.p[start].bet_for_round, t.p[start].bet_for_round, t.pot, t.p[start].stack);
	
	return bet;

*/

	printf("AI calculating bet...\n");
	int bet = AI_bet(amount_to_call - t.p[start].bet_for_round, t.p[start].bet_for_round, t.pot, t.p[start].stack);
	return bet;
}

int get_total_players()
{

/*
	clearScreen();
	writeToLCD("*Input total number", 0);
	writeToLCD("of players (n < 10):", 1);
	int total = readFromKeypad(2);
	clearScreen();
	writeToLCD("*Is there an AI? ", 0);
	writeToLCD("*1 = Y , 2 = N ", 1);
	int response = readFromKeypad(2);
	if (response == 1){
		t.is_AI = 1;
		writeToLCD("AI is playing", 3);
	}
	else {
		t.is_AI = 0;
		writeToLCD("AI is not playing", 3);
	}
	delay(2000);

	return total;
*/

	printf("Enter total players: ");
	int total = 0;
	std::cin >> total;
	printf("Is there AI (1 = yes)? ");
	int res = 0;
	std::cin >> res;
	if (res == 1)
		t.is_AI = 1;
	else
		t.is_AI = 0;
	return total;

}

bool check_game_over()
{
	// count is number of players with a non zero stack size
	int count = 0;

	for (int i = 0; i < t.total_players; ++i)
	{
		if (t.p[i].stack != 0)
			++count;
	}

	return (count == 1);
}

// return the index of the next player that is still in game from the base index
int next_player_game(int base_index)
{
	int next_index = (base_index + 1) % t.total_players;
	while (t.p[next_index].in_game == 0)
	{
		base_index = (base_index + 1) % t.total_players;
		next_index = base_index;
	}
	return next_index;
}

// return the index of the next player that is still in hand from the base index
int next_player_hand(int base_index)
{
	int next_index = (base_index + 1) % t.total_players;
	while (t.p[next_index].in_hand == 0)
	{
		base_index = (base_index + 1) % t.total_players;
		next_index = base_index;
	}
	return next_index;
}

// input the AI cards via the RFID scanner
void input_AI_cards()
{

/*
	clearScreen();
	writeToLCD("Scan AI card 1", 0);
	a.c1 = findCards();
	clearScreen();
	writeToLCD("Scan AI card 2", 0);
	a.c2 = findCards();
	a.initial_stack = t.p[t.AI_index].stack;
*/
	int v1 = 0;
	int v2 = 0;
	char s1 = ' ';
	char s2 = ' ';
	printf("Input AI card 1 value: ");
	std::cin >> v1;
	printf("Input AI card 1 suit: ");
	std::cin >> s1;
	printf("Input AI card 2 value: ");
	std::cin >> v2;
	printf("Input AI card 2 suit: ");
	std::cin >> s2;

	Card c1;
	Card c2;
	c1.number = v1;
	c1.suit = s1;
	c2.number = v2;
	c2.suit = s2;

	a.c1 = c1;
	a.c2 = c2;
}

// input the flop cards via the RFID scanner
void input_flop_cards()
{

/*
	clearScreen();
	writeToLCD("Scan flop card 1", 0);
	a.table_cards[0] = findCards();
	clearScreen();
	writeToLCD("Scan flop card 2", 0);
	a.table_cards[1] = findCards();
	clearScreen();
	writeToLCD("Scan flop card 3", 0);
	a.table_cards[2] = findCards();
	a.table_size = 3;
*/
	int v1 = 0;
	int v2 = 0;
	int v3 = 0;
	char s1 = ' ';
	char s2 = ' ';
	char s3 = ' ';
	printf("Input flop card 1 value: ");
	std::cin >> v1;
	printf("Input flop card 1 suit: ");
	std::cin >> s1;
	printf("Input flop card 2 value: ");
	std::cin >> v2;
	printf("Input flop card 2 suit: ");
	std::cin >> s2;
	printf("Input flop card 3 value: ");
	std::cin >> v3;
	printf("Input flop card 3 suit: ");
	std::cin >> s3;

	Card c1;
	Card c2;
	Card c3;
	c1.number = v1;
	c1.suit = s1;
	c2.number = v2;
	c2.suit = s2;
	c3.number = v3;
	c3.suit = s3;

	a.table_cards[0] = c1;
	a.table_cards[1] = c2;
	a.table_cards[2] = c3;
	a.table_size = 3;
}

// input the turn cards via the RFID scanner
void input_turn_cards()
{

/*
	clearScreen();
	writeToLCD("Scan turn card", 0);
	a.table_cards[3] = findCards();
	a.table_size = 4;
*/
	int v1 = 0;
	char s1 = ' ';
	printf("Input turn card value: ");
	std::cin >> v1;
	printf("Input turn card suit: ");
	std::cin >> s1;

	Card c1;

	c1.number = v1;
	c1.suit = s1;

	a.table_cards[3] = c1;

	a.table_size = 4;
}

// input the river cards via the RFID scanner
void input_river_cards()
{
	
/*
	clearScreen();
	writeToLCD("Scan river card", 0);
	a.table_cards[4] = findCards();
	a.table_size = 5;
*/
	int v1 = 0;
	char s1 = ' ';
	printf("Input river card value: ");
	std::cin >> v1;
	printf("Input river card suit: ");
	std::cin >> s1;

	Card c1;

	c1.number = v1;
	c1.suit = s1;

	a.table_cards[4] = c1;

	a.table_size = 5;
}

// prints to players to see who winner is, gets input, return value is player number (NOT INDEX)
int get_winner()
{

	bool valid = false;
	int winner = -1;
	while (!valid)
	{

/*
		clearScreen();
		writeToLCD("Enter hand winner: ", 0);

		if (t.is_AI == 1 && t.p[t.AI_index].in_hand == 1)
		{

			sprintf(buffer, "AI is player %d", t.AI_index + 1);
			writeToLCD(buffer, 1);
			winner = readFromKeypad(2);

		}
		else
			winner = readFromKeypad(1);
*/
		
		printf("Input hand winner (AI is player %d): ", t.AI_index+1);
		std::cin >> winner;
		
		for (int i = 0; i < t.total_players; ++i)
		{
			if (t.p[i].in_hand == 1 && winner - 1 == i)
				valid = true;
		}
	}
	return winner - 1;
}

// set the winner of the hand
void set_winner(int who_won)
{
	t.p[who_won].stack += t.pot;
	t.pot = 0;

/*
	clearScreen();
	if(who_won+1 == t.AI_index)
		sprintf(buffer, "AI won");
	else
		sprintf(buffer, "Player %d won", who_won + 1);
	writeToLCD(buffer, 0);

	sprintf(buffer, "Winner's stack: %d", t.p[who_won].stack);
	writeToLCD(buffer, 1);
	delay(3000);
*/

	printf("Player %d won the hand!\nWinner's Stack: %d\n", who_won + 1, t.p[who_won].stack);

	for (int i = 0; i < t.total_players; ++i)
	{
		if (t.p[i].stack == 0)
		{
			t.p[i].in_game = 0;
			t.p[i].in_hand = 0;
		}
	}
}

// input which player was the winner of the showdown, get to river and betting is over
void input_winner()
{
	// get which is winner from numpad

/*
	clearScreen();
	writeToLCD("SHOWDOWN", 0);
	delay(2000);
*/

	printf("SHOWDOWN!!!\n");

	int winner = get_winner();

	set_winner(winner);

}

int betting_round(int start, int max_bet)
{
	// index of player who won
	int who_won = -1;

	// check to see if someone went all in this hand
	// and if its start of beting round, no more betting
	if (t.is_all_in == 1 && max_bet == 0)
		return who_won;

	// finds out the number of players still in the hand
	int total_players_hand = 0;
	for (int i = 0; i < t.total_players; ++i)
	{
		if (t.p[i].in_hand == 1 && t.p[i].in_game == 1)
			++total_players_hand;
	}

	// go around the table once
	for (int i = 0; i < total_players_hand; ++i)
	{

		// find next player to bet who is in hand
		start = next_player_hand(start);

		if (t.p[start].bet_for_round == max_bet && max_bet != 0)
			continue;

		// prompt player at index with amount to call
		// display the current players stack, amount to call, and current bet as they input it
		// return int of what value they put into numpad before pressing enter
		int bet = 0;
		if (t.is_AI == 1 && t.AI_index == start)
			bet = get_AI_bet(start, max_bet);
		else
			bet = get_bet_amount(start, max_bet);

		// dont let some1 bet more than what they have...
		if (bet > t.p[start].stack)
			bet = t.p[start].stack;

		// the smallest stack of everyone who is still in the hand
		int smallest_stack = INT_MAX;
		for (int i = 0; i < t.total_players; ++i)
		{
			if (t.p[i].in_hand == 1)
			{
				if ((t.p[i].stack + t.p[i].bet_for_round) < smallest_stack)
					smallest_stack = t.p[i].stack + t.p[i].bet_for_round;
			}
		}

		if (bet > (smallest_stack - t.p[start].bet_for_round))
		{

/*
			clearScreen();
			if (start == t.AI_index)
				sprintf(buffer, "AI bet %d", bet);
			else
				sprintf(buffer, "Player %d bet %d", start + 1, bet);
			writeToLCD(buffer, 0);
			sprintf("But max bet is %d", smallest_stack);
			writeToLCD(buffer, 1);
			sprintf(buffer, "Therefore bet = %d", smallest_stack - t.p[start].bet_for_round);
			writeToLCD(buffer, 2);
*/

			bet = smallest_stack - t.p[start].bet_for_round;

			printf("Bet cuttoff is max of small stack + their bet for round: %d\n", smallest_stack);
			printf("Therefore bet = %d\n", bet);
		}
		else
		{

/*
			clearScreen();
			if (start == t.AI_index)
				sprintf(buffer, "AI bet %d", bet);
			else
				sprintf(buffer, "Player %d bet %d", start + 1, bet);
			writeToLCD(buffer, 0);
*/
			printf("Bet is %d\n", bet);
		}

		t.p[start].bet_for_round += bet;
		t.p[start].stack -= bet;

		// check the bet
		if (t.p[start].stack == 0)
		{
			t.pot += bet;
			t.is_all_in = 1;

			if (t.p[start].bet_for_round > max_bet)
				max_bet = t.p[start].bet_for_round;

/*
			writeToLCD("All in", 3);
*/

			printf("All in!\n");

		}
		else if (t.p[start].bet_for_round == max_bet)
		{
			// they called the current max bet
			t.pot += bet;

/*
			writeToLCD("Called", 3);
*/

			printf("Called\n");

		}
		else if (t.p[start].bet_for_round > max_bet)
		{

/*
			sprintf(buffer, "Raised by %d", t.p[start].bet_for_round - max_bet);
			writeToLCD(buffer, 3);
*/

			printf("Raised by %d\n", t.p[start].bet_for_round - max_bet);

			// they raised the current max bet
			max_bet = t.p[start].bet_for_round;
			t.pot += bet;

		}
		else
		{
			// they folded
/*
			writeToLCD("Folded", 3);
*/

			printf("Folded\n");

			t.p[start].in_hand = 0;
			t.p[start].stack += bet;
			t.p[start].bet_for_round -= bet;

			// check if hand is over if there is only 1 person left
			int players_left = 0;
			int player_index = -1;
			for (int i = 0; i < t.total_players; ++i)
			{
				if (t.p[i].in_hand == 1)
				{
					++players_left;
					player_index = i;
				}
			}

			// hand is over and player won
			if (players_left == 1)
				return player_index;

		}

/*
		if (start == t.AI_index)
			delay(1000);
		delay(2000);
*/

	}


	// check if betting round is done
	// occurs when all players in the hand have bet the same amount for the round
	// or if they have gone all in
	bool done = true;
	int bet = -1;
	for (int i = 0; i < t.total_players; ++i)
	{
		if (t.p[i].in_hand == 1)
		{
			if (bet == -1)
				bet = t.p[i].bet_for_round;
			if (t.p[i].bet_for_round != bet)
			{
				done = false;
				break;
			}
		}
	}

	if (!done)
		who_won = betting_round(start, max_bet);

	return who_won;

	// if everyone's bet in this round is the same, then we are done, else need to still loop thru the players still in game
}

int start_hand()
{

/*
	clearScreen();
	writeToLCD("Hand is starting", 0);
	delay(2000);
*/

	printf("Hand is starting!\n");

/*
	clearScreen();
	sprintf(buffer, "Player %d has", t.dealer_button + 1);
	writeToLCD(buffer, 0);
	writeToLCD("dealer chip", 1);
	delay(2000);
*/

	printf("Player %d has the dealer chip\n", t.dealer_button + 1);

	// set everyone who is in game to be in hand as well
	for (int i = 0; i < t.total_players; ++i)
	{
		if (t.p[i].in_game == 1)
			t.p[i].in_hand = 1;
		else
			t.p[i].in_hand = 0;
		t.p[i].bet_for_round = 0;
	}
	t.is_all_in = 0;

	int small_index = next_player_game(t.dealer_button);
	if (t.p[small_index].stack < SMALL_BLIND)
	{
		t.p[small_index].bet_for_round = t.p[small_index].stack;
		t.p[small_index].stack = 0;
	}
	else
	{
		t.p[small_index].stack -= SMALL_BLIND;
		t.p[small_index].bet_for_round = SMALL_BLIND;
	}

/*
	clearScreen();
	writeToLCD("Deducting small", 0);
	sprintf(buffer, "blind from player %d", small_index + 1);
	writeToLCD(buffer, 1);
	delay(2000);
*/

	printf("Deducted small blind from player %d\n", small_index + 1);

	int big_index = next_player_game(small_index);
	if (t.p[big_index].stack < BIG_BLIND)
	{
		t.p[big_index].bet_for_round = t.p[big_index].stack;
		t.p[big_index].stack = 0;

	}
	else
	{
		t.p[big_index].stack -= BIG_BLIND;
		t.p[big_index].bet_for_round = BIG_BLIND;
	}

/*
	clearScreen();
	writeToLCD("Deducting big", 0);
	sprintf(buffer, "blind from player %d", big_index + 1);
	writeToLCD(buffer, 1);
	delay(2000);
*/

	printf("Deducted big blind from player %d\n", big_index+1);

	t.pot = BIG_BLIND + SMALL_BLIND;

	// input AI's cards into system
	if (t.is_AI == 1 && t.p[t.AI_index].in_game == 1)
		input_AI_cards();

	// cards are then dealt, blinds set, now betting round
	int amount_to_call = BIG_BLIND;

	// start is the current index of the player who needs to bet
	int start = big_index;

/*
	clearScreen();
	writeToLCD("Betting round", 0);
	delay(2000);
*/

	printf("Pre-flop betting round\n");

	int winner = betting_round(start, amount_to_call);

	// once betting round is over, set eveyone's bet for round to 0
	for (int i = 0; i < t.total_players; ++i)
		t.p[i].bet_for_round = 0;

	return winner;
}

int flop()
{

/*
	clearScreen();
	writeToLCD("Flop is starting", 0);
	delay(2000);
*/

	printf("Flop is starting\n");
	if (t.is_AI == 1 && t.p[t.AI_index].in_hand == 1)
		input_flop_cards();

	int start = t.dealer_button;
	if (t.p[start].in_hand == 0)
	{
		start = next_player_hand(start);
	}

/*
	clearScreen();
	writeToLCD("Flop betting round", 0);
	delay(2000);
*/

	printf("Flop betting round\n");
	int winner = betting_round(start, 0);

	// once betting round is over, set eveyone's bet for round to 0
	for (int i = 0; i < t.total_players; ++i)
		t.p[i].bet_for_round = 0;

	return winner;
}

int turn()
{

/*
	clearScreen();
	writeToLCD("Turn is starting", 0);
	delay(2000);                  // waits for a second
*/

	printf("Turn is starting\n");
	if (t.is_AI == 1 && t.p[t.AI_index].in_hand == 1)
		input_turn_cards();

	int start = t.dealer_button;
	if (t.p[start].in_hand == 0)
	{
		start = next_player_hand(start);
	}

/*
	clearScreen();
	writeToLCD("Turn betting round", 0);
	delay(2000);
*/

	printf("Turn betting round\n");
	int winner = betting_round(start, 0);

	// once betting round is over, set eveyone's bet for round to 0
	for (int i = 0; i < t.total_players; ++i)
		t.p[i].bet_for_round = 0;

	return winner;
}

int river()
{

/*
	clearScreen();
	writeToLCD("River is starting", 0);
	delay(2000);
*/

	printf("River is starting\n");
	if (t.is_AI == 1 && t.p[t.AI_index].in_hand == 1)
		input_river_cards();

	int start = t.dealer_button;
	if (t.p[start].in_hand == 0)
	{
		start = next_player_hand(start);
	}

/*
	clearScreen();
	writeToLCD("River betting round", 0);
	delay(2000);
*/
	printf("River betting round\n");
	int winner = betting_round(start, 0);

	// once betting round is over, set eveyone's bet for round to 0
	for (int i = 0; i < t.total_players; ++i)
		t.p[i].bet_for_round = 0;

	return winner;
}

void run_Game()
{
	bool done = check_game_over();
	while (!done)
	{
		int who_won;

		who_won = start_hand();

		if (who_won == -1) who_won = flop();

		if (who_won == -1) who_won = turn();

		if (who_won == -1) who_won = river();

		if (who_won == -1)
			input_winner();
		else
			set_winner(who_won);

		done = check_game_over();

		// move dealer button over 1 spot
		t.dealer_button = (t.dealer_button + 1) % t.total_players;
		while (t.p[(t.dealer_button + 1) % t.total_players].in_game == 0)
		{
			t.dealer_button = (t.dealer_button + 1) % t.total_players;
		}
	}
}

void initializeGame()
{
	// call to the interface to prompt and read numpad input, want an int
	t.total_players = get_total_players();
	t.dealer_button = 0;

	for (int i = 0; i < t.total_players; ++i)
	{
		Player a;
		a.stack = INITIAL_STACK;
		a.in_game = 1;
		a.in_hand = 0;
		t.p[i] = a;
	}

	// if AI is enabled, add AI player with index and increase total players
	if (t.is_AI == 1)
	{
		Player a;
		a.stack = INITIAL_STACK;
		a.in_game = 1;
		a.in_hand = 0;
		t.p[t.total_players] = a;
		t.AI_index = t.total_players;
		++t.total_players;
	}

/*
	clearScreen();
	writeToLCD("Time to start", 0);
	writeToLCD("the game", 1);
	delay(2000);
*/
	printf("Starting game!\n");

	run_Game();
}

int main()
{
	srand(time(NULL));
	initializeGame();
}