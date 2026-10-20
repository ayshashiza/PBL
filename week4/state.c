#include <stdio.h>
#include <string.h>
#include "state.h"

void init_sample(SystemState *s) {
    int a[5][3]={{0,1,0},{2,0,0},{3,0,2},{2,1,1},{0,0,2}};
    int m[5][3]={{7,5,3},{3,2,2},{9,0,2},{2,2,2},{4,3,3}};
    memset(s,0,sizeof(*s)); s->p=5; s->r=3;
    strcpy(s->resource[0],"A"); strcpy(s->resource[1],"B"); strcpy(s->resource[2],"C");
    s->total[0]=10; s->total[1]=5; s->total[2]=7;
    for(int i=0;i<s->p;i++) {
        sprintf(s->process[i],"P%d",i); s->active[i]=1;
        for(int j=0;j<s->r;j++){s->allocation[i][j]=a[i][j];s->maximum[i][j]=m[i][j];}
    }
}
int available(SystemState *s, int out[MAX_R]) {
    for(int j=0;j<s->r;j++){
        int used=0; for(int i=0;i<s->p;i++) if(s->active[i]) used+=s->allocation[i][j];
        out[j]=s->total[j]-used;
        if(out[j]<0) return 0;
    } return 1;
}
int validate(SystemState *s, char *msg, int n) {
    int av[MAX_R];
    if(s->p<1||s->p>MAX_P||s->r<1||s->r>MAX_R){snprintf(msg,n,"Invalid process/resource count.");return 0;}
    for(int j=0;j<s->r;j++) if(s->total[j]<0){snprintf(msg,n,"Negative total resource count.");return 0;}
    for(int i=0;i<s->p;i++) for(int j=0;j<s->r;j++){
        if(s->allocation[i][j]<0||s->maximum[i][j]<0||s->request[i][j]<0){snprintf(msg,n,"Matrix values must be non-negative.");return 0;}
        if(s->allocation[i][j]>s->maximum[i][j]){snprintf(msg,n,"%s allocation exceeds maximum claim.",s->process[i]);return 0;}
    }
    if(!available(s,av)){snprintf(msg,n,"Allocated resources exceed total inventory.");return 0;}
    msg[0]='\0'; return 1;
}
void print_matrices(SystemState *s) {
    int av[MAX_R]; available(s,av);
    printf("\nTOTAL: "); for(int j=0;j<s->r;j++) printf("%s=%d ",s->resource[j],s->total[j]);
    printf("\nAVAILABLE: "); for(int j=0;j<s->r;j++) printf("%s=%d ",s->resource[j],av[j]);
    printf("\n\nALLOCATION MATRIX\nProcess");for(int j=0;j<s->r;j++)printf("\t%s",s->resource[j]);printf("\n");
    for(int i=0;i<s->p;i++){printf("%s",s->process[i]);for(int j=0;j<s->r;j++)printf("\t%d",s->allocation[i][j]);printf("%s\n",s->active[i]?"":" (terminated)");}
    printf("\nMAXIMUM MATRIX\nProcess");for(int j=0;j<s->r;j++)printf("\t%s",s->resource[j]);printf("\n");
    for(int i=0;i<s->p;i++){printf("%s",s->process[i]);for(int j=0;j<s->r;j++)printf("\t%d",s->maximum[i][j]);printf("\n");}
    printf("\nNEED MATRIX (Maximum - Allocation)\nProcess");for(int j=0;j<s->r;j++)printf("\t%s",s->resource[j]);printf("\n");
    for(int i=0;i<s->p;i++){printf("%s",s->process[i]);for(int j=0;j<s->r;j++)printf("\t%d",s->active[i]?s->maximum[i][j]-s->allocation[i][j]:0);printf("\n");}
    printf("\nOUTSTANDING REQUEST MATRIX\nProcess");for(int j=0;j<s->r;j++)printf("\t%s",s->resource[j]);printf("\n");
    for(int i=0;i<s->p;i++){printf("%s",s->process[i]);for(int j=0;j<s->r;j++)printf("\t%d",s->request[i][j]);printf("\n");}
}
