#include <stdio.h>
#include <stdlib.h>
/* #include <limits.h> */
#include <string.h>

#define NOT_GATE 1
#define AND_GATE 2
#define OR_GATE 3
#define NAND_GATE 4
#define NOR_GATE 5
#define XOR_GATE 6
#define XNOR_GATE 7
#define UNDEF_GATE 0
#define DEBUG_LEVEL 0 /*unused*/

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
  Gate** gates;
} GateStack;

typedef struct {
  int inputs;
  int outputs;
  int nodes;
  int gates;
  int levels;
  GateStack *stack;
} Circuit;


char* gateTypeName(int gcode){
  switch (gcode){
  case NOT_GATE:
    return "NOT";
  case AND_GATE:
    return "AND";
  case OR_GATE:
    return "OR";
  case NAND_GATE:
    return "NAND";
  case NOR_GATE:
    return "NOR";
  case XOR_GATE:
    return "XOR";
  case XNOR_GATE:
    return "XNOR";
  default:
    return "NA";
  }
}

void nextLine(FILE* fp){
  char c;
  int flag = 1;
  while (flag){
    c=fgetc(fp);
    switch (c){
    case '\n':
    case ' ':
      continue;
    case '#':
      while(fgetc(fp)!='\n');
      break;
    default:
      flag=0;
    }
  }
  fseek(fp, -1, SEEK_CUR);
}

void addGateInStack(GateStack* gs, Gate* g){
  int i;
  for (i=0;i<gs->number;i++){
    if (g==*(gs->gates + i)){
      return ;
    }
  }
  gs->number += 1;
  gs->gates = realloc(gs->gates,(gs->number)*sizeof(Gate));
  *(gs->gates + gs->number-1) = g;
}

void printGate(Gate* g){
  printf("Gate Type: %s\t",gateTypeName(g->gateType));
  printf("Rank:%d\n",g->rank);
  if(g->gateType == NOT_GATE){
    printf("\tInput node:%d\n",g->inputs[0]);
  }else{
    printf("\tInput nodes:%d,%d\n",g->inputs[0],g->inputs[1]);
  }
  printf("\tOutput node:%d\n",g->output);
}

void printArray(int length,int* array){
  int i;
  printf("[");
  for(i=0;i<length-1;i++){
    printf("%d,",*(array+i));
  }
  if(length>0){
  printf("%d",*(array+length-1));
  }
  printf("]");
}

void printGates(int numGate,Gate* gates){
  int i;
  for(i=0;i<numGate;i++){
    printGate(gates+i);
  }
}

void printCircuit(Circuit* cir){
  int i,j;
  printf("Circuit:: nodes: %d\tLevels:%d\n",cir->nodes,cir->levels);
  for(i=0; i < (cir->levels); i++){
      printf("********Level: %d********\n",i);
    for(j=0; j< ( (cir->stack+i) -> number);j++){
      printGate((cir->stack+i)->gates[j]);
    }
  }
  printf("******** :END: ********\n");
}

int processGate(Gate* g, int* inputList){
  /* printf("processing;;"); */
  /* printGate(g); */
  switch(g->gateType) {
  case NOT_GATE:
    inputList[g->output] = !inputList[g->inputs[0]];
    break;
  case AND_GATE:
    inputList[g->output] = inputList[g->inputs[0]] & inputList[g->inputs[1]];
    break;
  case OR_GATE:
    inputList[g->output] = inputList[g->inputs[0]] | inputList[g->inputs[1]];
    break;
  case NAND_GATE:
    inputList[g->output] = 1 - (inputList[g->inputs[0]] & inputList[g->inputs[1]]);
    break;
  case NOR_GATE:
    inputList[g->output] = 1 - (inputList[g->inputs[0]] | inputList[g->inputs[1]]);
    break;
  case XOR_GATE:
    inputList[g->output] = (inputList[g->inputs[0]] & (1-inputList[g->inputs[1]])) | (inputList[g->inputs[1]] & (1-inputList[g->inputs[0]]));
    break;
  case XNOR_GATE:
    inputList[g->output] = 1 - ((inputList[g->inputs[0]] & (1-inputList[g->inputs[1]])) | (inputList[g->inputs[1]] & (1-inputList[g->inputs[0]])));
    break;
    }
  return inputList[g->output];
}

