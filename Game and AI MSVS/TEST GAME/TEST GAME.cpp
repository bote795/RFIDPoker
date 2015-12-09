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

// clears the buffer we use to print to LCD
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

// clears the LCD screen
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

// used to input cards for the AI
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

// start is the player index in table, amount_to_call is the current max bet
// returns the amount the player bet, 0 if check/fold
int get_bet_amount(int start, int amount_to_call)
{
	clearScreen();
	sprintf(buffer, "Player %d enter bet", start + 1);
	writeToLCD(buffer, 0);
	sprintf(buffer, "Bet for round: %d", t.p[start].bet_for_round);
	writeToLCD(buffer, 1);
	sprintf(buffer, "Amount to call: %d", amount_to_call - t.p[start].bet_for_round);
	writeToLCD(buffer, 2);

	int bet = readFromKeypad(3);

	return bet;
}

int get_AI_bet(int start, int amount_to_call)
{
	clearScreen();
	writeToLCD("AI is calulating bet...", 0);
	sprintf(buffer, "Bet for round: %d", t.p[start].bet_for_round);
	writeToLCD(buffer, 1);
	sprintf(buffer, "Amount to call: %d", amount_to_call - t.p[start].bet_for_round);
	writeToLCD(buffer, 2);
	delay(2000);

	int bet = AI_bet(amount_to_call - t.p[start].bet_for_round, t.p[start].bet_for_round, t.pot, t.p[start].stack);
	
	return bet;
}

// prompts users for total num of players (2 to 10)
// and if an AI is playing (1 = YES, else = NO)
int get_total_players()
{
	clearScreen();
	writeToLCD("*Input total number", 0);
	writeToLCD("of players (n < 10):", 1);
	int total = readFromKeypad(2);
	
	// ask if AI is playing
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
	
	// if only 1 player has a non-zero stack, then game is over!
	return (count == 1);
}

// return the index of the next player that is still in game from the base index
int next_player_game(int base_index)
{
	// modulus for wrap-around
	int next_index = (base_index + 1) % t.total_players;
	
	// while this person at this index is NOT in the game, find next player
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
	// modulus for wrap-around
	int next_index = (base_index + 1) % t.total_players;
	
	// while this person at this index is NOT in the hand, find next player
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
	clearScreen();
	writeToLCD("Scan AI card 1", 0);
	a.c1 = findCards();
	clearScreen();
	writeToLCD("Scan AI card 2", 0);
	a.c2 = findCards();
	a.initial_stack = t.p[t.AI_index].stack;
}

// input the flop cards via the RFID scanner
void input_flop_cards()
{
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
}

// input the turn cards via the RFID scanner
void input_turn_cards()
{
	clearScreen();
	writeToLCD("Scan turn card", 0);
	a.table_cards[3] = findCards();
	a.table_size = 4;
}

// input the river cards via the RFID scanner
void input_river_cards()
{
	clearScreen();
	writeToLCD("Scan river card", 0);
	a.table_cards[4] = findCards();
	a.table_size = 5;
}

// prints to players to see who winner is, gets input, return value is player index
int get_winner()
{
	bool valid = false;
	int winner = -1;
	while (!valid)
	{
		clearScreen();
		writeToLCD("Enter hand winner: ", 0);
		
		// if there is AI and AI is still in hand, show which player is AI
		if (t.is_AI == 1 && t.p[t.AI_index].in_hand == 1)
		{
			sprintf(buffer, "AI is player %d", t.AI_index + 1);
			writeToLCD(buffer, 1);
			winner = readFromKeypad(2);
		}
		else
			winner = readFromKeypad(1);
		
		// check to make sure inputted player is in hand
		for (int i = 0; i < t.total_players; ++i)
		{
			if (t.p[i].in_hand == 1 && winner - 1 == i)
				valid = true;
		}
	}
	
	// winner - 1 since indexing, player 1 = index 0
	return winner - 1;
}

