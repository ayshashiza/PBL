#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "state.h"
#include "bankers.h"
#include "detection.h"
#include "rag.h"

static int read_int(const char *prompt,int min,int max){
    char line[128],*end; long v;
    for(;;){printf("%s",prompt);if(!fgets(line,sizeof line,stdin))return min;v=strtol(line,&end,10);
        while(*end==' '||*end=='\t'||*end=='\n')end++;
        if(end!=line&&*end=='\0'&&v>=min&&v<=max)return (int)v;
        printf("Enter a number from %d to %d.\n",min,max);
    }
}
static void print_seq(SystemState *s,int seq[MAX_P],int len){for(int i=0;i<len;i++)printf("%s%s",s->process[seq[i]],i==len-1?"\n":" -> ");}
int main(void){
    SystemState s;init_sample(&s);
    char msg[1024];
    for(;;){
        printf("\n=== DEADLOCK SIMULATION SYSTEM (C VERSION) ===\n");
        printf("1. System matrices\n2. Run Banker's Algorithm\n3. Request resources\n4. Detect deadlock\n5. Recover (terminate process)\n6. Show Resource Allocation Graph edges\n7. Reset sample\n0. Exit\n");
        int choice=read_int("Choice: ",0,7);
        if(choice==0)break;
        if(choice==1){print_matrices(&s);}
        else if(choice==2){
            int seq[MAX_P],len=0;
            if(!validate(&s,msg,sizeof msg)){printf("Invalid state: %s\n",msg);continue;}
            if(safe_sequence(&s,seq,&len)){printf("STATE: SAFE\nSafe sequence: ");print_seq(&s,seq,len);}
            else printf("STATE: UNSAFE; no complete safe sequence exists.\n");
        } else if(choice==3){
            printf("Processes: ");for(int i=0;i<s.p;i++)if(s.active[i])printf("%d=%s  ",i,s.process[i]);printf("\n");
            int pi=read_int("Process index: ",0,s.p-1),req[MAX_R];
            for(int j=0;j<s.r;j++){char prompt[80];snprintf(prompt,sizeof prompt,"Request quantity for %s: ",s.resource[j]);req[j]=read_int(prompt,0,100000);}
            request_resources(&s,pi,req,msg,sizeof msg);printf("%s\n",msg);
        } else if(choice==4){
            int blocked[MAX_P],count=detect_deadlock(&s,blocked);
            if(!count)printf("No deadlocked/unfinished processes identified by the current request matrix.\n");
            else {printf("Processes left unfinished: ");for(int k=0;k<count;k++)printf("%s%s",s.process[blocked[k]],k==count-1?"\n":", ");}
        } else if(choice==5){
            printf("Active processes: ");for(int i=0;i<s.p;i++)if(s.active[i])printf("%d=%s  ",i,s.process[i]);printf("\n");
            int pi=read_int("Process index to terminate: ",0,s.p-1);terminate_process(&s,pi,msg,sizeof msg);printf("%s\n",msg);
        } else if(choice==6) print_rag(&s);
        else if(choice==7){init_sample(&s);printf("Sample restored.\n");}
    }
    printf("Goodbye.\n");return 0;
}

