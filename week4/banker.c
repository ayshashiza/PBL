#include <stdio.h>
#include <string.h>
#include "bankers.h"
int safe_sequence(SystemState *s, int seq[MAX_P], int *seq_len) {
    int work[MAX_R], finish[MAX_P]={0}; *seq_len=0; available(s,work);
    for(int i=0;i<s->p;i++) finish[i]=!s->active[i];
    int progress=1;
    while(progress){
        progress=0;
        for(int i=0;i<s->p;i++) if(s->active[i]&&!finish[i]){
            int ok=1;
            for(int j=0;j<s->r;j++) if(s->maximum[i][j]-s->allocation[i][j]>work[j]){ok=0;break;}
            if(ok){for(int j=0;j<s->r;j++)work[j]+=s->allocation[i][j];finish[i]=1;seq[(*seq_len)++]=i;progress=1;}
        }
    }
    for(int i=0;i<s->p;i++) if(!finish[i]) return 0;
    return 1;
}
int request_resources(SystemState *s,int pi,int req[MAX_R],char *msg,int n){
    if(pi<0||pi>=s->p){snprintf(msg,n,"Unknown process.");return 0;}
    if(!s->active[pi]){snprintf(msg,n,"Process has been terminated.");return 0;}
    for(int j=0;j<s->r;j++){
        if(req[j]<0){snprintf(msg,n,"Request values must be non-negative.");return 0;}
        if(req[j]>s->maximum[pi][j]-s->allocation[pi][j]){snprintf(msg,n,"Request exceeds remaining Need.");return 0;}
    }
    int av[MAX_R];available(s,av);
    for(int j=0;j<s->r;j++) if(req[j]>av[j]){
        for(int k=0;k<s->r;k++)s->request[pi][k]=req[k];
        snprintf(msg,n,"Insufficient Available resources. Request recorded as outstanding.");return 0;
    }
    for(int j=0;j<s->r;j++)s->allocation[pi][j]+=req[j];
    int seq[MAX_P],len=0;
    if(safe_sequence(s,seq,&len)){
        for(int j=0;j<s->r;j++)s->request[pi][j]=0;
        snprintf(msg,n,"Request granted; system remains safe. Sequence: ");
        for(int k=0;k<len;k++){char part[24];snprintf(part,sizeof(part),"%s%s",s->process[seq[k]],k==len-1?"":" -> ");strncat(msg,part,n-strlen(msg)-1);}
        return 1;
    }
    for(int j=0;j<s->r;j++){s->allocation[pi][j]-=req[j];s->request[pi][j]=req[j];}
    snprintf(msg,n,"Denied: request would leave the system unsafe; recorded as outstanding.");return 0;
}
