/************************************************************************
* Pierre Morel - 2003
*
* @name World.cc
* @memo contains curves and points of the work space
*
* @author Pierre Morel
* date march 2003
************************************************************************/

#include "World.h"
#include "Vector.h"
#include "Curve.h"
#include "Constraint.h"

void World::Init() {
  this->nbvector=0;
  this->nbcurve=0;
  this->nbconstraint=0;
}

World::World() {
  this->Init();
}

void World::AddVector(Vector * v) {
  if (nbvector<W_MAX_VECTOR) {
    v->SetId(nbvector+1);
    vectorlist[nbvector++]=v;
  }
}

int World::RmvVector(const int id) {
  if (id>0 && id<=nbvector) {
    this->RemoveConstraint(vectorlist[id-1]);
    delete vectorlist[id-1];
    if (id<nbvector) {
      vectorlist[id-1]=vectorlist[nbvector-1];
      vectorlist[id-1]->SetId(id);
    }
    nbvector--;
    return 1;
  }
  else {
    return 0;
  }
}

Vector* World::GetVector(const int id) {
  if (id>0 && id<=nbvector) {
    return vectorlist[id-1];
  }
  else {
    return NULL;
  }
}

int World::GetNbVector() {
  return nbvector;
}

void World::AddCurve(Curve * c) {
  if (nbcurve<W_MAX_CURVE) {
    c->SetId(nbcurve+1);
    curvelist[nbcurve++]=c;
  }
}

int World::RmvCurve(const int id) {
  if (id>0 && id<=nbcurve) {
    delete curvelist[id-1];
    if (id<nbcurve) {
      curvelist[id-1]=curvelist[nbcurve-1];
      curvelist[id-1]->SetId(id);
    }
    nbcurve--;
    return 1;
  }
  else {
    return 0;
  }
}

Curve* World::GetCurve(const int id) {
  if (id>0 && id<=nbcurve) {
    return curvelist[id-1];
  }
  else {
    return NULL;
  }
}

int World::GetNbCurve() {
  return nbcurve;
}

void World::AddConstraint(Constraint * v) {
  if (nbconstraint<W_MAX_CONSTRAINT) {
    constraintlist[nbconstraint++]=v;
  }
}

Constraint* World::GetConstraint(const int id) {
  if (id>0 && id<=nbconstraint) {
    return constraintlist[id-1];
  }
  else {
    return NULL;
  }
}

int World::GetNbConstraint() {
  return nbconstraint;
}

void World::ApplyConstraints() {
  int i,b1,b2;
  Vector *v1,*v2;
  Constraint *c;
  for(i=0;i<nbconstraint;i++) {
    c=constraintlist[i];
    v1=c->GetVectorA();
    b1=v1->IsSelected();
    v2=c->GetVectorB();
    b2=v2->IsSelected();
    if (b1 && !b2) {
      c->ApplyTransform(v1);
    }
    else if (!b1 && b2) {
      c->ApplyTransform(v2);
    }
  }
}

Constraint* World::SearchConstraint(Vector *v1,Vector *v2) {
  int i;
  Constraint *c;
  for(i=0;i<nbconstraint;i++) {
    c=constraintlist[i];
    if (v1->GetId()==c->GetVectorA()->GetId() && v2->GetId()==c->GetVectorB()->GetId()) {
      return c;
    }
  }
  return NULL;
}

void World::RefreshConstraints() {
 int i;
  for(i=0;i<nbconstraint;i++) {
    constraintlist[i]->Refresh();
  }
}

void World::RemoveConstraint(Vector *v) {
 int i,id;
 id=v->GetId();
  for(i=0;i<nbconstraint;i++) {
    Constraint *c;
    c=constraintlist[i];
    if (c->GetVectorA()->GetId()==id ||
	c->GetVectorB()->GetId()==id ||
	c->GetVectorO()->GetId()==id) {
      delete c;
      constraintlist[i]=constraintlist[--nbconstraint];
    }
  }
}
