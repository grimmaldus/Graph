/*
PROOF OF CONCEPT!!!
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#include "graph.h"
#include "heap.h"

#define BUFFER_LENGTH 256
#define STRING_LENGTH 32
#define DELIM ","

typedef struct _stack {
    Node *data;
    struct _stack *next;
}stack;

stack *top = NULL;

void push(Node *info)
{
    stack *temp;
    temp=(stack *)malloc(sizeof(stack));
    if( temp == NULL) {
        fprintf(stderr,"Memory overflow!");
        exit(EXIT_FAILURE);
    }
    temp->data=info;
    temp->next=top;
    top=temp;
    stack_size++;
}

Node *pop()
{
    Node *info = NULL;
    stack *t;
    if( top ==  NULL) {
        return NULL;
    }
    else
    {
        t=top;
        info=top->data;
        top=top->next;
        t->next=NULL;
        free(t);
        return info;
    }
}

bool isEmpty(){
    return top == NULL;
}

int main(int argc, char* argv[])
{
    //INIT GRAPH
    //REALEASE THIS !!
    Graph *my_graph = graph_new();

    //READ NODES
    FILE *pFileRead = fopen(argv[1],"r");

    if(pFileRead == NULL){
        perror("Error opening file");
        free(my_graph);
        return 1;
    }

    char buffer[BUFFER_LENGTH] = {0};
    char node_id[STRING_LENGTH] = {0};

    while(!feof(pFileRead)){
        if(fgets(buffer, BUFFER_LENGTH, pFileRead) != NULL){
            printf("%s", buffer);
            char *pChar = strtok(buffer, DELIM);
            strcpy(node_id, pChar);
            graph_insert_node(my_graph, atoi(node_id));
            while(pChar != NULL){
                pChar = strtok (NULL, DELIM);
            }
        }
    }

    fclose(pFileRead);

    //READ EDGES
    pFileRead = fopen(argv[2],"r");

    if(pFileRead == NULL){
        perror("Error opening file");
        return 1;
    }

    char node_from[STRING_LENGTH] = {0};
    char node_to[STRING_LENGTH] = {0};
    char edge_value[STRING_LENGTH] ={0};

    while(!feof(pFileRead)){
        if(fgets(buffer, BUFFER_LENGTH, pFileRead) != NULL){
            printf("%s", buffer);
            char *pChar = strtok(buffer, DELIM);
            strcpy(node_from, pChar);
            pChar = strtok(NULL, DELIM);
            strcpy(node_to,pChar);
            pChar = strtok (NULL, DELIM);
            pChar = strtok (NULL, DELIM);
            strcpy(edge_value,pChar);
            while(pChar != NULL){
                pChar = strtok (NULL, DELIM);
            }
            graph_insert_edge(my_graph, atoi(node_from),atoi(node_to), atoi(edge_value));
        }
    }

    fclose(pFileRead);

    //DIJKSTRA ALGO
    Heap *my_heap = heap_new_from_graph(my_graph);
    Node *start = graph_get_node(my_graph,atoi(argv[3]));
    Node *finish = graph_get_node(my_graph,atoi(argv[4]));

    heap_decrease_distance(my_heap,start, 0, NULL);

    Node *u = NULL;
    while(!heap_is_empty(my_heap)){
        u = heap_extract_min(my_heap);

        if(node_get_distance(u) == UINT_MAX) {
            break;
        }

        printf("Examining %d\n",node_get_id(u));
        if(u == finish) {
            printf("Found\n");
            break;
        }
        printf("Not found yet!\n");
        struct edge *neighbors = node_get_edges(u);
        printf("This node got %d neighbors\n ", node_get_n_outgoing(u));
        for(unsigned int i = 0; i < node_get_n_outgoing(u); i++){
            printf("\tExaming neighbor %d and the mindealy of the edge is %d\n\n", node_get_id(neighbors[i].destination),neighbors[i].mindelay);
            unsigned int alt = node_get_distance(u) + neighbors[i].mindelay;
            if(alt < node_get_distance(neighbors[i].destination)) {
                heap_decrease_distance(my_heap,neighbors[i].destination,alt,u);
            }

        }

    }

    Node *node_path = NULL;
    node_path = finish;
    while(node_get_previous(node_path) != NULL){
        push(node_path);
        node_path = node_get_previous(node_path);
    }


    //WRITE TO STANDART OUTPUT
    /*
    Node *from = start;
    Node *to = NULL;
    unsigned int mindelay = 0;

    fprintf(stdout,"digraph {\n");
    while(!isEmpty()){
        to = pop();
        for(unsigned int i = 0; i < node_get_n_outgoing(from); i++){
            struct edge *path_edges = node_get_edges(from);
            if(path_edges[i].destination == to){
                mindelay = path_edges[i].mindelay;
                break;
            }
        }
        fprintf(stdout,"\t%u -> %u [label=%u];\n", node_get_id(from),node_get_id(to),mindelay);
        if(isEmpty()){
            break;
        }
        from = to;
    }
    fprintf(stdout,"}\n");
    */

    //WRITE TO FILE
    FILE *pFileWrite;
    pFileWrite = fopen("output.dot","a+");

    Node *wfrom = start;
    Node *wto = NULL;
    unsigned int mindelay = 0;

    fprintf(pFileWrite,"digraph {\n");
    while(!isEmpty()){
        wto = pop();
        for(unsigned int i = 0; i < node_get_n_outgoing(wfrom); i++){
            struct edge *path_edges = node_get_edges(wfrom);
            if(path_edges[i].destination == wto){
                mindelay = path_edges[i].mindelay;
                break;
            }
        }
        fprintf(pFileWrite,"\t%u -> %u [label=%u];\n", node_get_id(wfrom),node_get_id(wto),mindelay);
        if(isEmpty()){
            break;
        }
        wfrom = wto;
    }
    fprintf(pFileWrite,"}\n");

    fclose(pFileWrite);
    //FREE ALL
    free(top);
    heap_free(my_heap);
    graph_free(my_graph);
    return 0;
}

