#include <stdio.h>
#include <stdlib.h>

/* 
   README 
   This program was compiled on gcc (Ubuntu 7.3.0-27ubuntu1~18.04) 7.3.0
*/

//Strucutre for accessible rows and columns
struct accessible {
  int *ptr;
  int length;
  int acc_ctr;
};

struct accessible *create(int size) {
  int i;
  struct accessible *rtr = malloc(sizeof(struct accessible));
  rtr->ptr = malloc(sizeof(int) * size);
  rtr->length = size;
  //initialise all elements accessible
  for(i = 0; i < size; i++)
    rtr->ptr[i] = 1;
  rtr->acc_ctr = size;  
  return rtr;
}

int length_of(struct accessible *acc) {
  return acc->length;
}

int remove_access(struct accessible *acc, int index) {
  if(index < acc->length) {
    acc->ptr[index] = 0;
    --(acc->acc_ctr);
    return 1;
  }
  else
    return -1;
}

int grant_access(struct accessible *acc, int index) {
  if(index < acc->length) {
    acc->ptr[index] = 1;
    ++(acc->acc_ctr);
    return 1;
  }
  else
    return -1;
}



//Return index of nth accessible element in acc
int get_nth_accessible(struct accessible *acc, int nth) {
  int i,j,ctr=0;
  for(i = 0; i < acc->length ; i++){
    if(acc->ptr[i] == 1)
      ctr++;
    //return nth accessible element
    if(ctr == nth)
      return i;
  }

  //else return
  return -1;
}

//function to find minimum sum in matrix
int find_min_sum(int **input_matrix, struct accessible *acc_rows, struct accessible *acc_cols) {
  int i,j,sum = 0;
  int min_sum = -1;
  static int pas = 0;
  //if matrix is equal to or greater than 2x2
  if((acc_rows->acc_ctr > 1) && (acc_cols->acc_ctr > 1)) {

    //select first row accessible and iterate on it
    i = get_nth_accessible(acc_rows, 1);

    //Minimum sum to be compared for each value of j
    for(j = 0; j < length_of(acc_cols) ; j++) {
      if(acc_cols->ptr[j] == 1) { //Coloumn is accessible
	
	//debug
	printf("Working for fair %d\t i: %d j: %d\n", input_matrix[i][j],i,j);

	//remove access of i
	remove_access(acc_rows, i);
	remove_access(acc_cols, j);
	sum = input_matrix[i][j] + find_min_sum(input_matrix, acc_rows, acc_cols);

	if(min_sum == -1)
	  min_sum = sum;
	else 
	  min_sum = min_sum < sum ? min_sum : sum;

	printf("Minimum sum: %d sum: %d\n", min_sum,sum);
	
	//grant back access
	grant_access(acc_rows, i);
	grant_access(acc_cols, j);
      } 
    }
    return min_sum;
  }
  
  //only 1 element in matrix provided
  else {
    i = get_nth_accessible(acc_rows, 1);
    j = get_nth_accessible(acc_cols, 1);
    printf("Single element %d\n", input_matrix[i][j]);
    return input_matrix[i][j];
  }
}

void print_matrix(int **input_matrix, int n_src, int n_dst) {
  int i,j;
  //MATRIX PRINT LOOP
  printf("SourceV Destination>\n\n");
  for(i = 0; i < n_src; i++) {
    //Loop for jth destination
    printf("%d   |\t",i + 1);
    for(j = 0; j < n_dst; j++)
      printf("%d\t", input_matrix[i][j]);
    printf("\n-----------------------------------------------------------------\n");
  }

}


int main() {
  int i,j;
  int n_src, n_dst, **input_matrix, **output_matrix, *ptr, *ptr_,size;
  struct accessible *acc_rows, *acc_cols;
  //input
  printf("Enter number of source: ");
  scanf("%d", &n_src);
  printf("Enter number of destinations: ");
  scanf("%d", &n_dst);

  //allocating memory
  //Sources are rows and destination columns
  size = (sizeof(int *) * n_src) + (sizeof(int) * n_src * n_dst);
  input_matrix = (int **) malloc(size);
  output_matrix = (int **) malloc(size);

  //creating accessibility matrix
  acc_rows = create(n_src);
  acc_cols = create(n_dst);
  
  //ptr now points to first element of matrix
  ptr = (int *) (input_matrix + n_src);
  ptr_ = (int *) (output_matrix + n_src);
  //pointing all rows to proper locations
  for(i = 0; i < n_src; i++){
    input_matrix[i] = ptr + (n_dst * i);
    output_matrix[i] = ptr_ + (n_dst * i);
  }

  //INPUT LOOP
  //Loop for ith source
  for(i = 0; i < n_src; i++) {
    //Loop for jth destination
    printf("Enter fairs for Source %d\n", i + 1);
    for(j = 0; j < n_dst; j++){
      printf("\tDestination %d: ", j + 1);
      scanf("%d", &input_matrix[i][j]);

      //initializing output_matrix[i][j]
      output_matrix[i][j] = 0;
    }
  }

  //Print matrix
  printf("Your input matrix is\n");
  print_matrix(input_matrix, n_src, n_dst);

  printf("TEST: find_min_sum function return %d\n", find_min_sum(input_matrix, acc_rows, acc_cols));
}
