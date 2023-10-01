#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

void Get_Graph_groups(int **group, int *graph_edges, struct Node *graph_nodes, int no_of_nodes, int edge_list_size, int num_omp_threads)
{

    *group = (int *)malloc(no_of_nodes * sizeof(int));
    // Initialize all elements of the group array to -1
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

        // Pick a node with no group ID at random
        for (unsigned int i = 0; i < no_of_nodes; i++)
        {
            if ((*group)[i] == -1)
            {
                done = false;
                source = i;
                countgroup = countgroup + 1;
                break;
            }
        }

        // If all nodes are already grouped, exit the loop
        if (source == -1)
        {
            break;
        }

        // Mark the source node as its own group
        (*group)[source] = source;

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
