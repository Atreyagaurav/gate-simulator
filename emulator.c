#include <stdio.h>
#include <stdlib.h>
/* #include <limits.h> */
#include <string.h>

#define NOT_GATE 1
#define AND_GATE 2
#define OR_GATE 3
#define UNDEF_GATE 0

/* enum {NOT,AND,OR} GateType; */

typedef struct {
  int gateType;
  int rank;
  int inputs[2];
  int output;
} Gate;

typedef struct {
  int rank;
  int number;
  Gate* gates;
} GateStack;

typedef struct {
  int inputs;
  int outputs;
  int nodes;
  int gates;
  int levels;
  GateStack* stack;
} Circuit;


char* gateTypeName(int gcode){
  switch (gcode){
  case NOT_GATE:
    return "NOT";
  case AND_GATE:
    return "AND";
  case OR_GATE:
    return "OR";
  default:
    return "NA";
  }
}

void addGateInStack(GateStack* gs, Gate* g){
  int i;
  for (i=0;i<gs->number;i++){
    if (g==(gs->gates + i)){
      return ;
    }
  }
  gs->number += 1;
  gs->gates = realloc(gs->gates,(gs->number)*sizeof(Gate));
  *(gs->gates + gs->number-1) = *g;
}

void printGate(Gate g){
  printf("Gate Type: 1\n");
  printf("\tInput nodes:%d,%d\n",g.inputs[0],g.inputs[1]);
  printf("\tOutput node:%d",g.output);
}

int processGate(Gate g, int* inputList){
  switch(g.gateType) {
  case NOT_GATE:
    inputList[g.output] = !inputList[g.inputs[0]];
    break;
  case AND_GATE:
    inputList[g.output] = inputList[g.inputs[0]] & inputList[g.inputs[1]];
    break;
  case OR_GATE:
    inputList[g.output] = inputList[g.inputs[0]] | inputList[g.inputs[1]];
    break;
    }
  return inputList[g.output];
}

Gate parseGateDef(char* str){
  Gate g;
  char gtype[5],nodes[10];
  int i=0;
  int j=0;
  while(str[i]!=':'){
    gtype[i] = str [i];
    i++;
  }
  gtype[i]='\0';
  while(str[i]!='\0'){
    nodes[j++] = str[i++];
  }
  nodes[j]='\0';
  
  if (strcmp(gtype,"AND")==0){
      g.gateType = AND_GATE;
  }else if (strcmp(gtype,"OR")==0){
    g.gateType = OR_GATE;
  }else if (strcmp(gtype,"NOT")==0){
    g.gateType = NOT_GATE;
  }else{
    g.gateType = UNDEF_GATE;
  }
  switch (g.gateType){
  case AND_GATE:
  case OR_GATE:
    sscanf(nodes,"%d-%d->%d",g.inputs,g.inputs+1,&g.output);
    break;
  case NOT_GATE:
    sscanf(nodes,"%d->%d",g.inputs,&g.output);
  }
  return g;
}

int processFile(Circuit* cir, char* infile, char* outfile){
  
  while(1){
    /*TODO*/
  }
}

int max_nd(int a, int b){
  if(a==-1 || b==-1){
    return -1;
  }
  if(a>b){
    return a;
  }else{
    return b;
  }
}

void processGatesRank(int numGates, int numInput, int numNodes, Gate* gates){
  int* nodesRank;
  Gate* g;
  int i,j,n1,n2,rank;
  nodesRank = malloc(numNodes * sizeof(int));
  for(i=0;i<numInput;i++){
    *(nodesRank+i) = 0;
  }
  for(;i<numNodes;i++){
    *(nodesRank+i) = -1;
  }

  for(j=0;j<numGates;j++){
    for(i=0;i<numGates;i+=){
      g = (gates+i);
      n1 = *(g->inputs);
      n2 = *(g->inputs+1);
      rank = max_nd(*(nodesRank+n1),*(nodesRank+n2));
      *(nodesRank+ g->output) = rank;
      if (rank >-1){
	g->rank = rank;
      }
    }
  }
}

void processStack(Circuit* cir,int* nodesValue){
  int i,j;
  for(i=0; i < (cir->levels); i++){
    for(j=0; j< (cir->stack->number);j++){
      processGate(*(cir->stack->gates+j), nodesValue);
    }
  }
}

Circuit* makeCircuit(int gatesNumber, Gate* gates){
  Circuit c;
  int i,j,n;
  n=0;
  for(i=0;i<gatesNumber;i++){
    n = max_nd(n,(gates+i)->rank);
  }
  if (n==-1){
    printf("well well well,\n\tSee who made an error\n");
  }
  
  
  return &c;
}
  
int main(int argc,char* argv[]){
  int *inputList;
  int i,totN,inN,outN,gateN;
  char filename[20],line[20];
  Gate *gates;
  GateStack *gstack;
  char c;

  FILE * fp;
  if (argc>1){
    strcpy(filename,argv[1]);
  }else{
    printf("Enter filename:");
    scanf("%s",filename);
  }

  fp = fopen(filename,"r");
  fscanf(fp,"nodes:%d\n",&totN);
  fscanf(fp,"input:%d\n",&inN);
  fscanf(fp,"output:%d\n",&outN);
  fscanf(fp,"gates:%d\n",&gateN);

  inputList = malloc(totN * sizeof(int));

  for(i=0;i<totN;i++){
    inputList[i]=0;
  }
  gates = malloc(gateN * sizeof(Gate));
  printf("%d,%d,%d,%d\n",totN,inN,outN,gateN);

  for(i=0;i<gateN;i++){
    fscanf(fp,"%s\n",line);
    *(gates+i) = parseGateDef(line);
    printf("parsing %d gates\r",i+1);
  }
  printf("\n");
  fclose(fp);

  fp = fopen("simple.inc","r");
  for(i=0;i<inN;i++){
    fscanf(fp,"%c",&c);
    *(inputList+i) = c-'0';
  }
  fclose(fp);

  gstack = malloc(sizeof(GateStack));
  gstack->rank = 0;
  gstack->number=1;
  gstack->gates = gates;
  
  
  return 0;
}

