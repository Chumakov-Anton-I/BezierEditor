/************************************************************************
* Pierre Morel - 2003
*
* @name TextFile.cc
* @memo Read and write data in a simple text file
*
* @author Pierre Morel
* date march 2003
************************************************************************/

#include "TextFile.h"
#include "Vector.h"
#include "Curve.h"
#include "World.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

TextFile::TextFile(char * input) {
  this->input=input;
}

int TextFile::Read(World * world) {
  Curve * curve=NULL;
  Vector P;
  char x[50],y[50];
  char line[100];
  FILE * file=fopen(this->input,"r");
  if (!file)
    return 1;
  while (fgets(line,100,file)!=NULL) {
    if ((line[0] == '#') || (line[0] == '\n' )) {
      continue;
    }
    if ((line[0] == 'C') || (line[0] == 'c' )) {
      curve=new Curve(world);
      world->AddCurve(curve);
      continue;
    }
    sscanf(line,"%s %s",x,y);
    P.Set((float)atof(x),(float)atof(y),0);
    curve->AddPoint(&P);
  }
  fclose(file);
  return 0;
}


int TextFile::Write(World * world) {
  Curve * curve;
  Vector * P;
  int i,j;
  FILE * file=fopen(this->input,"w");
  if (!file)
    return 1;
  for(i=0;i<world->GetNbCurve();i++) {
    fprintf(file,"C\n");
    curve=world->GetCurve(i+1);
    for(j=0;j<curve->GetNbPoint();j++) {
      P=curve->GetPoint(j);
      fprintf(file,"%f %f\n",P->GetX(),P->GetY());
    }
  }
  fclose(file);
  return 0;
}
