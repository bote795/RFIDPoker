#define PERCENT_OFFSET  0.20

// returns a random number within a range
int random_int(float value)
{
  long r;
  int offset = (int)ceil(value*PERCENT_OFFSET);
  int max = (int)ceil(value) + offset;
  int min = (int)ceil(value) - offset;

  return random(min, max+1);
}

int preflop_value()
{
  char s1 = a.c1.suit;
  char s2 = a.c2.suit;
  int n1 = a.c1.number;
  int n2 = a.c2.number;

  int value = 0;

  if (n1 == n2)                         // pocket pair
  {
    if (n1 == 14 || n1 == 13)                 // A-A, K-K
    {
      value = 100;
    }
    else if (n1 == 12 || n1 == 11)                // Q-Q, J-J
    {
      value = 95;
    }
    else if (n1 == 10)                      // 10-10
    {
      value = 90;
    }
    else if (n1 > 6)                      // 7:9-7:9 pair
    {
      value = 80;
    }
    else                            // 2:6-2:6 pair
    {
      value = 60;
    }
  }
  else if (n1 == 14 || n2 == 14)                  // exactly 1 Ace
  {
    if (s1 == s2)                       // suited
    {
      if (n1 == 13 || n2 == 13)               // A-K
      {
        value = 90;
      }
      else if (n1 == 12 || n2 == 12 || n1 == 11 || n2 == 11)  // A-Q, A-J
      {
        value = 80;
      }
      else if (n1 == 10 || n2 == 10)              // A-10
      {
        value = 70;
      }
      else if (n1 > 6 || n2 > 6)                // A-7:9
      {
        value = 65;
      }
      else                          // A-2:6
      {
        value = 55;
      }
    }
    else                            // unsuited
    {
      if (n1 == 13 || n2 == 13)               // A-K
      {
        value = 85;
      }
      else if (n1 == 12 || n2 == 12 || n1 == 11 || n2 == 11)  // A-Q, A-J
      {
        value = 75;
      }
      else if (n1 == 10 || n2 == 10)              // A-10
      {
        value = 65;
      }
      else if (n1 > 6 || n2 > 6)                // A-7:9
      {
        value = 60;
      }
      else                          // A-2:6
      {
        value = 50;
      }
    }
  }
  else if (n1 >= 11 && n2 >= 11)
  {
    if (s1 == s2)                       // suited
    {
      if ((n1 == 13 || n2 == 13) && (n1 >= 11 || n2 >= 11)) // K-Q, K-J
      {
        value = 80;
      }
      else if ((n1 == 12 || n2 == 12) && (n1 == 11 || n2 == 11))  // Q-J
      {
        value = 75;
      }
      else                          // K:J-2:10
      {
        if ((n1 >= 11 || n2 >= 11) && (n1 == 10 || n2 == 10))
          value = 55;
        else
          value = 45;
      }
    }
    else                            // unsuited
    {
      if ((n1 == 13 || n2 == 13) && (n1 >= 11 || n2 >= 11)) // K-Q, K-J
      {
        value = 75;
      }
      else if ((n1 == 12 || n2 == 12) && (n1 == 11 || n2 == 11))  // Q-J
      {
        value = 70;
      }
      else
      {
        if ((n1 >= 11 || n2 >= 11) && (n1 == 10 || n2 == 10))
          value = 50;
        else
          value = 40;
      }
    }
  }
  else if ((n1 == 10 || n2 == 10) || (n1 == 9 || n2 == 9))      // one 10 or one 9
  {
    if (s1 == s2)                       // suited
    {
      value = 30;
    }
    else                            // unsuited
    {
      value = 25;
    }
  }
  else
  {
    value = 0;
  }

  // check for sequential
  if ((n1 == (n2 + 1)) || (n1 == (n2 - 1)))           // sequential
  {
    if (s1 == s2)                       // suited
    {
      if ((n1 == 9) || (n2 == 9) || (n1 == 8) || (n2 == 8)) // 7-8, 8-9, 9-10
      {
        value += 20;
      }
      else if (!((n1 >= 12) || (n2 >= 12)))           // not face card
      {
        value += 10;
      }
    }
    else
    {
      if ((n1 == 9) || (n2 == 9) || (n1 == 8) || (n2 == 8)) // 7-8, 8-9, 9-10
      {
        value += 15;
      }
      else if (!((n1 >= 12) || (n2 >= 12)))           // not face card
      {
        value += 5;
      }
    }
  }

  return value;

}


