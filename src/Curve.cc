/************************************************************************
* Pierre Morel - 2003
*
* @name Curve.cc
* @memo Manipulate a bezier curve, ie a set of control points
*
* @author Pierre Morel
* date march 2003
************************************************************************/

#include "Curve.h"
#include "Vector.h"
#include "World.h"
#include "Color.h"
#include "Constraint.h"
#include "Selection.h"
#include <string.h>

Selection* Curve::selection;

void Curve::Init(const int color,const char*name,World * world) {
  this->size=0;
  this->color=color;
  this->name=(char*)malloc(50);
  strncpy(this->name,name,50);
  this->world=world;
  this->selected=0;
  this->visible=1;
}

Curve::Curve(const int color,const char *name,World * world) {
  Init(color,name,world);
}

Curve::Curve(World * world) {
  Init(1,"",world);
}

/**
 * @TODO must remove references in any selection
 */
Curve::~Curve() {
  int i;
  for(i=0;i<this->size;i++) {
    this->world->RmvVector(PointList[i]->GetId());
  }
}

void Curve::SetSelection(Selection*s) {
  selection=s;
}

void Curve::SetId(int id) {
  this->id=id;
}

int Curve::GetId() {
  return this->id;
}

void Curve::Select() {
  this->selected=1;
}

void Curve::UnSelect() {
  this->selected=0;
}

int Curve::IsSelected() {
  return this->selected;
}

void Curve::Visible() {
  this->visible=1;
}

void Curve::InVisible() {
  this->visible=0;
}

int Curve::IsVisible() {
  return this->visible;
}


int Curve::AddPoint(const Vector *v) {
  if (size>=NB_MAX_POINTS) {
    return 0;
  }
  else {
    PointList[size++]=new Vector(v);
    this->world->AddVector(PointList[size-1]);
    return 1;
  }
}

int Curve::RmvPoint(int id) {
  int i,j;
  Vector *v;
  if (this->size<=4) {
    return 0;
  }
  for(i=0;i<this->size;i+=3) {
    v=PointList[i];
    if (v->GetId()==id) {
      if (i==0) {
	world->RmvVector(PointList[0]->GetId());
	world->RmvVector(PointList[1]->GetId());
	world->RmvVector(PointList[2]->GetId());
	for(j=0;j<this->size-3;j++) {
	  PointList[j]=PointList[j+3];
	}
      }
      else if (i==this->size-1) {
	world->RmvVector(PointList[i]->GetId());
	world->RmvVector(PointList[i-1]->GetId());
	world->RmvVector(PointList[i-2]->GetId());
      }
      else {
	world->RmvVector(PointList[i-1]->GetId());
	world->RmvVector(PointList[i]->GetId());
	world->RmvVector(PointList[i+1]->GetId());
	for(j=i-1;j<this->size-3;j++) {
	  PointList[j]=PointList[j+3];
	}
      }
      this->size-=3;
      return 1;
    }
  }
  return 0;
}

Vector* Curve::ComputePoint(float x, float y) {
  float t,tmin=0,dmin,d;
  int i,imin=0;
  float v[2];
  Compute(0,0,v);
  dmin=sqr(x-v[0])+sqr(y-v[1]);
  for(i=0;i<size-3;i+=3) {
    for(t=0;t<1;t+=STEP) {
      Compute(i,t,v);
      d=sqr(x-v[0])+sqr(y-v[1]);
      if (d<dmin) { dmin=d; imin=i; tmin=t; }      
    }
    t=1;
    Compute(i,t,v);
    d=sqr(x-v[0])+sqr(y-v[1]);
    if (d<dmin) { dmin=d; imin=i; tmin=t; }      
  }
  Compute(imin,tmin,v);
  if (imin==0 && tmin==0) {
    return new Vector(x,y,0);
  }
  if (imin==size-4 && tmin==1) {
    return new Vector(x,y,0);
  }
  return new Vector(v[0],v[1],0);
}

