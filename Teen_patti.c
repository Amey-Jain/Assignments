#include <stdio.h>
#include <stdlib.h>

/* 
   README 
   This program was compiled on gcc (Ubuntu 7.3.0-27ubuntu1~18.04) 7.3.0
*/

/* 
   Card to data mapping 
   HEARTS  [A, 1, 2.......K] -> [0,12]
   SPADES  [A, 1, 2.......K] -> [13,25]
   DIAMOND [A, 1, 2.......K] -> [26,38]
   CLUBS   [A, 1, 2.......K] -> [39,51]
*/


//Logic for getting card number and color
#define CARD_NUM(X) ((X % 13) + 1)
#define CARD_CLR(X) ((X / 13) + 1)

#define GREEN  "\x1B[32m"

struct card {
  int card_rank;
  int card_clr;
};

struct set {
  struct card cards[3];
};

/*
Stores three integers into a set structure in an ascending order of rank
*/
struct set *to_set(int X[3]) {
  int i,j;
  struct set *arranged_ascending_set = malloc(sizeof(struct set));
  struct card temp;
  //storing in set
  for(i=0; i < 3; i++) {
    arranged_ascending_set->cards[i].card_rank = CARD_NUM(X[i]);
    arranged_ascending_set->cards[i].card_clr  = CARD_CLR(X[i]);
  }

  //sorting cards inside set
  for(i=2; i>0; i--) {
    for(j=0; j<i; j++){
      if(arranged_ascending_set->cards[j].card_rank > arranged_ascending_set->cards[j+1].card_rank)
	//swap
	{
	  temp = arranged_ascending_set->cards[j];
	  arranged_ascending_set->cards[j] = arranged_ascending_set->cards[j+1];
	  arranged_ascending_set->cards[j+1] = temp;
	}
    }
  }

  /* DEBUG
  for(i=0; i<3; i++)
    printf("\nCARD : %d %d",arranged_ascending_set->cards[i].card_rank,arranged_ascending_set->cards[i].card_clr);
  printf("\n---\n");
  */
  return arranged_ascending_set;
}


// Elementary test operations
// After closely analysing problem to find winning set the problem drills down to three tests
// 1. Color test - If all cards of same color or not
// 2. Sequence test - If all cards are in same order or not
// 3. Rank test - If all cards have same ranks or not
// Note: Functionalities of rank and sequence test could have been contained in same function
// but are kept initially saperate for simplification.
//
// There are 6 orders in which a set can win
// 1. Trail - All cards are of same rank (same_rank_test)
// 2. Pure sequence - All cards are of same color and in sequence (color_test -> sequence_test)
// 3. Sequence - All cards are in sequence (sequence_test)
// 4. Color - All cards are of same color (color_test)
// 5. Pair - Two of cards have same rank (same_rank_test)
// 6. Highest card - Card of highest rank 
//
// Comments in functions
// X, Y and Z denote cards
// Square brackets are used to tell if they are in same or different group for which they are tested


// Color test
int color_test(struct set arranged_set){
  if(arranged_set.cards[0].card_clr == arranged_set.cards[1].card_clr) {
    if(arranged_set.cards[0].card_clr == arranged_set.cards[2].card_clr) {
      //[X,Y,Z] All of same color
      return 1;
    }
    else {
      //[X,Y][Z] Two of same color
      return 3;
    }
  }
  else {
    if(arranged_set.cards[0].card_clr == arranged_set.cards[2].card_clr) {
      //[X,Z][Y] Two of same color
      return 4;
    }
    else {
      if(arranged_set.cards[1].card_clr == arranged_set.cards[2].card_clr) {
	//[X][Y,Z] Two of same color
	return 2;
      }
      else {
	//[X][Y][Z] All three different colors
	return 5;
      }
    }
  }
}