int postflop_value()
{
  int value = 0;

  // n1 & n2 == anything on table, ++ count for number
  char s1 = a.c1.suit;
  char s2 = a.c2.suit;
  int n1 = a.c1.number;
  int n2 = a.c2.number;

  // count of same number card
  int num_count1 = 1;
  if (n1 == n2)
  {
    ++num_count1;
  }
  int num_count2 = num_count1;

  // count of same number suit
  int suit_count1 = 1;
  if (s1 == s2)
  {
    ++suit_count1;
  }
  int suit_count2 = suit_count1;

  // straight
  bool straight = false;
  bool straight_four = false;
  int straight_high = 0;
  int numbers[15] = { 0 };
  numbers[n1] = 1;
  numbers[n2] = 1;
  if (n1 == 14 || n2 == 14)
    numbers[1] = 1;

  // loop through the table cards and increment suits and number, also check for sequential cards
  for (int i = 0; i < a.table_size; ++i)
  {
    char s = a.table_cards[i].suit;
    int n = a.table_cards[i].number;

    if (s1 == s)
      ++suit_count1;

    if (s2 == s)
      ++suit_count2;

    if (n1 == n)
      ++num_count1;

    if (n2 == n)
      ++num_count2;

    ++numbers[n];

    int num_in_row = 0;
    bool start = false;
    for (int j = 1; j < 15; ++j)
    {
      //printf("index: %d had count = %d\n", j, numbers[j]);
      if (numbers[j] != 0)
      {
        start = true;
        num_in_row++;
      }
      if (num_in_row == 4)
      {
        straight_four = true;
      }
      if (num_in_row == 5)
      {
        straight = true;
        straight_high = j;
        break;
      }
      if (start && numbers[j] == 0)
      {
        num_in_row = 0;
        start = false;
      }
    }

  }

  // look to see if we have high card
  int high = 0;
  for (int i = 0; i < 15; ++i)
  {
    if (numbers[i] != 0)
      high = i;
  }

  // flush
  bool flush = false;
  bool flush_4 = false;
  bool flush_3 = false;
  char bestsuit = 'z';
  if (suit_count1 == 5 || suit_count2 == 5)
  {
    flush = true;
    if (suit_count1 == 5)
      bestsuit = s1;
    else
      bestsuit = s2;
  }
  else if (suit_count1 == 4 || suit_count2 == 4)  // 4 of same suit
  {
    flush_4 = true;
    value = 40;
    return value;
  }
  else if (suit_count1 == 3 || suit_count2 == 3 && a.table_size == 3) // 3 of same suit and its flop
  {
    flush_3 = true;
    value = 10;
    return value;
  }
  else
  {

  }

  int best1 = 0;
  int best2 = 0;

  if (straight && flush)
  {
    //printf("Straight flush\n");
    value = 100;
  }
  else if (num_count1 == 4 || num_count2 == 4)
  {
    if (num_count1 == 4)
      best1 = n1;
    else
      best1 = n2;
    //printf("4 of a kind with %ds\n", best1);
    value = 100;
  }
  else if ((num_count1 == 3 && num_count2 == 2) || (num_count2 == 3 && num_count1 == 2))
  {
    if (num_count1 == 3)
    {
      best1 = n1;
      best2 = n2;
    }
    else
    {
      best1 = n2;
      best2 = n1;
    }
    //printf("Full house with three %ds and two %ds\n", best1, best2);
    value = 95;
  }
  else if (flush)
  {
    //printf("Flush with %c\n", bestsuit);
    value = 90;
  }
  else if (straight)
  {
    //printf("Straight with %d high\n", straight_high);
    value = 85;
  }
  else if (num_count1 == 3 || num_count2 == 3)
  {
    if (num_count1 == 3)
      best1 = n1;
    else
      best1 = n2;
    //printf("3 of a kind with %ds\n", best1);
    value = 80;
  }
  else if (num_count1 == 2 && num_count2 == 2 && n1 != n2)
  {
    //printf("Two pair with %ds and %ds\n", n1, n2);
    value = 70;
  }
  else if (num_count1 == 2 || num_count2 == 2)
  {
    if (num_count1 == 2)
      best1 = n1;
    else
      best1 = n2;
    //printf("One pair with %ds\n", best1);
    if (best1 >= 10)
      value = 60;
    else if (best1 >= 6)
      value = 45;
    else
      value = 30;
  }
  else if (n1 == high || n2 == high)
  {
    if (n1 == high)
      best1 = n1;
    else
      best1 = n2;
    //printf("High card with %d high\n", best1);
    value = 10;
  }
  else
  {
    //printf("Got nothing\n");
    value = 0;
  }

  return value;

  // 

}

