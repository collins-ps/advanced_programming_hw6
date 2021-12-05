#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include <string.h>
#include "graph.h"

void print_graph(Graph *g){
  Edgenode *e = NULL;
  for (int i=1;i<=g->nvertices;++i){              /* for each node 0..NV-1 */
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
  g->nvertices = 100;                      
  g->directed = true;
  g->edges = malloc((g->nvertices)*sizeof(Edgenode));
  for (int i=0;i<g->nvertices;++i)
    g->edges[i] = NULL;
  
  Edgenode *prev;
    // while ((read = getline(&line, &len, file)) != -1){
    //     int u = atoi ( strtok(line, " ") );
    //     char *str = " ";
    //     while (str != NULL && str[0] != '\n'){ 
    //         str = strtok(NULL, "[");
    //         int v = atoi ( strtok(NULL, ", ") );
    //         double w = atof ( strtok(NULL, "]") );

    //         Edgenode *new_node = (Edgenode *) malloc(sizeof(Edgenode));

    //         if (g->edges[u] == NULL) {       /* if first edge foor vertex u */
    //         g->edges[u] = new_node;
    //         g->edges[u]->y = v;
    //         g->edges[u]->weight = w;
    //         g->edges[u]->next = NULL;
    //         prev = new_node;
    //         } else {                         /* adding another edge to vertex u */
    //         prev->next = new_node;
    //         new_node->y = v;
    //         new_node->weight = w;
    //         new_node->next = NULL;
    //         prev = new_node;
    //         }
    //         num_edges++;
    //         str = strtok(NULL, " ");
    //     }

    // }
    while ((read = getline(&line, &len, file)) != -1){
        char *str = strtok(line, " ");
        int u = atoi ( str );
        // printf("%d ",u);
        
        str = strtok(NULL, "[");
        str = strtok(NULL, ", ");

        while (str != NULL && str != EOF && str[0] != '\n' ){ 
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
            // str = strtok(NULL, "[");
        }

    }
    g->nedges = num_edges;
  fclose(file);

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

Graph *transpose_graph(Graph *g_original){
  FILE *file = fopen("transpose.txt","w+");
  fprintf(file, "%d %d\n", g_original->nvertices, g_original->nedges);
  Edgenode *e = NULL;
  for (int i=1;i<=g_original->nvertices;++i){              /* for each node 0..NV-1 */
    e = g_original->edges[i];                             /* e=pointer to root of node i's edge list */
    while (e != NULL) {                          /* traverse list */
      fprintf(file, "%d %d %.9f \n", e->y, i, e->weight); /* print edge and weight */
      e = e->next;
    }
  }
  // fclose(file);
  Graph *g;
  int nvertices, nedges;
  int u,v,w;
  // FILE *file = NULL;
  // file = fopen(filename,"r");
  fscanf(file,"%d %d", &nvertices, &nedges);     /* read header */
  //  printf("NV = %d    NE = %d\n",nvertices,nedges);
  

  g->nvertices = nvertices;                      /* initialize Graph fields */
  // printf("%d\n", nvertices);
  g->nedges = nedges;
  g->directed = true;
  g->edges = malloc((nvertices)*sizeof(Edgenode));
  
  for (int i=0;i<nvertices;++i)
    g->edges[i] = NULL;
  
  Edgenode *prev;
  while (fscanf(file, "%d %d %d\n", &u,&v,&w) != EOF) {
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
  }
  fclose(file);
  return g;
}

// int kosaraju(Graph *g){

// }

int main(){
    Graph g;
    read_graph("markov-graph.txt",&g,true); 
    // print_graph(&g);
    // Graph *transpose = transpose_graph(&g);
    // print_graph(transpose);
    destroyGraph(&g);
    // destroyGraph(transpose);
    return 0;
}

