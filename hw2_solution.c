/* In the following code, you need to fill in body 
 * of each function
*/
#include "queue.h"
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

/*
 * Allocate memory for a node of type struct QueueNode and
 * initialize it with the item value and its arrival_time;
 * Return a pointer to the new node.
 */
struct QueueNode* CreateNode(int item, uint64 arrival_time) {
    struct QueueNode* node = (struct QueueNode*) malloc(sizeof(struct QueueNode));
    node->item = item;
    node->arrival_time = arrival_time;
    node->next = NULL;
    return node;
}

/*
 * Insert node with specified item and arrival time at the tail of the list.
 * If the queue is empty, then both the head and the tail should point to the new node
 * Should increment the arrival_count of the queue
 */
void Insert (struct Queue *q, int item, uint64 arrival_time) {
    q->arrival_count++;
    if(CountNodes(q) == 0){
        q->head = CreateNode(item,arrival_time);
        q->tail = q->head;
        return;    
    }
    q->tail->next = CreateNode(item,arrival_time);
    q->tail = q->tail->next;
    return;
}

/*
 * Delete node from head of the list and free its allocated memory
 * head pointer should move to the next node in the list. 
 * If head == NULL, do nothing
 * If the queue has one node, then both the head and the tail should point to NULL after the node is deleted
 * Should decrement the arrival_count of the queue
 */
void Delete (struct Queue *q) {
    if(CountNodes(q) <= 0){return;}
    q->departure_count++;
    if(CountNodes(q) == 1){
        free (q->head);
        q->head = NULL;
        q->tail = NULL;
        return;
    }
    struct QueueNode* node = q->head;
    q->head = q->head->next;
    free (node);
    return;
}

/*
 * Count the current number of nodes in the queue.
 * Return 0 if the queue is empty, i.e., head == tail == NULL
 */
uint64 CountNodes (struct Queue *q) {
    uint64 count = 0;
    struct QueueNode *ptr = q->head;
	while (ptr) {
        ptr = ptr->next;
		count++;
     }
    return count;
}

/*
 * Return the first node holding the value item
 * return NULL if none found
 */
struct QueueNode* FindNode(struct Queue *q, int item) {
    struct QueueNode *ptr = q->head;
	while (ptr) {
        if(ptr->item == item){return ptr;}
        ptr = ptr->next;
     }
    return NULL;
}

/*
 * Return a pointer to first node that has an arrival time >= t
 * For example, in the queue head:{1,4}, {2, 80}, {3, 500}, {4, 510}, tail:{5, 640}
 * FindNodeAfterTime(q, 505) returns a pointer to QueueNode {4, 510}
 * FindNodeAfterTime(q, 80) returns a pointer to QueueNode {2, 80}
 */
struct QueueNode* FindNodeAfterTime(struct Queue *q, uint64 t) {
    struct QueueNode *ptr = q->head;
	while (ptr) {
        if(ptr->arrival_time >= t){return ptr;}
        ptr = ptr->next;
     }
    return NULL;
}

/*
 * Return the count of all nodes that have an arrival time >= t
 * For example, in the queue head:{1,4}, {2, 80}, {3, 500}, {4, 510}, tail:{5, 640}
 * CountNodesAfterTime(q, 505) returns 2
 * CountNodesAfterTime(q, 80) returns 4
 */
uint64 CountNodesAfterTime(struct Queue *q, uint64 t) {
    uint64 count = 0;
    struct QueueNode *ptr = FindNodeAfterTime(q,t);
	while (ptr) {
        ptr = ptr->next;
		count++;
     }
    return count;
}

/*
 * Compute the average response time for all nodes in the queue that have arrival_time > current_time
 * You can call FindNodeAfterTime() to find the first node at/after current_time
 * You should traverse the queue and compute the response time for each node. 
 * The response time for a node should be the same as service_time if there are no other nodes in the queue 
 * (that is, the node's arrival_time is >= previous node's arrival_time plus service_time)
 * The response time should be service_time+queueing delay if the current node overlaps with previous node(s)
 * You need to add up all the response times of all nodes, then divide by their count to get an average response time  
 */
float AverageResponseTime(struct Queue *q, uint64 current_time) {
    struct QueueNode *node = FindNodeAfterTime(q,current_time);
    if(node == NULL){return 0;}

    int total_node = CountNodesAfterTime(q,current_time);
    float total_res_time = 0;
    uint64 queuing_delay = 0;
    uint64 server_busy_until = 0;

    struct QueueNode *ptr = q->head;
    while(ptr != node){
        if(ptr->arrival_time < server_busy_until){ queuing_delay = server_busy_until - ptr->arrival_time;}
        else{queuing_delay = 0;}
        server_busy_until = ptr->arrival_time + (queuing_delay + q->service_time);
        ptr = ptr->next;

    }

    while(node){
        if(node->arrival_time < server_busy_until){
            queuing_delay = server_busy_until - node->arrival_time;
        }
        else{queuing_delay = 0;}
        server_busy_until = node->arrival_time + (queuing_delay + q->service_time);
        total_res_time = total_res_time + (queuing_delay + q->service_time);
        node = node->next;
    }

    return total_res_time/total_node;
}

/*
 * Compute the queue utilization for all nodes that have arrival_time > current_time
 * You can call FindNodeAfterTime() to find the first node at/after current_time
 * Should return total_time_used/total_time 
 * total_time = finish time of last node - arrival time of first node at/after current time
 * total_time_used is the total time after current_time where an item is in service 
 */
float Utilization(struct Queue *q, uint64 current_time) {
    struct QueueNode *node = q->tail;
    int total_node = CountNodesAfterTime(q,current_time);
    if(node == NULL || total_node == 0){return 0;}

    uint64 queuing_delay = 0;
    uint64 server_busy_until = 0;

    struct QueueNode *ptr = q->head;
    while(ptr != node){
        if(ptr->arrival_time < server_busy_until){ queuing_delay = server_busy_until - ptr->arrival_time;}
        else{queuing_delay = 0;}
        server_busy_until = ptr->arrival_time + (queuing_delay + q->service_time);
        ptr = ptr->next;

    }
    

    if(node->arrival_time < server_busy_until){
        queuing_delay = server_busy_until - node->arrival_time;
    }
    else{queuing_delay = 0;}
    server_busy_until = node->arrival_time + (queuing_delay + q->service_time);

    float total_time = server_busy_until -  FindNodeAfterTime(q,current_time)->arrival_time;
    float total_time_used = total_node * q->service_time;
    return total_time_used/total_time;
}