// Sequence test
int sequence_test(struct set arranged_set, int *rank) {
  if(arranged_set.cards[1].card_rank - arranged_set.cards[0].card_rank== 1) {
    if(arranged_set.cards[2].card_rank - arranged_set.cards[1].card_rank == 1) {
      //[X,Y,Z] All cards are in sequence
      //for exception of sequence [A 2 3]
      if(arranged_set.cards[0].card_rank == 1){
	*rank = 14;
	return 1;
      }
      
      *rank = arranged_set.cards[2].card_rank;
      return 1;
    }
    else {
      //[X,Y][Z] Two cards are in sequence, considered for highest ranking
      *rank = arranged_set.cards[2].card_rank;
      return 6;
    }
  }
  else {
    if(arranged_set.cards[2].card_rank - arranged_set.cards[1].card_rank == 1) {
      //[X][Y,Z] Two cards are in sequence
      //for exception of sequence [Q K A]
      if((arranged_set.cards[0].card_rank == 1) && (arranged_set.cards[1].card_rank == 12)) {
	*rank = 15;
	return 1;
      }

      *rank = arranged_set.cards[2].card_rank;
      return 2;
    }
    else {
      //[X][Y][Z] None of cards are in sequence. Returning for highest card.
      *rank = arranged_set.cards[2].card_rank;
      if(arranged_set.cards[0].card_rank == 1) //Its a Ace
	*rank = 15;
      return 4;
    }
  }
}

// Same rank test
int same_rank_test(struct set arranged_set, int *rank) {
  if(arranged_set.cards[0].card_rank == arranged_set.cards[1].card_rank){
    if(arranged_set.cards[1].card_rank == arranged_set.cards[2].card_rank) {
      //[X,Y,Z] 1
      *rank = arranged_set.cards[0].card_rank;
      if(*rank == 1) //its an [A,A,A]
	*rank = 15;
      return 1;
    }
    else {
      //[X,Y][Z] Pair
      *rank = arranged_set.cards[1].card_rank;
      if(*rank == 1) //its an [A,A,--]
	*rank = 15;
      return 3;
    }
   }
  else {
    if(arranged_set.cards[1].card_rank == arranged_set.cards[2].card_rank) {
      //[X][Y,Z] Pair
      *rank = arranged_set.cards[1].card_rank;
      if(*rank == 1) //its an [--,A,A]
	*rank = 15;
      return 2;
    }
    else {
      //[X][Y][Z] Returning for highest card
      *rank = arranged_set.cards[2].card_rank;
      if(arranged_set.cards[0].card_rank == 1) //its an A
	*rank = 15;
      return 4;
    }
  }
}

// This function applies elementary test (color, sequence and rank) by taking their
// output return values and ranking to optimize order of set. 
// Input take an integer array and return set order(trail, pure sequence, sequence..etc) and
// its respective set rank
int get_order_of_set(int x[3], int *rank) {
  struct set *ptr = to_set(x);
  struct set ptr_set;
  int i,j;
  for(i=0;i < 3; i++)
    ptr_set.cards[i] = ptr->cards[i];

  //color_test
  if(color_test(ptr_set) == 1) {
    //sequence test
    if(sequence_test(ptr_set, rank) == 1) {
      //its a pure sequence, return 2
      return 2;
    }
    else {
      //it doesn't have any sequence and is all three are same color so it can only make a Highest card
      j = sequence_test(ptr_set, rank);
      *rank = j;
      /* printf("DEBUG1: Sequence %d rank %d", j, *rank); debug */
      return 2;
    }
  }
  else {
    if(sequence_test(ptr_set, rank) == 1) {
      //its a sequence
      return 3;
    }
    else {
      int temp_res = same_rank_test(ptr_set, rank);
      if(temp_res == 1) {
	//Its a trail
	return 1;
      }
      else if(temp_res == 3 || temp_res == 2) {
	//Its a pair
	return 5;
      }
      else {
	j = sequence_test(ptr_set, rank);
	/*	printf("DEBUG2: Sequence %d rank %d", j, *rank); debug */
	return 6;
      }
    }
  }   
}

void show_set(struct set s) {
  int i;
  for(i=0;i<3;i++) {
    //display card rank
    switch(s.cards[i].card_rank) {
    case 1: printf("A");
      break;
    case 11: printf("J");
      break;
    case 12: printf("Q");
      break;
    case 13: printf("K");
      break;
    default: printf("%d", s.cards[i].card_rank);
      break;
    }
    printf("\t");
    switch(s.cards[i].card_clr) {
    case 1: printf("HEARTS");
      break;
    case 2: printf("SPADES");
      break;
    case 3: printf("DIAMONDS");
      break;
    case 4: printf("CLUBS");
      break;
    default: printf("ERROR: Unknown color\n");
      break;
    }
    printf("\n");
  }
}

