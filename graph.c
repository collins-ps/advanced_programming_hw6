#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <time.h>
#include <math.h>
#include "graph.h"

#define num_vertices 101

void print_graph(Graph *g);
void read_graph(char *filename, Graph *g, bool directed);
void destroyGraph(Graph *g);
Graph *transpose_graph(Graph *g_original, Graph *g);
void dfs(Graph *g, int v, bool *processed, int *levels, int curr_level);
int kosaraju(Graph *g);
int gcd(int a, int b);
int findGCD(int arr[], int n); // referenced https://www.geeksforgeeks.org/gcd-two-array-numbers/
int find_period(Graph *g);
double **generateMatrix(Graph *g);
void destroyMatrix(double **matrix);
void printMatrix(double **matrix);
double **generateCDF(double **pdf);
void markovChain(int x, double **pdf, int nsteps, double *visits);
double *findProbabilities(Graph *g, int num_iters, int sample_size);

int main(int argc, char **argv){
    int num_iters; 
    int sample_size;
    int checks;
    if (argc > 3){
      num_iters = atoi(argv[1]);
      sample_size = atoi(argv[2]);
      checks = atoi(argv[3]);
    }
    else{
      num_iters = 10; 
      sample_size = 10000000;
      checks = 0;
    }

    srand(time(NULL)); 
    Graph g;
    read_graph("markov-graph.txt",&g,true); 

    if (kosaraju(&g) == 1)
      printf("Graph is irreducible.\n");
    printf("Period is: %d\n", find_period(&g));

    double *results = findProbabilities(&g,num_iters, sample_size);
    double sum = 0;
    printf("Given %d iterations and %d steps, estimated stationary probabilities (5dp):\n", num_iters, sample_size);
    for (int i = 1; i < g.nvertices; i++){
      printf("%.5f ", results[i]);
      sum += results[i];
    }
    printf("\n%.3f\n", sum);
    
    if (checks > 0){
        double perc_diff = 0;
        for (int i = 1; i <= checks; i++){
          double *check = findProbabilities(&g,num_iters, sample_size);
          for (int j = 1; j < g.nvertices; j++){
            perc_diff += fabs( (results[j] - check[j]) / results[j] ) * 100;
          }
          free(check);
        }
        double ave_perc_diff = perc_diff / ( (g.nvertices-1) * checks );
        printf("Average percentage difference: %.3f\n", ave_perc_diff);
    }
    
    free(results);
    destroyGraph(&g);
    return 0;
}

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
    g->nvertices = num_vertices;                      
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
    double w = e->weight;
    if (g->edges[u] == NULL) {       /* if first edge foor vertex u */
      g->edges[u] = new_node;
      g->edges[u]->y = i;
      g->edges[u]->weight = w;
      g->edges[u]->next = NULL;
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
    }
      e = e->next;
    }
  }
  return g;
}

void dfs(Graph *g, int v, bool *processed, int *levels, int curr_level){
    processed[v] = true;
    levels[v] = curr_level++;
    Edgenode *edge = g->edges[v];
    while (edge != NULL){
      if (processed[edge->y] == false){
        dfs(g, edge->y, processed, levels, curr_level);
      }
      edge = edge->next;
    }
}

int kosaraju(Graph *g){
    bool *processed = malloc((g->nvertices)*sizeof(bool));
    int levels[g->nvertices];
    for (int i=1; i < g->nvertices; i++){
      processed[i] = false;
    }
    dfs(g, 1, processed, levels, 1);
    for (int i=1; i < g->nvertices; i++){
      if( processed[i] == 0)
        return 0;
    }
    for (int i=1; i < g->nvertices; i++){
      processed[i] = false;
    }
    Graph g_transpose;
    transpose_graph(g, &g_transpose);
    dfs(&g_transpose, 1, processed, levels, 1);
    for (int i=1; i < g->nvertices; i++){
      if( processed[i] == 0)
        return 0;
    }
    free(processed);
    destroyGraph(&g_transpose);
    return 1;
}

int gcd(int a, int b){
    if (a == 0)
        return b;
    return gcd(b%a, a);
}

