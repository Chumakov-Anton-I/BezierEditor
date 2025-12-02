/************************************************************************
* Pierre Morel - 2003
*
* @name Constraint.cc
* @memo Defines some constraints between points (for instance, to translate 
* @meno several points when user translates only one)
*
* @author Pierre Morel
* date march 2003
************************************************************************/

#include "Constraint.h"

World * Constraint::world;


void Constraint::SetWorld(World * w) {
  world=w;
}

Constraint::Constraint() {
  this->A=NULL;
  this->B=NULL;
  this->O=new Vector(0,0,0);
  this->mode=CO_TRANSLATION;
  this->alpha=0;
  this->beta=0;
}

Constraint::Constraint(Vector *A,Vector *B) {
  this->A=A;
  this->B=B;
  this->mode=CO_TRANSLATION;
  this->O=B->Sub(A);
  this->alpha=0;
  this->beta=0;
}

Constraint::~Constraint() {
}

void Constraint::Init() {
  Vector *v,*w,*u;
  float x,y,z;
  //scale
  v=A->Sub(O);
  w=B->Sub(O);
  x=v->Length();
  y=w->Length();
  if (x==0) {
    this->beta=1;
  }
  else {
    this->beta=y/x;
  }
  //angle
  z=v->DotProduct(w)/(x*y);
  this->alpha=acos(z);
  u=v->CrossProduct(w);
  u->Scale(1/(x*y));
  if (u->GetZ()<0)
    this->alpha=-this->alpha;
  delete v;
  delete w;
  delete u;
}

Constraint::Constraint(Vector *A,Vector *B,Vector *O) {
  this->A=A;
  this->B=B;
  this->mode=CO_HOMOTHETY;
  this->O=O;
  this->Init();
}

void Constraint::SetTranslation(Vector*V) {
  this->mode=CO_TRANSLATION;
  this->O=V;
}


void Constraint::SetHomothety(Vector*V,float angle,float scale) {
  this->mode=CO_HOMOTHETY;
  this->O=V;
  this->alpha=angle;
  this->beta=scale;
}

void Constraint::Refresh() {
  if (this->mode==CO_TRANSLATION) {
    this->O->Set(this->B->Sub(this->A));
  }
  else if (this->mode==CO_HOMOTHETY) {
    this->Init();
  }
}


Vector* Constraint::GetVectorA() {
  return this->A;
}

Vector* Constraint::GetVectorB() {
  return this->B;
}

Vector* Constraint::GetVectorO() {
  return this->O;
}

void Constraint::ApplyTransform(Vector* v) {
  Vector *V1,*V2;
  if (mode==CO_TRANSLATION) {
    if (this->A->GetId()==v->GetId()) {
      V1=A->ScaleCopy(1);
      V2=V1->Add(this->O);
      B->Set(V2);
      delete V1;
      delete V2;
    }
  }
  else if (mode==CO_HOMOTHETY) {
    if (this->A->GetId()==v->GetId()) {
      V1=A->ScaleCopy(1);
      V2=V1->Homothety(this->O,this->alpha,this->beta);
      B->Set(V2);
      delete V1;
      delete V2;
    }
  }
}
