#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#include "graph.h"
#include "heap.h"
#include "stack.h"

#define BUFFER_LENGTH 256
#define STRING_LENGTH 32
#define DELIM ","

void dijkstra(Heap *pHeap,Node *start, Node *finish){
    heap_decrease_distance(pHeap, start, 0, NULL);

    Node *u = NULL;
    while(!heap_is_empty(pHeap)){
        u = heap_extract_min(pHeap);

        if(node_get_distance(u) == UINT_MAX) {
            break;
        }

        if(u == finish) {
            break;
        }

        struct edge *neighbors = node_get_edges(u);
        for(unsigned int i = 0; i < node_get_n_outgoing(u); i++){
            unsigned int alt = node_get_distance(u) + neighbors[i].mindelay;
            if(alt < node_get_distance(neighbors[i].destination)) {
                heap_decrease_distance(pHeap,neighbors[i].destination,alt,u);
            }
        }

    }
}

bool make_path(Node *start, Node *finish, lifo_stack *pStack){

    Node *node_path = finish;
    while(node_get_previous(node_path) != NULL){
        push(pStack, node_path);
        node_path = node_get_previous(node_path);
    }
    if(isEmpty(pStack)){
        fprintf(stderr,"Path doensn't exist\n");
        return false;
    }
    push(pStack, start);
    return true;
}

void generate_stream(FILE *stream, lifo_stack *pStack){
    Node *from = pop(pStack);
    Node *to = NULL;
    unsigned int mindelay = 0;

    fprintf(stream,"digraph {\n");
    while(!isEmpty(pStack)){
        to = pop(pStack);
        for(unsigned int i = 0; i < node_get_n_outgoing(from); i++){
            struct edge *path_edges = node_get_edges(from);
            if(path_edges[i].destination == to){
                mindelay = path_edges[i].mindelay;
                break;
            }
        }
        fprintf(stream,"\t%u -> %u [label=%u];\n", node_get_id(from),node_get_id(to),mindelay);
        if(isEmpty(pStack)){
           break;
        }
        from = to;
    }
    fprintf(stream,"}\n");
}

bool write_stream(const char *input,lifo_stack *pStack){
    if(input == NULL){
        generate_stream(stdout,pStack);
        return true;
    }
    //release this!!
    FILE *pFileWrite = fopen(input,"a+");
    if(pFileWrite == NULL){
        perror("Error creating file\n");
        return false;
    }

    generate_stream(pFileWrite,pStack);

    fclose(pFileWrite);
    return true;
}

bool read_file(const char *input, Graph *pGraph, bool edges, bool nodes){
    //realeas this
    FILE *pFileRead = fopen(input,"r");

    if(pFileRead == NULL){
        perror("Error opening file\n");
        return false;
    }

    char buffer[BUFFER_LENGTH] = {0};

    unsigned int node_id = 0;
    unsigned int node_to = 0;
    unsigned int node_from = 0;
    unsigned int mindelay = 0;

    while(!feof(pFileRead)){
        if(fgets(buffer, BUFFER_LENGTH, pFileRead) != NULL){
            if(nodes){
                char *pChar = strtok(buffer, DELIM);
                node_id = atoi(pChar);
                if(!graph_insert_node(pGraph, node_id)){
                    fprintf(stderr,"Ran out of memory\n");
                    fclose(pFileRead);
                    return false;
                }
            }
            if(edges){
                //get source node
                char *pChar = strtok(buffer, DELIM);
                node_from = atoi(pChar);

                pChar = strtok(NULL, DELIM);
                node_to = atoi(pChar);

                //skip
                pChar = strtok (NULL, DELIM);

                //get mintime
                pChar = strtok (NULL, DELIM);
                mindelay = atoi(pChar);

                if(!graph_insert_edge(pGraph, node_from, node_to, mindelay)){
                    fprintf(stderr,"Ran out of memory\n");
                    fclose(pFileRead);
                    return false;
                }
            }
        }
    }

    fclose(pFileRead);

    return true;
}

int main(int argc, char* argv[]){
    if((argc < 5) || (argc > 6)){
        fprintf(stderr,"Wrong number %d of parameters!\n", argc);
        return 1;
    }

    //READ NODES AND EDGES
    //realeas this
    Graph *my_graph = graph_new();

    if (!read_file(argv[1],my_graph,false,true)) {
        graph_free(my_graph);
        return 1;
    }
    if (!read_file(argv[2],my_graph,true,false)) {
        graph_free(my_graph);
        return 1;
    }

    //DIJKSTRA
    //realeas this
    Heap *my_heap = heap_new_from_graph(my_graph);
    lifo_stack *my_stack = init_stack();
    Node *start = graph_get_node(my_graph,atoi(argv[3]));
    Node *finish = graph_get_node(my_graph,atoi(argv[4]));

    if(start == NULL){
        fprintf(stderr,"Starting node doesn't exist\n");
        heap_free(my_heap);
        graph_free(my_graph);
        return 1;
    }

    if(finish == NULL){
        fprintf(stderr,"End node doesn't exist\n");
        heap_free(my_heap);
        graph_free(my_graph);
        return 1;
    }

    if(start != finish){
        dijkstra(my_heap, start, finish);

        if(!make_path(start,finish,my_stack)){
            stack_free(my_stack);
            heap_free(my_heap);
            graph_free(my_graph);
            return 1;
        }
    }

    //WRITE OUTPUT
    if (argc == 5){
        write_stream(NULL, my_stack);
    } else {
        if(!write_stream(argv[5],my_stack)){
            stack_free(my_stack);
            heap_free(my_heap);
            graph_free(my_graph);
            return 1;
        }
    }

    //free section
    stack_free(my_stack);
    heap_free(my_heap);
    graph_free(my_graph);
    return 0;
}

