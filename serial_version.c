#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "timer.h"

#define MAX_RANDOM 65535


//Linked List node          
struct list_node_s {
    int data;
    struct list_node_s* next;
};

float f_insert, f_delete, f_member;
int sample;
struct list_node_s* head = NULL;

int Insert(int value, struct list_node_s** head_pp);

int Delete(int value, struct list_node_s** head_pp);

int Member(int value, struct list_node_s* head_p);

void getInputs(int argc, char* argv[]);

void shuffle(int* array, size_t n);

int calculateSTD(double time_list[], int samples, double mean);

void freeMemory(struct list_node_s** head_pp);

int main(int argc, char* argv[]) {

    int n = 1000;
    int m = 10000;
    getInputs(argc, argv);

    int m_insert, m_delete, m_member;
    int randomarray[m];
    int operations[m];
    double start, finish, elapsed,mean;
    double total_time = 0 ;
    double time_list[sample];
    //int noOfThreads;

    m_member = m*f_member;
    m_insert = m*f_insert;
    m_delete = m*f_delete;
    
    int j;
    for(j= 0 ; j<sample; j++){
        int i = 0;
        while (i < n) {
            int x = rand() % MAX_RANDOM;
            if (Insert(x, &head) == 1){
                randomarray[i] = x;
                i++;
            }
        }

        for(i = 0; i < m-n; i++){
            randomarray[i+n] = rand() % MAX_RANDOM;
        }

        for(i = 0; i < m_member ; i++){
            operations[i] = 0;
        }
        for(i = m_member; i < m_member+m_insert ; i++){
            operations[i] = 1;
        }
        for(i = m_member+m_insert; i < m_member+m_insert+m_delete ; i++){
            operations[i] = 2;
        }

        shuffle(operations, m);
        shuffle(randomarray, m);
        int count_tot = 0;

        GET_TIME(start);
        for(count_tot =0 ; count_tot < m ; count_tot++){

            int rand_select = operations[count_tot];
            int in_value = randomarray[count_tot];

            if (rand_select == 0) {
                Member(in_value, head);
            }

            else if (rand_select == 1) {
                Insert(in_value, &head);
            }

            else if (rand_select == 2) {
                Delete(in_value, &head);
            }
        }

        GET_TIME(finish);
        elapsed = finish - start;
        time_list[j] = elapsed;
        total_time += elapsed;
        //printf("   Elapsed time = %e seconds\n", elapsed);
        freeMemory(&head);
        
    }
    mean = total_time/sample;
    calculateSTD(time_list, sample, mean);
    return 0;
}


int Member(int value, struct list_node_s* head_p){
    struct list_node_s* curr_p = head_p;

    while(curr_p != NULL && curr_p->data < value)
        curr_p = curr_p ->next;

    if(curr_p == NULL || curr_p->data > value){
        return 0;
    }else{
        return 1;
    }
}


int Insert(int value, struct list_node_s** head_pp) {
    struct list_node_s* curr_p = *head_pp;
    struct list_node_s* pred_p = NULL;
    struct list_node_s* temp_p = NULL;

    while (curr_p != NULL && curr_p->data < value) {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p == NULL || curr_p->data > value) {
        temp_p = malloc(sizeof(struct list_node_s));
        temp_p->data = value;
        temp_p->next = curr_p;

        if (pred_p == NULL)  /* New head node */
            *head_pp = temp_p;
        else
            pred_p->next = temp_p;

        return 1;
    }
    else /* Value Already in list */
        return 0;
}


int Delete(int value, struct list_node_s** head_pp) {
    struct list_node_s* curr_p = *head_pp;
    struct list_node_s* pred_p = NULL;

    while (curr_p != NULL && curr_p->data < value) {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p != NULL && curr_p->data == value) {
        if (pred_p == NULL) {
            *head_pp = curr_p->next;
            free(curr_p);
        }
        else {
            pred_p->next = curr_p->next;
            free(curr_p);
        }
        return 1;
    }
    else
        return 0;
}


/*------------------------------------------------------------------
 * Function:    Get_args
 * Purpose:     Get the command line args
 * In args:     argc, argv
 * Globals out: thread_count, n
 */
void getInputs(int argc, char* argv[]) {
    if (argc != 5)
    {
        printf("Please give the command: ./serial_linked_list <f_Member> <f_Insert> <f_Delete> <sample size>\n");
        exit(0);
    }

    f_member = (float) atof(argv[1]);
    f_insert = (float) atof(argv[2]);
    f_delete = (float) atof(argv[3]);
    sample = (int) strtol(argv[4], (char**)NULL, 10);

   if ( f_member + f_insert + f_delete!=1.0) {
	   printf("Please enter valid fractions\n");
       exit(0);
   }

} 


void shuffle(int* array, size_t n)
{
    if (n > 1) 
    {
        size_t i;
        for (i = 0; i < n - 1; i++) 
        {
          size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
         
          int t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}

int calculateSTD(double time_list[], int samples, double mean){
  int i;
  float std=0;
  float temp=0.0;
  float min_samples;
  for(i=0; i<samples; i++){
    time_list[i] -= mean;
    temp = time_list[i]*time_list[i];
    std += temp;
  }
  std = std/samples;
  std = sqrt(std);
  min_samples = pow((100*1.96*std)/(5*mean),2);
  printf("Average time spent = %f\n",mean);
  printf("Standard Deviation = %f\n",(std));
  printf("Minimum samples need = %f\n", min_samples);

  return 0;
}


/* Function:   freeMemory
 * Purpose:    Free the storage used by the list
 * In/out arg: head_pp, pointer to the head of the list pointer
 *               on input, NULL on return
 */
void freeMemory(struct list_node_s** head_pp) {
	struct list_node_s* curr_p;
	struct list_node_s* succ_p;

	if (head_pp == NULL)
		return;

	curr_p = *head_pp;
	succ_p = curr_p->next;

	while (succ_p != NULL) {
		free(curr_p);
		curr_p = succ_p;
		succ_p = curr_p->next;
	}

	free(curr_p);
	*head_pp = NULL;
}