// set the winner of the hand
void set_winner(int who_won)
{
	// increase winner's stack
	t.p[who_won].stack += t.pot;
	t.pot = 0;
	
	// display who won and their new stack
	clearScreen();
	if(who_won+1 == t.AI_index)
		sprintf(buffer, "AI won");
	else
		sprintf(buffer, "Player %d won", who_won + 1);
	writeToLCD(buffer, 0);

	sprintf(buffer, "Winner's stack: %d", t.p[who_won].stack);
	writeToLCD(buffer, 1);
	delay(3000);

	// if some1 stack is now 0, set them out of the game and hand
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
	clearScreen();
	writeToLCD("SHOWDOWN TIME", 0);
	delay(2000);

	int winner = get_winner();

	set_winner(winner);
}

// controls the betting round and determines when it is over
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
		
		// if the player already bet the max bet, he doesnt need to bet
		// and its not the start of a round (where it starts at 0)
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
		
		// display message of player's bet
		clearScreen();
		if (start == t.AI_index)
			sprintf(buffer, "AI bet %d", bet);
		else
			sprintf(buffer, "Player %d bet %d", start + 1, bet);
		writeToLCD(buffer, 0);
		
		// check to see if bet is more than lowest stack can bet
		if (bet > (smallest_stack - t.p[start].bet_for_round))
		{
			sprintf("Max bet is %d", smallest_stack);
			writeToLCD(buffer, 1);
			sprintf(buffer, "Now bet set to %d", smallest_stack - t.p[start].bet_for_round);
			writeToLCD(buffer, 2);
			
			// their bet is now set to smallest stack minus how much we already bet
			bet = smallest_stack - t.p[start].bet_for_round;
		}
		
		// update their round bet and stack (will change back if fold)
		t.p[start].bet_for_round += bet;
		t.p[start].stack -= bet;

		// check the bet
		if (t.p[start].stack == 0)
		{
			t.pot += bet;
			t.is_all_in = 1;
			
			// update max bet if their bet is more (when ALL IN)
			if (t.p[start].bet_for_round > max_bet)
				max_bet = t.p[start].bet_for_round;
			
			writeToLCD("All in", 3);
		}
		else if (t.p[start].bet_for_round == max_bet)
		{
			// they called the current max bet
			t.pot += bet;

			writeToLCD("Called", 3);
		}
		else if (t.p[start].bet_for_round > max_bet)
		{
			sprintf(buffer, "Raised by %d", t.p[start].bet_for_round - max_bet);
			writeToLCD(buffer, 3);

			// they raised the current max bet
			max_bet = t.p[start].bet_for_round;
			t.pot += bet;
		}
		else
		{
			// they folded
			writeToLCD("Folded", 3);
			
			// update table values
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
		
		// delay for displaying is a player is ALL IN, called, raised, or folded
		if (start == t.AI_index)
			delay(1000);
		delay(2000);
	}

	// check if betting round is done
	// occurs when all players in the hand have bet the same amount for the round
	bool done = true;
	int bet = -1;
	for (int i = 0; i < t.total_players; ++i)
	{
		if (t.p[i].in_hand == 1)
		{
			if (bet == -1)
				bet = t.p[i].bet_for_round;
				
			// if anyone still in hand does not have the same bet for round
			// then we need another bet round
			if (t.p[i].bet_for_round != bet)
			{
				done = false;
				break;
			}
		}
	}

	// if everyone's bet in this round is the same, then we are done,
	// else need to still loop thru the players still in hand that need to bet
	if (!done)
		who_won = betting_round(start, max_bet);

	return who_won;
}