void Curve::ComputePoint2(float x, float y) {
  float t,tmin=0,dmin,d;
  int i,imin=0;
  float v[2];
  Vector *V1,*V2,*B,*C,*D,*E,*F,*I;
  Compute(0,0,v);
  dmin=sqr(x-v[0])+sqr(y-v[1]);
  for(i=0;i<size-3;i+=3) {
    for(t=0;t<1;t+=STEP) {
      Compute(i,t,v);
      d=sqr(x-v[0])+sqr(y-v[1]);
      if (d<dmin) { dmin=d; imin=i; tmin=t; }      
    }
    t=1;
    Compute(i,t,v);
    d=sqr(x-v[0])+sqr(y-v[1]);
    if (d<dmin) { dmin=d; imin=i; tmin=t; }      
  }
  if (imin==0 && tmin==0) {
    for(i=size;i>0;i--) {
      PointList[i+2]=PointList[i-1];
    }
    V1=new Vector(x,y,0);
    PointList[0]=V1;
    this->world->AddVector(V1);

    V2=PointList[3]->ScaleCopy(2);
    V1=V2->Sub(PointList[4]);
    PointList[2]=V1;
    this->world->AddVector(V1);
    delete V2;

    V2=V1->Add(PointList[0]);
    V2->Scale(0.5);
    PointList[1]=V2;
    this->world->AddVector(V2);
    this->world->AddConstraint(new Constraint(PointList[0],PointList[1]));
    this->world->AddConstraint(new Constraint(PointList[3],PointList[2]));
    this->world->AddConstraint(new Constraint(PointList[2],PointList[4],PointList[3]));
    this->world->AddConstraint(new Constraint(PointList[4],PointList[2],PointList[3]));
    size+=3;
    }
  else if (imin==size-4 && tmin==1) {
    V2=PointList[size-1]->ScaleCopy(2);
    V1=V2->Sub(PointList[size-2]);
    AddPoint(V1);
    delete V2;

    V2=new Vector(x,y,0);
    V1=V1->Add(V2);
    V1->Scale(0.5);
    AddPoint(V1);
    AddPoint(V2);
    
    this->world->AddConstraint(new Constraint(PointList[size-1],PointList[size-2]));
    this->world->AddConstraint(new Constraint(PointList[size-4],PointList[size-3]));
    this->world->AddConstraint(new Constraint(PointList[size-3],PointList[size-5],PointList[size-4]));
    this->world->AddConstraint(new Constraint(PointList[size-5],PointList[size-3],PointList[size-4]));
    
  }
  else {
    Constraint * C1;
    Compute(imin,tmin,v);
    for(i=size;i>imin+2;i--) {
      PointList[i+2]=PointList[i-1];
    }

    B=PointList[imin]->Interpolate(PointList[imin+1],tmin);
    F=PointList[imin+5]->Interpolate(PointList[imin+6],tmin);
    I=PointList[imin+1]->Interpolate(PointList[imin+5],tmin);
    C=B->Interpolate(I,tmin);
    E=I->Interpolate(F,tmin);
    D=C->Interpolate(E,tmin);

    PointList[imin+1]->Set(B);
    PointList[imin+2]=C;
    PointList[imin+3]=D;
    PointList[imin+4]=E;
    PointList[imin+5]->Set(F);

    this->world->AddVector(B);
    this->world->AddVector(C);
    this->world->AddVector(D);
    this->world->AddVector(E);
    this->world->AddVector(F);

    C1=world->SearchConstraint(PointList[imin],PointList[imin+1]);
    if (C1!=NULL) {
      C1->Refresh(); }

    C1=world->SearchConstraint(PointList[imin+6],PointList[imin+5]);
    if (C1!=NULL) {
      C1->Refresh(); }

    C1 =new Constraint(PointList[imin+3],PointList[imin+2]);
    world->AddConstraint(C1);
    C1 =new Constraint(PointList[imin+3],PointList[imin+4]);
    world->AddConstraint(C1);
    C1 =new Constraint(PointList[imin+4],PointList[imin+2],PointList[imin+3]);
    world->AddConstraint(C1);
    C1 =new Constraint(PointList[imin+2],PointList[imin+4],PointList[imin+3]);
    world->AddConstraint(C1);
    delete I;
    delete B;
    delete F;
    size+=3;
  }
}


void Curve::AddConstraints() {
  int i;
  Constraint *c;
  for(i=0;i<size-3;i+=3) {
    c =new Constraint(PointList[i],PointList[i+1]);
    world->AddConstraint(c);
    c =new Constraint(PointList[i+3],PointList[i+2]);
    world->AddConstraint(c);
    if (i+4<size) {
      c =new Constraint(PointList[i+4],PointList[i+2],PointList[i+3]);
      world->AddConstraint(c);
      c =new Constraint(PointList[i+2],PointList[i+4],PointList[i+3]);
      world->AddConstraint(c);
    }  
  }
}


void Curve::Translate(const Vector *v) {
  int i;
  for(i=0;i<size;i++) {
    PointList[i]->Translate(v);
  }
}


Vector* Curve::GetPoint(int i) {
  if (i>=size || i<0 ) {
    return NULL;
  }
  else {
    return PointList[i];
  }
}

int Curve::GetNbPoint() {
  return this->size;
}

void Curve::Draw(BEenum mode) {
  int i;
  float t;
  float v[2];
  if (IsVisible()==0) {
    return;
  }
  if(mode==BE_SELECT_CURVE) {
    glLoadName(this->id);
  }
  glPushMatrix();
  if (IsSelected()) {
    if (selection!=NULL) {
      float dx,dy,dz;
      Vector *w;
      w=selection->GetTranslation();
      dx=w->GetX();
      dy=w->GetY();
      dz=w->GetZ();
      glTranslatef(dx,dy,dz);
    }
  }
  for(i=0;i<size-3;i+=3) {
    PointList[i]->Draw(mode,green);
    PointList[i+1]->Draw(mode,blue);
    // line between i and i+1
    PointList[i]->Draw(PointList[i+1],mode,blue);
    // line between i+2 and i+3
    PointList[i+2]->Draw(mode,blue);
    if (i+3<size) {
      PointList[i+2]->Draw(PointList[i+3],mode,blue);
    }
    else if(mode==BE_SELECT_POINT) {
      glLoadName(0);
    }
    if (IsSelected()) {
      yellow->GlColor3fv();
    }
    else {
      red->GlColor3fv();
    }
    glBegin(GL_LINE_STRIP);
    for(t=0;t<1;t+=STEP) {
      Compute(i,t,v);
      glVertex2fv(v);
    }
    Compute(i,1,v);
    glVertex2fv(v);
    glEnd();
  }
  if (i<size)
    PointList[i]->Draw(mode,green);
  glPopMatrix();
}

void Curve::Compute(int i,float t,float v[2]) {
  float t1=1-t;
  Vector *P1,*P2,*P3,*P4,*P;
  P1=PointList[i]->ScaleCopy(t1*t1*t1);
  P2=PointList[i+1]->ScaleCopy(3*t*t1*t1);
  P3=PointList[i+2]->ScaleCopy(3*t*t*t1);
  P4=PointList[i+3]->ScaleCopy(t*t*t);
  P=P1->Add(P2);
  delete P1;
  delete P2;
  P1=P->Add(P3);
  delete P;
  delete P3;
  P=P1->Add(P4);
  delete P1;
  delete P4;
  v[0]=P->GetX();
  v[1]=P->GetY();
  delete P;
}
