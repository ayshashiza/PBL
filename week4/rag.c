#include <stdio.h>
#include "rag.h"
void print_rag(SystemState *s){
    printf("\nRESOURCE ALLOCATION GRAPH EDGES\n");
    printf("Allocation edge: Resource -> Process; Request edge: Process -> Resource\n");
    int any=0;
    for(int i=0;i<s->p;i++)if(s->active[i])for(int j=0;j<s->r;j++){
        if(s->allocation[i][j]>0){printf("  %s -> %s (allocation %d)\n",s->resource[j],s->process[i],s->allocation[i][j]);any=1;}
        if(s->request[i][j]>0){printf("  %s -> %s (request %d)\n",s->process[i],s->resource[j],s->request[i][j]);any=1;}
    }
    if(!any)printf("  No allocation/request edges.\n");
}
