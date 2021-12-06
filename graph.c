#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include <string.h>
#include <ctype.h>
#include "graph.h"

void print_graph(Graph *g){
  Edgenode *e = NULL;
  for (int i=1;i<g->nvertices;++i){              /* for each node 0..NV-1 */
    e = g->edges[i];                             /* e=pointer to root of node i's edge list */
    while (e != NULL) {                          /* traverse list */
      printf("%d %d %.9f \n", i, e->y, e->weight); /* print edge and weight */
      e = e->next;
    }
  }
}

void read_graph(char *filename, Graph *g, bool directed){
    int num_edges = 0;
    size_t len = 0;
    ssize_t read;
    int u,v,w;
    char buf[len];
    FILE *file = NULL;
    char *line = NULL;
    file = fopen(filename,"r");
    g->nvertices = 101;                      
    g->directed = true;
    g->edges = malloc((g->nvertices)*sizeof(Edgenode));
    for (int i=0;i<g->nvertices;++i)
      g->edges[i] = NULL;

    Edgenode *prev;
    while ((read = getline(&line, &len, file)) != -1){
        char *str = strtok(line, " ");
        if (isdigit(str[0] == 0))
          break;
        int u = atoi ( str );
        
        str = strtok(NULL, "[");
        str = strtok(NULL, ", ");

        while (str != NULL && str[0] != '\n' && str[0] != -1 ){ 
            if (isdigit(str[0] == 0))
              break;
            int v = atoi ( str );
            str = strtok(NULL, "]");
            double w =  strtold(str, NULL);

            Edgenode *new_node = (Edgenode *) malloc(sizeof(Edgenode));

            if (g->edges[u] == NULL) {       /* if first edge foor vertex u */
            g->edges[u] = new_node;
            g->edges[u]->y = v;
            g->edges[u]->weight = w;
            g->edges[u]->next = NULL;
            prev = new_node;
            } else {                         /* adding another edge to vertex u */
            prev->next = new_node;
            new_node->y = v;
            new_node->weight = w;
            new_node->next = NULL;
            prev = new_node;
            }
            num_edges++;
            str = strtok(NULL, " [");
        }

    }
    g->nedges = num_edges;
  fclose(file);
  if (line)
      free(line);

}

void destroyGraph(Graph *g){
  for (int i=0; i<g->nvertices;++i){
    Edgenode *node = g->edges[i];
    while (node != NULL){
      Edgenode *next = node->next;
      free(node);
      node = next;
    }
  }
  free(g->edges);
}

Graph *transpose_graph(Graph *g_original, Graph *g){
  g->nvertices = g_original->nvertices;                      /* initialize Graph fields */
  g->nedges = g_original->nedges;
  g->directed = true;
  g->edges = malloc((g->nvertices)*sizeof(Edgenode));
  
  for (int i=0;i<g->nvertices;++i)
    g->edges[i] = NULL;
  
  Edgenode *e = NULL;
  Edgenode *prev = NULL;
  for (int i = 1; i < g_original->nvertices; i++){              /* for each node 0..NV-1 */
    e = g_original->edges[i];                             /* e=pointer to root of node i's edge list */
    while (e != NULL) {                          /* traverse list */
      Edgenode *new_node = (Edgenode *) malloc(sizeof(Edgenode));
    int u = e->y; 
    // int v = i;
    double w = e->weight;
    if (g->edges[u] == NULL) {       /* if first edge foor vertex u */
      g->edges[u] = new_node;
      g->edges[u]->y = i;
      g->edges[u]->weight = w;
      g->edges[u]->next = NULL;
      // prev = new_node;
    } else {                         /* adding another edge to vertex u */
      Edgenode *crawl = g->edges[u];
      while (crawl != NULL){
        prev = crawl;
        crawl = crawl->next;
      }
      prev->next = new_node;
      new_node->y = i;
      new_node->weight = w;
      new_node->next = NULL;
      // prev = new_node;
    }
      e = e->next;
    }
  }
  return g;
}

// int kosaraju(Graph *g){

// }

int main(){
    Graph g;
    read_graph("markov-graph.txt",&g,true); 
    // print_graph(&g);
    Graph transpose;
    transpose_graph(&g, &transpose);
    print_graph(&transpose);
    destroyGraph(&g);
    destroyGraph(&transpose);
    return 0;
}