#define PERCENT_OF_THRESHOLD_PREFLOP  0.20
#define PERCENT_OF_THRESHOLD_POSTFLOP 0.30
#define PERCENT_ROUND_BET       0.75
#define PERCENT_DIFFERENCE        0.20
#define BLUFF_CHANCE          0.20
#define BLUFF_INCREMENT_PRE       0.20
#define BLUFF_INCREMENT_POST      0.40
#define SLOW_PLAY_CHANCE        0.20
#define CALL_CHANCE           0.30
#define RAISE_PERCENT         0.50

int AI_bet(int amount_to_call, int bet_for_round, int pot, int stack)
{

  // table size needs to be set to 0 at the start of a hand, also clear it out

  int value = 0;
  int initial_stack = a.initial_stack;
  int bet = 0;
  float threshold = 0.0;
  float thres_value = 0.0;

  float bluff = (float)(random(101)) / 100;
  float slow = (float)(random(101)) / 100;
  float call = (float)(random(101)) / 100;
  
  // pre-flop strategy
  // based on two first cards and the bets from others
  // if Ace suited with .... this value is assigned to it
  // if king suit do this ... and so on for all combos
  // bet is based on value of cards, amount to call? /raise, current pot, how much i bet already
  if (a.table_size == 0)
  {
    // value of the pair of cards
    value = preflop_value();

    // maximum bet for round, percent of current stack?, rounded to 5s?
    threshold = (float)value / 100;

    // first time bet for round
    if (bet_for_round == 0)
    {
      thres_value = stack * threshold;
      a.threshold_value = thres_value;

      // random bet within a range
      bet = random_int((float)(thres_value * PERCENT_OF_THRESHOLD_PREFLOP));
    }
    else
    {
      // means we have been raised
      thres_value = a.threshold_value;

      // if the total amount we have bet so far in round
      // plus the amount we need to call to stay,
      // is more than our threshold value
      if ((bet_for_round + amount_to_call) > thres_value)
      {
        // do we call this or do we fold? we already put in bet_for_round with a hand value of threshold
        if(value >= 90)
          bet = amount_to_call;
        else
          bet = thres_value - bet_for_round;
      }
      else
      {
        // should we call or raise? if raise how much do we raise by
        if (call <= CALL_CHANCE)
          bet = amount_to_call;
        else
        {
          // we raise
          // how much to raise by?

          // how much more can we bet till we reach the threshold
          int difference = (int)(thres_value) - (bet_for_round + amount_to_call);

          // lets raise by a random amount thats left, but make it a percent of the total difference
          // like if difference is 200, we only bet from 0-100 (50% of difference)
          float raise_amount = ceil((float)((random(difference+1))*RAISE_PERCENT));
          bet = amount_to_call + (int)raise_amount;
        }
      }
    }

    // if the calculated bet is lower than the amount to call, 
    // do i need to bump up my bet to just call? how much have i already put in
    // how much more do i need to bet to match?
    if (bet < amount_to_call)
    {
      // if the random bet made it go below amount to call when the thres is actually above it
      if (thres_value > (amount_to_call + bet_for_round))
        bet = amount_to_call;

      int difference = amount_to_call - bet;
      int per_dif = (int) (PERCENT_DIFFERENCE*bet);
      if (difference < per_dif)
        bet = amount_to_call;

      // if i already bet X much, i should call to a point... when do i cut my loses
      int per_round = (int) (PERCENT_ROUND_BET*initial_stack);
      if ((initial_stack - stack) > per_round)
        bet = amount_to_call;

      // bluff chance, and if bluff, how much to bluff by is determined by another random variable, 1-75 (75%) = low bluff, 75-95 (20%) = medium bluff, 95-100 (5%) = high bluff
      // should there be chance to under-bet hand? if really good hand, should buff low to fool opponent?
      if (bluff <= BLUFF_CHANCE)
        bet = amount_to_call;

    }
    else
    {
      // we are already going to raise, can make it a crazy raise if we bluff
      if (bluff <= BLUFF_CHANCE)
      {
        bet += (int)(stack*BLUFF_INCREMENT_PRE);
        bet = random_int((float)bet);
      }
    }
  }

  // post-flop
  // current value of hand (is it a pair, a 3 of kind, flush, straight, full house, four of kind)
  // probability of getting one of these hand types, and the value of the hand type
  // bet is based on value of cards, amount to call? /raise, current pot, how much I bet already
  // along with current value of hand with table hands, how much each person put into the hand, probability of getting/not getting a better hand, probability of opponents of other hands
  else
  {
    value = postflop_value();

    // maximum bet for round, percent of current stack?, rounded to 5s?
    threshold = (float)value / 100;

    // first time bet for round
    if (bet_for_round == 0)
    {
      thres_value = stack * threshold;
      a.threshold_value = thres_value;

      // random bet within a range
      bet = random_int((float)(thres_value * PERCENT_OF_THRESHOLD_POSTFLOP));
    }
    else
    {
      // means we have been raised
      thres_value = a.threshold_value;

      // if the total amount we have bet so far in round
      // plus the amount we need to call to stay,
      // is more than our threshold
      if ((bet_for_round + amount_to_call) > thres_value)
      {
        // do we call this or do we fold? we already put in bet_for_round with a hand value of threshold
        if(value >= 80)
          bet = amount_to_call;
        else
          bet = thres_value - bet_for_round;
      }
      else
      {
        // should we call or raise? if raise how much do we raise by
        if (call <= CALL_CHANCE)
          bet = amount_to_call;
        else
        {
          // we raise
          // how much to raise by?

          // how much more can we bet till we reach the threshold
          int difference = (int)(thres_value)-(bet_for_round + amount_to_call);

          // lets raise by a random amount thats left, but make it a percent of the total difference
          // like if difference is 200, we only bet from 0-100 (50% of difference)
          float raise_amount = ceil((float)((random(difference+1))*RAISE_PERCENT));
          bet = amount_to_call + (int)raise_amount;
        }
      }
    }

    // then, find out how much to bet based on min, thresh, amount_to_call, and the bet for round (thres-bet4round, min-bet4round)

    // if the calculated bet is lower than the amount to call, do i need to bump up my bet to just call? how much have i already put in, how much more do i need to bet to match?
    if (bet < amount_to_call)
    {
      // if the random bet made it go below amount to call when the thres is actually above it
      if (thres_value > (amount_to_call + bet_for_round))
        bet = amount_to_call;

      int difference = amount_to_call - bet;
      int per_dif = (int)(PERCENT_DIFFERENCE*bet);
      if (difference < per_dif)
        bet = amount_to_call;

      // if i already bet X much, i should call to a point... when do i cut my loses
      int per_round = (int)(PERCENT_ROUND_BET*initial_stack);
      if ((initial_stack - stack) > per_round)
        bet = amount_to_call;

      // bluff chance, and if bluff, how much to bluff by is determined by another random variable, 1-75 (75%) = low bluff, 75-95 (20%) = medium bluff, 95-100 (5%) = high bluff
      // should there be chance to under-bet hand? if really good hand, should buff low to fool opponent?
      if (bluff <= BLUFF_CHANCE)
        bet = amount_to_call;

    }
    else
    {
      if (bluff <= BLUFF_CHANCE)
      {
        bet += (int)(stack*BLUFF_INCREMENT_POST);
        bet = random_int((float)bet);
      }
      else if (slow <= SLOW_PLAY_CHANCE)
      {
        bet = amount_to_call;
      }
    }

  }
  return bet;
}
