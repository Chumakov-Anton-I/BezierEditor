/************************************************************************
* Pierre Morel - 2003
*
* @name Vector.cc
* @memo to manipulate Vector (add, substracte, Draw ...)
*
* @author Pierre Morel
* date march 2003
************************************************************************/

#include "Vector.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include "Selection.h"
#include "Color.h"

Selection* Vector::selection=NULL;
int Vector::count=0;
float Vector::step=2;

void Vector::Init(float x,float y,float z) {
  this->x=x;
  this->y=y;
  this->z=z;
  this->selected=0;
  this->id=0;
  count++;
}

Vector::~Vector() {
  count--;
}

Vector::Vector(float x,float y,float z) {
  Init(x,y,z);
}

Vector::Vector() {
  Init(0,0,0);
}


Vector::Vector(const Vector *v) {
  Init(v->x,v->y,v->z);
}

void Vector::SetSelection(Selection *s) {
  selection=s;
}

void Vector::SetStep(float s) {
  step=s;
}

void Vector::Select() {
  this->selected=1;
}

void Vector::UnSelect() {
  this->selected=0;
}

int Vector::IsSelected() {
  return this->selected;
}

void Vector::Set(float x,float y,float z) {
  this->x=x;
  this->y=y;
  this->z=z;
}

void Vector::Set(const Vector * v) {
  this->x=v->x;
  this->y=v->y;
  this->z=v->z;
}

int Vector::GetId() {
  return id;
}

void Vector::SetId(int id) {
  this->id=id;
}

float Vector::GetX() {
  return x;
}

float Vector::GetY() {
  return y;
}

float Vector::GetZ() {
  return z;
}

Vector * Vector::Add(const Vector *v) {
  Vector *r;
  r=new Vector(this);
  r->x+=v->x;
  r->y+=v->y;
  r->z+=v->z;
  return r;
}

Vector * Vector::Sub(const Vector *v) {
  Vector *r;
  r=new Vector(this);
  r->x-=v->x;
  r->y-=v->y;
  r->z-=v->z;
  return r;
}

Vector * Vector::Homothety(Vector *o,float angle,float scale) {
  Vector *r;
  float x,y,xb,yb;
  r=this->Sub(o);
  x=r->GetX();
  y=r->GetY();
  xb=o->GetX()+scale*(cos(angle)*x-sin(angle)*y);
  yb=o->GetY()+scale*(cos(angle)*y+sin(angle)*x);
  r->Set(xb,yb,r->GetZ());
  return r;
}

float Vector::DotProduct(const Vector *v) {
  return this->x*v->x+this->y*v->y+this->z*v->z;
}

Vector * Vector::CrossProduct(const Vector *v) {
  Vector *r;
  r=new Vector();
  r->x=this->y*v->z-this->z*v->y;
  r->y=this->z*v->x-this->x*v->z;
  r->z=this->x*v->y-this->y*v->x;
  return r;
}

void Vector::Scale(float a){
  this->x*=a;
  this->y*=a;
  this->z*=a;
}

Vector * Vector::ScaleCopy(float a){
  Vector *r,*w;
  float dx=0,dy=0,dz=0;
  r=new Vector();
    if (this->selected==1) {
      if (selection!=NULL) {
	w=selection->GetTranslation();
	dx=w->GetX();
	dy=w->GetY();
	dz=w->GetZ();
      }
    }
  r->x=(this->x+dx)*a;
  r->y=(this->y+dy)*a;
  r->z=(this->z+dz)*a;
  return r;
}


float Vector::Length(){
  return sqrt(sqr(this->x)+sqr(this->y)+sqr(this->z));
}

void Vector::Normalize(){
  float l=Length();
  if (l==0) return;
  this->x/=l;
  this->y/=l;
  this->z/=l;
}

void Vector::Translate(const Vector *v) {
  this->x+=v->x;
  this->y+=v->y;
  this->z+=v->z;
}

char * Vector::ToString(){
  char * s;
  s=(char*)malloc(50);
  sprintf(s,"%7.2f %7.2f %7.2f",this->x,this->y,this->z);
  return s;
}

void Vector::Print(){
  printf("%s\n",ToString());
}



void Vector::Draw(BEenum mode,Color * color) {
  float v[2];
  float dx=0,dy=0;
  Vector * w;
    if (mode==BE_SELECT_POINT) {
      glLoadName(this->id);
      //printf("%d\n",this->id);
    }
    if (this->selected==1) {
      if (this->selection!=NULL) {
	Color *c2;
	w=this->selection->GetTranslation();
	dx=w->GetX();
	dy=w->GetY();
	c2=color->Select();
	c2->GlColor3fv();
      }
    }
    else {
      color->GlColor3fv();
    }

	
    glBegin(GL_POLYGON);
    v[0]=this->x-step+dx;
    v[1]=this->y-step+dy;
    glVertex2fv(v);
    v[0]=this->x+step+dx;
    v[1]=this->y-step+dy;
    glVertex2fv(v);
    v[0]=this->x+step+dx;
    v[1]=this->y+step+dy;
    glVertex2fv(v);
    v[0]=this->x-step+dx;
    v[1]=this->y+step+dy;
    glVertex2fv(v);
    glEnd();
}

void Vector::Draw(Vector *g,BEenum mode,Color * color) {
  float v[2];
  Vector * w;
    if (mode==BE_SELECT_POINT) {
      glLoadName(0);
    }
    color->GlColor3fv();
    glBegin(GL_LINE_STRIP); 
    w=this->ScaleCopy(1);
    v[0]=w->GetX();
    v[1]=w->GetY();
    glVertex2fv(v);
    delete w;
    w=g->ScaleCopy(1);
    v[0]=w->GetX();
    v[1]=w->GetY();
    glVertex2fv(v);
    glEnd();
    delete w;
}

int Vector::NbVector() {
  return count;
}


Vector* Vector::Interpolate(Vector *V,float t) {
Vector *w,*x,*z;
 z=this->ScaleCopy(1-t);
 w=V->ScaleCopy(t);
 x=z->Add(w);
 delete z;
 delete w;
 return x;
}

int Vector::IsIncluded(Vector *a,Vector *b) {
  if ( (a->GetX()-x)*(b->GetX()-x)>0)
    return 0;
  if ( (a->GetY()-y)*(b->GetY()-y)>0)
    return 0;
  return 1;
}
