/************************************************************************
* Pierre Morel - 2003
*
* @name Selection.cc
* @memo contains a subset of curves and points selected by the user
* @meno Allows to manipulate (for instance to move) several points or
* @meno curves at the same time
*
* @author Pierre Morel
* date march 2003
************************************************************************/

#include "Selection.h"
#include "Vector.h"
#include "Curve.h"
#include "World.h"
#include "Color.h"

void Selection::Init(World * world) {
  nbvector=0;
  nbcurve=0;
  mode=SEVECTOR;
  this->world=world;
  start=new Vector();
  translation=new Vector();
  box1=new Vector();
  box2=new Vector();
}

void Selection::SetWorld(World * world) {
  this->world=world;
}
 
Selection::Selection(World *world) {
  this->Init(world);
}

void Selection::SelectMode(SEnum mode) {
  if (this->mode==SEVECTOR) {
    int i;
    for(i=0;i<nbvector;i++) {
      vectorlist[i]->UnSelect();
    }
  }
  else {
    int i;
    for(i=0;i<nbcurve;i++) {
      curvelist[i]->UnSelect();
    }
  }
  this->mode=mode;
  nbvector=0;
  nbcurve=0;
}

void Selection::AddVector(int id) {
  Vector *v;
  if (this->mode!=SEVECTOR) {
    this->SelectMode(SEVECTOR);
  }
  if (RmvVector(id)==1) {
    return;
  }
  v=world->GetVector(id);
  this->vectorlist[nbvector++]=v;
  v->Select();
}

int Selection::GetNbVector() {
  if (this->mode==SEVECTOR) {
    return this->nbvector;
  }
  return 0;
}

int Selection::RmvVector(int id) {
  if (this->mode==SEVECTOR) {
    int i;
    for(i=0;i<nbvector;i++) {
      if (this->vectorlist[i]->GetId()==id) {
	this->vectorlist[i]->UnSelect();
	if (i<nbvector-1) {
	  this->vectorlist[i]=this->vectorlist[nbvector-1];
	}
	nbvector--;
	return 1;
      }
    }
  }
  return 0;
}

Vector* Selection::GetVector(int i) {
  if (mode==SEVECTOR && i>=0 && i<nbvector) {
    return this->vectorlist[i];
  }
  return NULL;
}


void Selection::AddCurve(int id) {
  Curve *c;
  if (this->mode!=SECURVE) {
    this->SelectMode(SECURVE);
  }
  if (RmvCurve(id)==1) {
    return;
  }
  c=world->GetCurve(id);
  this->curvelist[nbcurve++]=c;
  c->Select();
}

int Selection::GetNbCurve() {
  if (this->mode==SECURVE) {
    return this->nbcurve;
  }
  return 0;
}

int Selection::RmvCurve(int id) {
  if (this->mode==SECURVE) {
    int i;
    for(i=0;i<nbcurve;i++) {
      if (this->curvelist[i]->GetId()==id) {
	this->curvelist[i]->UnSelect();
	if (i<nbcurve-1) {
	  this->curvelist[i]=this->curvelist[nbcurve-1];
	}
	nbcurve--;
	return 1;
      }
    }
  }
  return 0;
}

Curve* Selection::GetCurve(int i) {
  if (mode==SECURVE && i>=0 && i<nbcurve) {
    return this->curvelist[i];
  }
  return NULL;
}


void Selection::Move(Vector*v) {
  int i;
  this->FixEnd(v);
  for(i=0;i<nbvector;i++) {
    vectorlist[i]->Translate(this->translation);
  }
  for(i=0;i<nbcurve;i++) {
   curvelist[i]->Translate(this->translation);
  }
  this->translation->Set(0,0,0);
  this->world->RefreshConstraints();
}

void Selection::FixStart(Vector*v) {
  this->start->Set(v);
  this->translation->Set(0,0,0);
}


void Selection::FixEnd(Vector*v) {
  Vector *s;
  s=v->Sub(this->start);
  this->translation->Set(s);
  delete s;
}

Vector * Selection::GetTranslation() {
  return this->translation;
}

void Selection::SetBoxStart(Vector *v) {
  this->box1->Set(v);
  this->box2->Set(v);
}

Vector* Selection::GetBoxStart() {
  return this->box1;
}


void Selection::SetBoxEnd(Vector *v) {
  this->box2->Set(v);
}

Vector* Selection::GetBoxEnd() {
  return this->box2;
}

int Selection::IsBoxIncluded(Vector *v) {
  return v->IsIncluded(this->box1,this->box2);
}


void Selection::DrawBox() {
  Vector *x=new Vector(this->box1->GetX(),this->box2->GetY(),0);
  x->Draw(this->box1,BE_RENDER,white);
  x->Draw(this->box2,BE_RENDER,white);
  delete x;
  x=new Vector(this->box2->GetX(),this->box1->GetY(),0);
  x->Draw(this->box1,BE_RENDER,white);
  x->Draw(this->box2,BE_RENDER,white);
  delete x;
}