int main(int argc, char *argv[]) {
  int x[3];
  int i,j,rank,ctr;
  int input_set;
   //default input matrix if values are not supplied
  int input_matrix[5][3] = {{ 0, 1, 2},
			    { 0, 32, 50},
			    { 24, 38, 51},
			    { 13, 14, 15},
			    { 43, 49, 16}};
  int output_matrix[5][3];

  char swtch;
  printf("Do you want to enter manual input(y/n)?\n");
  swtch = getchar();
  getchar();
  
  //Block for manual input
  if(swtch == 'y') {
    printf("Enter number of test cases : ");
    scanf("%d", &input_set);

    int input_mat[input_set][3];
    int output_mat[input_set][3];
    struct set *ptr,ptr_set;
    //input loop
    for(ctr = 0; ctr<input_set; ctr++) {
      printf("Enter set %d: ", ctr + 1);
      scanf("%d %d %d", &input_mat[ctr][0], &input_mat[ctr][1], &input_mat[ctr][2]);
      ptr = to_set(input_mat[ctr]);
      for(i = 0; i<3; i++)
	ptr_set.cards[i] = ptr->cards[i];
      j = get_order_of_set(input_matrix[ctr], &rank);
      printf("\n------ For set %d -------\n",ctr + 1);
      output_mat[ctr][0] = j;
      output_mat[ctr][1] = rank;
      output_mat[ctr][2] = input_set;
      show_set(ptr_set);
    }
    
    //ranker block 
    for(ctr = 0; ctr < input_set ; ctr++) {
      for(j=ctr + 1; j < input_set; j++) {
	if(output_mat[ctr][0] < output_mat[j][0]) 
	  --output_mat[ctr][2];
	else if(output_mat[ctr][0] > output_mat[j][0])
	  --output_mat[j][2];
	else { //if their orders are same then compare ranks
	  if(output_mat[ctr][1] < output_mat[j][1])
	    --output_mat[j][2];
	  else if(output_mat[ctr][1] > output_mat[j][1])
	    --output_mat[ctr][2];
	  else { //if ranks are also same then its a tie
	    --output_mat[j][2];
	    --output_mat[ctr][2];
	  }
	}
      }
    }
    
    for(ctr = 0; ctr < input_set ; ctr++) {
      if(output_mat[ctr][2] == 1)
	printf(GREEN"***** Player %d Won!!! *****\n",ctr + 1);
    }    
  }

  //Block for automatic input from array input_matrix
  else if(swtch == 'n') {
    input_set = 5;
    for(ctr=0;ctr<input_set;ctr++) {
      struct set *ptr,ptr_set;
      ptr = to_set(input_matrix[ctr]);
      for(i=0;i < 3; i++)
	ptr_set.cards[i] = ptr->cards[i];
      /*  printf("color_test %d\n", color_test(ptr_set)); debug */
      //  j = sequence_test(ptr_set, &rank);
      /*  printf("sequence_test %d rank %d\n",j,rank);  debug */
      /*  printf("rank_test %d\n", same_rank_test(ptr_set, &rank)); */
      j = get_order_of_set(input_matrix[ctr], &rank);
      printf("------For set %d------\n",ctr + 1);
      output_matrix[ctr][0] = j;
      output_matrix[ctr][1] = rank;
      output_matrix[ctr][2] = input_set;
      show_set(ptr_set);
    }
    
    //ranker block 
    for(ctr = 0; ctr < input_set ; ctr++) {
      for(j=ctr + 1; j < input_set; j++) {
	if(output_matrix[ctr][0] < output_matrix[j][0]) 
	  --output_matrix[ctr][2];
	else if(output_matrix[ctr][0] > output_matrix[j][0])
	  --output_matrix[j][2];
	else { //if their orders are same then compare ranks
	  if(output_matrix[ctr][1] < output_matrix[j][1])
	    --output_matrix[j][2];
	  else if(output_matrix[ctr][1] > output_matrix[j][1])
	    --output_matrix[ctr][2];
	  else { //if ranks are also same then its a tie
	    --output_matrix[j][2];
	    --output_matrix[ctr][2];
	  }
	}
      }
    }
    
    for(ctr = 0; ctr < input_set ; ctr++) {
      if(output_matrix[ctr][2] == 1)
	printf(GREEN"***** Player %d Won!!! *****\n",ctr + 1);
    }
  }

  else 
    printf("Wrong input. Try again\n");
  return 0;
} 
