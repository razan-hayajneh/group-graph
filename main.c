#include <stdio.h>
#include <string.h>
#include "Input_reader.h"
#include "graph_groups.h"
#include "omp_graph_groups.h"

void test(int argc, char **argv, int *numberofthread)
{
   *numberofthread = atoi(argv[1]);
}

int main(int argc, char **argv)
{
   clock_t start, end;
   
   double time_cpu;
   int no_of_nodes, edge_list_size, source, num_omp_threads;
   struct Node *graph_nodes;
   int *graph_edges;
   int *group;

   Read_Input(argc, argv, &graph_edges, &graph_nodes, &no_of_nodes, &edge_list_size, &source, &num_omp_threads);
   start = clock();
   Get_Graph_groups(&group, graph_edges, graph_nodes, no_of_nodes, edge_list_size, num_omp_threads);
   end = clock();
   time_cpu = (double)(end - start) / (CLOCKS_PER_SEC);
   printf("time performance with serial code:%f\n", time_cpu); 
   Get_Graph_groups_with_Parallel_Code(&group, graph_edges, graph_nodes, no_of_nodes, edge_list_size, num_omp_threads);
   return 0;
}