int findGCD(int arr[], int n) // referenced https://www.geeksforgeeks.org/gcd-two-array-numbers/
{
    int result = arr[0];
    for (int i = 1; i < n; i++)
    {
        result = gcd(arr[i], result);
 
        if(result == 1)
        {
           return 1;
        }
    }
    return abs(result);
}

int find_period(Graph *g){
    bool *processed = malloc((g->nvertices)*sizeof(bool));
    int *levels = malloc((g->nvertices)*sizeof(int));
    for (int i=1; i < g->nvertices; i++){
      processed[i] = false;
      levels[i] = 0;
    }
    dfs(g, 1, processed, levels, 1);
    int k_set[g->nedges];
    int j = 0;
    for (int i = 0; i < g->nvertices; i++){ // outgoing 
      Edgenode *edge = g->edges[i];
      while (edge != NULL){ // ingoing
        k_set[j] =  levels[edge->y] - levels[i] - 1;
        j++;
        edge = edge->next;
      }
    }
    int gcd = findGCD(k_set,g->nedges);
    free(processed);
    free(levels);
    return gcd;
}

double **generateMatrix(Graph *g){
  double **matrix = malloc( (g->nvertices) * sizeof(double *));
  for (int i = 1; i < g->nvertices; i++){
    matrix[i] = malloc(g->nvertices * sizeof(double));
    Edgenode *edge =  g->edges[i];
    for (int j = 1; j < g->nvertices; j++) {
      if (edge == NULL || edge->y != j){
        matrix[i][j] = 0;
      }
      else{
        matrix[i][edge->y] = edge->weight;
        edge = edge->next;
      }
    }
  }
  return matrix;
}

void destroyMatrix(double **matrix){
  for (int i = 1; i < num_vertices; i++){
    free(matrix[i]);
  }
  free(matrix);
}

void printMatrix(double **matrix){
  for (int i = 1; i < num_vertices; i++){
    for (int j = 1; j < num_vertices; j++) {
      printf("%.9f ", matrix[i][j]);
    }
    printf("\n");
  }
}

double **generateCDF(double **pdf){
  double **matrix = malloc( (num_vertices) * sizeof(double *));
  for (int i = 1; i < num_vertices; i++){
    matrix[i] = malloc(num_vertices * sizeof(double));
    matrix[i][1] = pdf[i][1];
    for (int j = 2; j < 100; j++){
      matrix[i][j] = pdf[i][j] + matrix[i][j-1];
    }
    matrix[i][100] = 1; // to renormalize matrix due to rounding errors
  }
  return(matrix);
}

void markovChain(int x, double **pdf, int nsteps, double *visits){
  double **cdf = generateCDF(pdf);
  for (int i = 1; i <= nsteps; i++){
    double rn = (double)rand() / (double)RAND_MAX;
    int j = 1;
    double cutoff = cdf[x][j];
    while (rn > cutoff){
      j++;
      cutoff = cdf[x][j];
    }
    x = j;
    visits[j]++;
  }
  destroyMatrix(cdf);
}

double *findProbabilities(Graph *g, int num_iters, int sample_size){
  double **pdf = generateMatrix(g);
  double **sampling_distribution = malloc(sizeof(double*) * (num_iters+1));
  for (int i = 1; i <= num_iters; i++){
    sampling_distribution[i] = malloc(sizeof(double) * g->nvertices);
    for (int j = 1; j < g->nvertices; j++){
      sampling_distribution[i][j] = 0.;
    }
  }

  for (int i = 1; i <= num_iters; i++){
    int r = rand() % g->nvertices;
    markovChain(r,pdf,sample_size, sampling_distribution[i]);
  }

  double *mean_proportion = malloc(g->nvertices * sizeof(double));
  for (int i = 1; i < g->nvertices; i++){
    double total_visits = 0;
    for (int j = 1; j <= num_iters; j++){
      total_visits += sampling_distribution[j][i];
    }
    mean_proportion[i] = total_visits / (num_iters * sample_size);
  }
  
  destroyMatrix(pdf);
  for (int i = 1; i <= num_iters; i++)
    free(sampling_distribution[i]);
  free(sampling_distribution);
  return mean_proportion;
}
