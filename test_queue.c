#include <stdio.h>
#include "queue.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

/*
 * Custom testing framework
 */

void dumpQueue(struct Queue *q);


/*
 * Main()
 */
int main(int argc, char** argv)
{
     // start with empty Queue
     struct Queue *Q = malloc(sizeof(struct Queue));
     Q->head = NULL;
     Q->tail = NULL;
     Q->service_time = 2;
     Q->arrival_count = 0;
     Q->departure_count = 0;
     
     // Add first node to queue
     Q->head = CreateNode (100, 1);  
     Q->tail = Q->head;   // Queue has single node {100, 1}
     Q->arrival_count ++;
     Insert (Q, 101, 7);  // Queue: {100, 1} {101, 7}
     Insert (Q, 102, 11); // Queue: {100, 1} {101, 7} {102, 11}
     Insert (Q, 103, 12); // Queue = {100, 1} {101, 7} {102, 11} {103, 12}
     Insert (Q, 104, 13); // Queue = {100, 1} {101, 7} {102, 11} {103, 12} {104, 13}
     Insert (Q, 105, 27); // Queue = {100, 1} {101, 7} {102, 11} {103, 12} {104, 13} {105, 27}
     dumpQueue(Q);
     printf("Node Count = %5lu\n", CountNodes(Q));   // should return 6
     printf("Node Count With Time >= 10 = %5lu\n", CountNodesAfterTime(Q, 10));  // should return 4
     Delete(Q); // Queue should be {101, 7} {102, 11} {103, 12} {104, 13} {105, 27}
     dumpQueue(Q);
     printf("Average Response Time for customers after time 11 = %f\n", 
             AverageResponseTime(Q, 28));  // should return 2.75
     printf("Utilization after time 11 = %5.3f\n", Utilization(Q, 28));  // should return 8/18 = 0.444
     

     // free queue
     struct QueueNode *ptr = Q->head;
     while (ptr) {
		    Q->head = ptr->next;
		    free (ptr);
		    ptr = Q->head;
     }
     Q->tail = NULL;
     free(Q);
     return 0;
}

/*
 * Helper Functions
 */
void dumpQueue(struct Queue *q)
{
	struct QueueNode *current = q->head;
	while (current != NULL) {
		printf("{%d,%lu} ", current->item, current->arrival_time);
		current = current->next;
	}
	printf("\n");
  printf("Total Arrivals = %lu\n", q->arrival_count);
  printf("Total Departures = %lu\n", q->departure_count);
  
}