int start_hand()
{
	clearScreen();
	writeToLCD("Hand is starting", 0);
	delay(2000);

	clearScreen();
	sprintf(buffer, "Player %d has", t.dealer_button + 1);
	writeToLCD(buffer, 0);
	writeToLCD("dealer chip", 1);
	delay(2000);

	// set everyone who is in game to be in hand as well
	// clear bet for round for everyone
	for (int i = 0; i < t.total_players; ++i)
	{
		if (t.p[i].in_game == 1)
			t.p[i].in_hand = 1;
		else
			t.p[i].in_hand = 0;
		t.p[i].bet_for_round = 0;
	}
	
	// reset all in status
	t.is_all_in = 0;
	
	// find the small blind player and deduct
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

	clearScreen();
	writeToLCD("Deducting small", 0);
	sprintf(buffer, "blind from player %d", small_index + 1);
	writeToLCD(buffer, 1);
	delay(2000);
	
	// find the big blind player and deduct
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
	
	clearScreen();
	writeToLCD("Deducting big", 0);
	sprintf(buffer, "blind from player %d", big_index + 1);
	writeToLCD(buffer, 1);
	delay(2000);
	
	// update pot
	t.pot = BIG_BLIND + SMALL_BLIND;

	// input AI's cards into system
	if (t.is_AI == 1 && t.p[t.AI_index].in_game == 1)
		input_AI_cards();

	// cards are then dealt, blinds set, now betting round
	int amount_to_call = BIG_BLIND;

	// start is the current index of the player who needs to bet
	int start = big_index;
	
	clearScreen();
	writeToLCD("Betting round", 0);
	delay(2000);
	
	int winner = betting_round(start, amount_to_call);

	// once betting round is over, set eveyone's bet for round to 0
	for (int i = 0; i < t.total_players; ++i)
		t.p[i].bet_for_round = 0;

	return winner;
}

int flop()
{
	clearScreen();
	writeToLCD("Flop is starting", 0);
	delay(2000);
	
	// if there is an AI, it is still in hand, and no one is ALL IN
	// input the flop cards
	if (t.is_AI == 1 && t.p[t.AI_index].in_hand == 1 && t.is_all_in == 0)
		input_flop_cards();
	
	// update to find starting player for betting round
	int start = t.dealer_button;
	if (t.p[start].in_hand == 0)
	{
		start = next_player_hand(start);
	}
	
	clearScreen();
	writeToLCD("Flop betting round", 0);
	delay(2000);
	
	int winner = betting_round(start, 0);

	// once betting round is over, set eveyone's bet for round to 0
	for (int i = 0; i < t.total_players; ++i)
		t.p[i].bet_for_round = 0;

	return winner;
}

int turn()
{
	clearScreen();
	writeToLCD("Turn is starting", 0);
	delay(2000);
	
	// if there is an AI, it is still in hand, and no one is ALL IN
	// input the turn cards
	if (t.is_AI == 1 && t.p[t.AI_index].in_hand == 1 && t.is_all_in == 0)
		input_turn_cards();
	
	// update to find starting player for betting round
	int start = t.dealer_button;
	if (t.p[start].in_hand == 0)
	{
		start = next_player_hand(start);
	}
	
	clearScreen();
	writeToLCD("Turn betting round", 0);
	delay(2000);
	
	int winner = betting_round(start, 0);
	
	// once betting round is over, set eveyone's bet for round to 0
	for (int i = 0; i < t.total_players; ++i)
		t.p[i].bet_for_round = 0;

	return winner;
}

int river()
{
	clearScreen();
	writeToLCD("River is starting", 0);
	delay(2000);
	
	// if there is an AI, it is still in hand, and no one is ALL IN
	// input the river cards
	if (t.is_AI == 1 && t.p[t.AI_index].in_hand == 1 && t.is_all_in == 0)
		input_river_cards();
	
	// update to find starting player for betting round
	int start = t.dealer_button;
	if (t.p[start].in_hand == 0)
	{
		start = next_player_hand(start);
	}
	
	clearScreen();
	writeToLCD("River betting round", 0);
	delay(2000);
	
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
		
		// if who_won == -1, we do not know the winner yet
		// else then we know who won already so set the winner
		if (who_won == -1) who_won = flop();

		if (who_won == -1) who_won = turn();

		if (who_won == -1) who_won = river();
		
		// if we dont know who won after river, need showdown
		// else set the winner
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
	
	clearScreen();
	writeToLCD("Time to start", 0);
	writeToLCD("the game", 1);
	delay(2000);
	
	run_Game();
}