void parseGateDef(Gate* g,char* str){
  char gtype[5],nodes[10];
  int i=0;
  int j=0;
  while(str[i]==' ' || str[i]=='\t'){
    i++;
  }
  while(str[i]!=':'){
    gtype[j++] = str [i++];
  }
  gtype[i++]='\0';
  j=0;
  while(str[i]!='\0'){
    if(str[i]==' ' || str[i]=='\t'){
      i+=1;
      continue;
    }
    nodes[j++] = str[i++];
  }
  nodes[j]='\0';
  
  if (strcmp(gtype,"AND")==0){
    g->gateType = AND_GATE;
  }else if (strcmp(gtype,"OR")==0){
    g->gateType = OR_GATE;
  }else if (strcmp(gtype,"NOT")==0){
    g->gateType = NOT_GATE;
  }else if (strcmp(gtype,"NAND")==0){
    g->gateType = NAND_GATE;
  }else if (strcmp(gtype,"NOR")==0){
    g->gateType = NOR_GATE;
  }else if (strcmp(gtype,"XOR")==0){
    g->gateType = XOR_GATE;
  }else if (strcmp(gtype,"XNOR")==0){
    g->gateType = XNOR_GATE;
  }else{
    g->gateType = UNDEF_GATE;
    printf("Unrecogniged Gate Type: %s",gtype);
  }
  switch (g->gateType){
  case NOT_GATE:
    sscanf(nodes,"%d->%d",g->inputs,&g->output);
    break;
  default:
    sscanf(nodes,"%d-%d->%d",g->inputs,g->inputs+1,&g->output);
  }
}

int readBooleans(FILE* fp,int nodeLength, int* nodes){
  char c;
  int i;
  for(i=0;i<nodeLength;i++){
    c = fgetc(fp);
    if(c==EOF){
      return 0;
    }
    if (c=='\n'){
      for(;i<nodeLength;i++){
	*(nodes+i)=0;
      }
    }
    *(nodes+i) = c - '0';
  }
  while(c!='\n' && c!=EOF){
    c=fgetc(fp);
  }
  return 1;
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
  int flag = 0;
  nodesRank = malloc(numNodes * sizeof(int));
  for(i=0;i<numInput;i++){
    *(nodesRank+i) = 0;
  }
  for(;i<numNodes;i++){
    *(nodesRank+i) = -1;
  }

  for(j=0;j<numNodes;j++){
    for(i=0;i<numGates;i++){
      g = (gates+i);
      if (g->gateType == NOT_GATE){
	n1 = *(g->inputs);
	rank = *(nodesRank+n1);
      }else{
      n1 = *(g->inputs);
      n2 = *(g->inputs+1);
      rank = max_nd(*(nodesRank+n1),*(nodesRank+n2));
      }
      if (rank >-1){
	*(nodesRank+ g->output) = rank+1;
	g->rank = rank;
      }
    }
    flag = 1;
    for(i=0;i<numNodes;i++){
      if(*(nodesRank+i)==-1){
	flag = 0;
      }
    }
    if(flag){
      break;
    }
  }
  printArray(numNodes, nodesRank);
  printf("\n");
  free(nodesRank);
}

void processCircuit(Circuit* cir,int* nodesValue){
  int i,j;
  for(i=0; i < (cir->levels); i++){
    for(j=0; j< ( (cir->stack+i) -> number);j++){
      processGate((cir->stack+i)->gates[j], nodesValue);
    }
  }
}

