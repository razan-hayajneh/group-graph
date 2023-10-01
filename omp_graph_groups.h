#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <omp.h>




void Get_Graph_groups_with_Parallel_Code(int **group, int *graph_edges, struct Node *graph_nodes, int no_of_nodes, int edge_list_size, int num_omp_threads)
{

    double start_time = omp_get_wtime();
    *group = (int *)malloc(no_of_nodes * sizeof(int));
    omp_set_num_threads(num_omp_threads);
// Initialize all elements of the group array to -1
#pragma omp paralell for
        for (int i = 0; i < no_of_nodes; i++)
        {
            (*group)[i] = -1;
        }
    bool done = false;
    int source = -1;
    int countgroup = 0;
  
    while (!done)
    {
        done = true;
        source = -1;
            bool continueFlag = false;
#pragma omp paralell {
    #pragma omp paralell for shared(continueFlag) shared(source) 
            // Pick a node with no group ID at random
            for (unsigned int i = 0; i < no_of_nodes; i++)
            {
                if (!continueFlag && (*group)[i] == -1)
                {
                    done = false;
                if(continueFlag){continue;}
                    continueFlag = true;  
                    source = i;             
                    #pragma omp atomic
                    countgroup = countgroup + 1;

                }

             
            // If all nodes are already grouped, exit the loop
            if (!continueFlag)
            {
                done = true;
            }
            else
            {
                
                continueFlag = false;

               #pragma omp atomic write  
                // Mark the source node as its own group
                (*group)[source] = source;
                

                #pragma omp paralell for
                // Group other nodes that belong to the same group as the source
                for (unsigned int j = graph_nodes[source].starting; j < graph_nodes[source].starting + graph_nodes[source].no_of_edges; j++)
                {
                    int dest = graph_edges[j];
                    if ((*group)[dest] == -1)
                    {
                        (*group)[dest] = source;
                    }
                    else
                    {
                        (*group)[source] = (*group)[dest];
                    }
                }
            }
        }
    }

    double end_time = omp_get_wtime();
    printf("\n Parallel With Atomic directive Code\n");
    double excution_time = end_time - start_time;
    printf("\n time performance:%f\n ", excution_time);
    printf("\n check group number in parallel:%i\n ", countgroup);
}


// lock step 
void Get_Graph_groups_with_lock_Parallel_Code(int **group, int *graph_edges, struct Node *graph_nodes, int no_of_nodes, int edge_list_size, int num_omp_threads)
{

    double start_time = omp_get_wtime();
    *group = (int *)malloc(no_of_nodes * sizeof(int));
    omp_set_num_threads(num_omp_threads);
// Initialize all elements of the group array to -1
#pragma omp paralell for
        for (int i = 0; i < no_of_nodes; i++)
        {
            (*group)[i] = -1;
        }
    bool done = false;
    int source = -1;
    int countgroup = 0;
     omp_lock_t lock;
omp_init_lock(&lock);
    while (!done)
    {
        done = true;
        source = -1;
#pragma omp parallel
        {
            bool continueFlag = false;
#pragma omp paralell for shared(continueFlag) 
            // Pick a node with no group ID at random
            for (unsigned int i = 0; i < no_of_nodes; i++)
            {
                if(continueFlag){continue;}
                  omp_set_lock(&lock);
                if (!continueFlag && (*group)[i] == -1)
                {
                    done = false;
                    source = i;
                    countgroup = countgroup + 1;
                    continueFlag = true;
                }
            // If all nodes are already grouped, exit the loop
            if (!continueFlag)
            {
                done = true;
            }
            else
            {
                continueFlag = false;

                // Mark the source node as its own group
                (*group)[source] = source;

#pragma omp paralell for
                // Group other nodes that belong to the same group as the source
                for (unsigned int j = graph_nodes[source].starting; j < graph_nodes[source].starting + graph_nodes[source].no_of_edges; j++)
                {
                    int dest = graph_edges[j];
                    if ((*group)[dest] == -1)
                    {
                        (*group)[dest] = source;
                    }
                    else
                    {
                        (*group)[source] = (*group)[dest];
                    }
                }
            }
            omp_unset_lock(&lock);

            }

        }
    }
omp_destroy_lock(&lock);
    double end_time = omp_get_wtime();
    printf("\n Parallel With Lock Code\n");
    double excution_time = end_time - start_time;
    printf("\n time performance:%f\n ", excution_time);
     printf("\n group number:%i\n ", countgroup);
}
