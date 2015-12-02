int AI_bet(int amount_to_call, int bet_for_round, int pot, int stack)
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
  return amount_to_call + 5;
}

void preflop_value()
{
  char s1 = a.c1.suit;
  char s2 = a.c2.suit;
  int n1 = a.c1.number;
  int n2 = a.c2.number;
  
  int value = 0;
  
  if(n1 == n2)                          // pocket pair
  {
    if(n1 == 14 || n1 == 13)                  // A-A, K-K
    {
      value = 100;
    }
    else if(n1 == 12 || n1 == 11)               // Q-Q, J-J
    {
      value = 95;
    }
    else if(n1 == 10)                     // 10-10
    {
      value = 90;
    }
    else if(n1 > 6)                     // 7:9-7:9 pair
    {
      value = 80;
    }
    else                            // 2:6-2:6 pair
    {
      value = 60;
    }
  }
  else if(n1 == 14 || n2 == 14)                 // exactly 1 Ace
  {
    if(s1 == s2)                        // suited
    {
      if(n1 == 13 || n2 == 13)                // A-K
      {
        value = 90;
      }
      else if(n1 == 12 || n2 == 12 || n1 == 11 || n2 == 11) // A-Q, A-J
      {
        value = 80;
      }
      else if(n1 == 10 || n2 == 10)             // A-10
      {
        value = 70;
      }
      else if(n1 > 6 || n2 > 6)               // A-7:9
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
      if(n1 == 13 || n2 == 13)                // A-K
      {
        value = 85;
      }
      else if(n1 == 12 || n2 == 12 || n1 == 11 || n2 == 11) // A-Q, A-J
      {
        value = 75;
      }
      else if(n1 == 10 || n2 == 10)             // A-10
      {
        value = 65;
      }
      else if(n1 > 6 || n2 > 6)               // A-7:9
      {
        value = 60;
      }
      else                          // A-2:6
      {
        value = 50;
      }
    }
  }
  else if(n1 >= 11 && n2 >= 11)
  {
    if(s1 == s2)                        // suited
    {
      if((n1 == 13 || n2 == 13) && (n1 >= 11 || n2 >= 11))  // K-Q, K-J
      {
        value = 80;
      }
      else if((n1 == 12 || n2 == 12) && (n1 == 11 || n2 == 11)) // Q-J
      {
        value = 75;
      }
      else                          // K:J-2:10
      {
        if((n1 >= 11 || n2 >= 11) && (n1 == 10 || n2 == 10))
          value = 55;
        else
          value = 45;
      }
    }
    else                            // unsuited
    {
      if((n1 == 13 || n2 == 13) && (n1 >= 11 || n2 >= 11))  // K-Q, K-J
      {
        value = 75;
      }
      else if((n1 == 12 || n2 == 12) && (n1 == 11 || n2 == 11)) // Q-J
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
  else if((n1 == 10 || n2 == 10) || (n1 == 9 || n2 == 9))     // one 10 or one 9
  {
    if(s1 == s2)                        // suited
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
  if((n1 == (n2+1)) || (n1 == (n2-1)))            // sequential
  {
    if(s1 == s2)                        // suited
    {
      if((n1 == 9) || (n2 == 9) || (n1 == 8) || (n2 == 8))  // 7-8, 8-9, 9-10
      {
        value += 20;
      }
      else if(!((n1 >= 12) || (n2 >= 12)))            // not face card
      {
        value += 10;
      }
    }
    else
    {
      if((n1 == 9) || (n2 == 9) || (n1 == 8) || (n2 == 8))  // 7-8, 8-9, 9-10
      {
        value += 15;
      }
      else if(!((n1 >= 12) || (n2 >= 12)))            // not face card
      {
        value += 5;
      }
    }
  }
  
}


void postflop_value()
{
  // n1 & n2 == anything on table, ++ count for number
  char s1 = a.c1.suit;
  char s2 = a.c2.suit;
  int n1 = a.c1.number;
  int n2 = a.c2.number;
  
  int num_count1 = 1; 
  if(n1 == n2)
  {
    ++num_count1;
  }
  int num_count2 = num_count1;
  
  int suit_count1 = 1;
  if(s1 == s2)
  {
    ++suit_count1;
  }
  int suit_count2 = suit_count1;
  
  // straight
  bool straight = false;
  bool straight_four = false;
  int straight_high = 0;
  int numbers[15] = {0};
  numbers[n1] = 1;
  numbers[n2] = 1;
  if(n1 == 14 || n2 == 14)
    numbers[1] = 1;

  for(int i = 0; i < a.table_size; ++i)
  {
    char s = a.table_cards[i].suit;
    int n = a.table_cards[i].number;
    
    if(s1 == s)
    {
      ++suit_count1;
    }
    
    if(s2 == s)
    {
      ++suit_count2;
    }
    
    if(n1 == n)
    {
      ++num_count1;
    }
    
    if(n2 == n)
    {
      ++num_count2;
    }
    
    ++numbers[n];

    int num_in_row = 0;
    bool start = false;
    for(int j = 1; j < 15; ++j)
    {
      //printf("index: %d had count = %d\n", j, numbers[j]);
      if(numbers[j] != 0)
      {
        start = true;
        num_in_row++;
      }
      if(num_in_row == 4)
      {
        straight_four = true;
      }
      if(num_in_row == 5)
      {
        straight = true;
        straight_high = j;
        break;
      }
      if(start && numbers[j] == 0)
      {
        num_in_row = 0;
        start = false;
      }
    }
    
  }

  int high = 0;
  for(int i = 0; i < 15; ++i)
  {
    if(numbers[i] != 0)
      high = i;
  }

  // flush
  bool flush = false;
  bool flush_4 = false;
  bool flush_3 = false;
  char bestsuit = 'z';
  if(suit_count1 == 5 || suit_count2 == 5)
  {
    flush = true;
    if(suit_count1 == 5)
      bestsuit = s1;
    else
      bestsuit = s2;
  }
  else if(suit_count1 == 4 || suit_count2 == 4)
  {
    flush_4 = true;
  }
  else if(suit_count1 == 3 || suit_count2 == 3)
  {
    flush_3 = true;
  }
  else
  {

  }

  int value = 0;
  int best1 = 0;
  int best2 = 0;

  if(straight && flush)
  {
    //printf("Straight flush\n");
  }
  else if(num_count1 == 4 || num_count2 == 4)
  {
    if(num_count1 == 4)
      best1 = n1;
    else
      best1 = n2;
    //printf("4 of a kind with %ds\n", best1);
  }
  else if((num_count1 == 3 && num_count2 == 2) || (num_count2 == 3 && num_count1 == 2))
  {
    if(num_count1 == 3)
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
  }
  else if(flush)
  {
    //printf("Flush with %c\n", bestsuit);  
  }
  else if(straight)
  {
    //printf("Straight with %d high\n", straight_high);
  }
  else if(num_count1 == 3 || num_count2 == 3)
  {
    if(num_count1 == 3)
      best1 = n1;
    else
      best1 = n2;
    //printf("3 of a kind with %ds\n", best1);
  }
  else if(num_count1 == 2 && num_count2 == 2 && n1 != n2)
  {
    //printf("Two pair with %ds and %ds\n", n1, n2);
  }
  else if(num_count1 == 2 || num_count2 == 2)
  {
    if(num_count1 == 2)
      best1 = n1;
    else
      best1 = n2;
    //printf("One pair with %ds\n", best1);
  }
  else if(n1 == high || n2 == high)
  {
    if(n1 == high)
      best1 = n1;
    else
      best1 = n2;
    //printf("High card with %d high\n", best1);
  }
  else
  {
    //printf("Got nothing\n");
  }



  // 
  
}