Circuit* makeCircuit(int gatesNumber, Gate* gates){
  Circuit* c;
  int i,j,n;
  GateStack* stack; 
  n=0;
  c = malloc(sizeof(Circuit));
  
  for(i=0;i<gatesNumber;i++){
    n = max_nd(n,(gates+i)->rank);
  }
  if (n==-1){
    printf("well well well,\n\tSee who made an error\n");
  }

  n+=1;
  stack = malloc(n*sizeof(GateStack));
  c->levels = n;
  for(i=0;i<n;i++){
    (stack+i)->rank = i;
    (stack+i)->number = 0;
    (stack+i)->gates = NULL;
    for(j=0;j<gatesNumber;j++){
      if((gates+j)->rank == i){
	(stack+i)->number += 1;
	(stack+i)->gates = realloc((stack+i)->gates,(stack+i)->number * sizeof(GateStack*));
	*((stack+i)->gates + (stack+i)->number - 1) = gates + j;
      }
    }
  }
  c->stack = stack;
  return c;
}


int processFile(Circuit* cir, char* infile, char* outfile){
  FILE* ifilep;
  FILE* ofilep;
  int i,j,flag;
  int * nodes;
  
  ifilep = fopen(infile,"r");
  ofilep = fopen(outfile,"w");

  nodes = malloc(cir->nodes * sizeof(int));
  for(i=0; i<cir->nodes; i++){
    *(nodes+i) = 0;
  }
  while(1){
    flag = readBooleans(ifilep, cir->inputs, nodes);
    if(flag==0){
      break;
    }
    processCircuit(cir,nodes);
    printArray(cir->inputs, nodes);
    printf("->");
    printArray(cir->outputs, nodes + cir->nodes - cir->outputs);
    printf("\n");
    for(i=0;i<cir->outputs;i++){
      fputc(*(nodes + cir->nodes - cir->outputs + i)+'0',ofilep);
    }
    fputc('\n',ofilep);
  }

  fclose(ifilep);
  fclose(ofilep);
  return 1;
}

Circuit* readCircuitFile(char* filename){
  int i,totN,inN,outN,gateN;
  char line[20];
  Gate *gates;
  char c;

  FILE * fp;
  fp = fopen(filename,"r");
  nextLine(fp);
  fscanf(fp,"nodes:%d\n",&totN);
  nextLine(fp);
  fscanf(fp,"input:%d\n",&inN);
  nextLine(fp);
  fscanf(fp,"output:%d\n",&outN);
  nextLine(fp);
  fscanf(fp,"gates:%d\n",&gateN);
  nextLine(fp);

  gates = malloc(gateN * sizeof(Gate));
  printf("%d,%d,%d,%d\n",totN,inN,outN,gateN);

  for(i=0;i<gateN;i++){
    fscanf(fp,"%s\n",line);
    nextLine(fp);
    parseGateDef((gates+i),line); /*memory leak manage later*/
    printf("parsing %d gates\r",i+1);
  }
  printf("\n");
  fclose(fp);

  processGatesRank(gateN, inN, totN, gates);
  
  printGates(gateN,gates);

  Circuit* cir;
  cir = makeCircuit(gateN, gates);
  cir->inputs = inN;
  cir->outputs = outN;
  cir->nodes = totN;
  cir->gates = gateN;

  return cir;
}

int main(int argc,char* argv[]){
  char cirFilename[30],inFilename[30],outFilename[30];
  
  if (argc>1){
    strcpy(cirFilename,argv[1]);
  }else{
    printf("Enter filename (without extension):");
    scanf("%s",cirFilename);
  }
  strcpy(inFilename, cirFilename);
  strcpy(outFilename, cirFilename);
  strcat(cirFilename,".cir");
  strcat(inFilename,".inc");
  strcat(outFilename,".out");
  
  Circuit* cir;
  cir = readCircuitFile(cirFilename);
  printCircuit(cir);
  processFile(cir, inFilename, outFilename);
  printf("Output written in file: %s\n",outFilename);
  return 0;
}

