/************************************************************************
* Pierre Morel - 2003
*
* @name Color.h
* @memo Manipulates colors (four floats between 0 and 1 corresponding to
* @meno red, green and blue color and transparency)
*
* @author Pierre Morel
* date march 2003
************************************************************************/

#include "Color.h"
#include <GL/glu.h>

void Color::Set(float r,float g,float b) {
  this->r=r;
  this->g=g;
  this->b=b;
}

Color::Color(float r,float g,float b) {
  this->Set(r,g,b);
}

Color::Color(COLORenum c) {
  if (c==WHITE) 
    Set(1,1,1); 
  else if(c==BLACK)
    Set(0,0,0); 
  else if(c==RED)
    Set(1,0,0);
  else if(c==GREEN)
    Set(0,1,0);
  else if(c==BLUE)
    Set(0,0,1);
  else if(c==YELLOW)
    Set(1,1,0);
}

void Color::GlColor3fv() {
    float color[3] = { r, g, b };
    //color[0] = this->r;
    //color[1] = this->g;
    //color[2] = this->b;
    glColor3fv(color);
}

Color * Color::Select() {
  return new Color(r*0.5f, g*0.5f, b*0.5f);
}

Color *white  = new Color(1, 1, 1);
Color *black  = new Color(0, 0, 0);
Color *red    = new Color(1, 0, 0);
Color *green  = new Color(0, 1, 0);
Color *blue   = new Color(0, 0, 1);
Color *yellow = new Color(1, 1, 0);
