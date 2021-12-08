#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <time.h>
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

void dfs(Graph *g, int v, bool *processed, int *levels, int curr_level){
    // printf("%d\n", v);
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
    return result;
}

int find_period(Graph *g){
    bool *processed = malloc((g->nvertices)*sizeof(bool));
    int *levels = malloc((g->nvertices)*sizeof(int));
    for (int i=1; i < g->nvertices; i++){
      processed[i] = false;
      levels[i] = 0;
    }
    dfs(g, 1, processed, levels, 1);
    // for (int i=1; i < g->nvertices; i++){
    //   printf("i: %d, level: %d\n", i, levels[i]);
    // }
    int k_set[g->nedges];
    int j = 0;
    for (int i = 0; i < g->nvertices; i++){ // outgoing 
      Edgenode *edge = g->edges[i];
      while (edge != NULL){ // ingoing
        k_set[j] =  levels[edge->y] - levels[i] - 1;
        // printf("%d ", k_set[j]);
        j++;
        edge = edge->next;
      }
    }
    int gcd = abs(findGCD(k_set,g->nedges));
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
  for (int i = 1; i < 101; i++){
    free(matrix[i]);
  }
  free(matrix);
}

void printMatrix(double **matrix){
  for (int i = 1; i < 101; i++){
    for (int j = 1; j < 101; j++) {
      printf("%.9f ", matrix[i][j]);
    }
    printf("\n");
  }
}

void mulMat(double **mat1, double **mat2, double **rslt) { // referenced https://www.geeksforgeeks.org/c-program-multiply-two-matrices/
    // double **rslt = malloc( (101) * sizeof(double *));
    // for (int i = 1; i < 101; i++)
    //   rslt[i] = malloc(101 * sizeof(double));
 
    for (int i = 1; i < 101; i++) {
        for (int j = 1; j < 101; j++) {
            rslt[i][j] = 0;
 
            for (int k = 1; k < 101; k++) {
                rslt[i][j] += mat1[i][k] * mat2[k][j];
            }
 
            // printf("%d\t", rslt[i][j]);
        }
 
        // printf("\n");
    }
    // return rslt;
}

double **generateCDF(double **pdf){
  double **matrix = malloc( (101) * sizeof(double *));
  for (int i = 1; i < 101; i++){
    matrix[i] = malloc(101 * sizeof(double));
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
  // int visits[101]; 
  // for (int i = 1; i < 101; i++){
  //   visits[i] = 0;
  // }
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
  // for (int i = 1; i < 101; i++){
  //   printf("%.3f ", (double)visits[i] / (double)nsteps );
  // }
}


int main(){
    srand(time(NULL)); 
    Graph g;
    read_graph("markov-graph.txt",&g,true); 
    // if (kosaraju(&g) == 1)
    //     printf("Graph is irreducible.\n");
    // printf("Period is: %d\n", find_period(&g));
    double **matrix = generateMatrix(&g);

    // double **cdf = generateCDF(matrix);
    // printMatrix(cdf);
    // destroyMatrix(cdf);

    double *visits = malloc(sizeof(double) * 101);
    int num_iters = 10; // in millions
    for (int i = 1; i < 101; i++){
      visits[i] = 0;
    }
    for (int i = 1; i <= num_iters; i++){
      int r = rand() % 101;
      markovChain(r,matrix,1000000, visits);
    }
    double sum = 0;
    for (int i = 1; i < 101; i++){
      visits[i] = (double)visits[i] / (double)(num_iters * 1000000);
      printf("%.3f ", visits[i]);
      sum += visits[i];
    }
    assert(sum - 1 < 0.000001);
    
    free(visits);

    // printMatrix(matrix);
    // findProbabilities(matrix, &g);
    destroyMatrix(matrix);
    
    destroyGraph(&g);
    return 0;
}

// double **findProbabilities(double **matrix, Graph *g){
//   double **k_step = malloc( (101) * sizeof(double *));
//   for (int i = 1; i < 101; i++){
//     k_step[i] = malloc(101 * sizeof(double));
//     for (int j = 1; j < 101; j++){
//       k_step[i][j] = matrix[i][j];
//     }
//   }

//   int steps = 10000000;
    
//   for (int i = 1; i <= 10000000; i++){ // 2 step
//     mulMat(k_step, matrix, k_step);
//   }
//   while (k_step[1][1] - k_step[100][1] > 0.00000001){
//     printf("Not equal\n");
//     steps += 1000000;
//     for (int i = 1; i <= 1000000; i++){ // 2 step
//       mulMat(k_step, matrix, k_step);
//     }
//   }

  
//   // while ((k_step[1][5] - k_step[100][5] > 0.000001) ){ 
//   //   steps++;
//   //   mulMat(k_step, matrix, k_step);
//   // }

//   // double **k_step = mulMat(matrix, matrix);
//   printMatrix(k_step, g);
//   printf("\nSteps: %d\n", steps);
//   for (int i = 1; i < 101; i++)
//     free(k_step[i]);
//   free(k_step);
// }
