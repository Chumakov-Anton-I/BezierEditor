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

#ifndef COLOR
#define COLOR

#include "Function.h"
#include "GL/glu.h"

enum COLORenum {
  WHITE, BLACK, RED, GREEN, BLUE, YELLOW };

class Color {
protected:
  float r,g,b,a;

public:
  Color(float r,float g,float b);
  Color(COLORenum c);
  void GlColor3fv();
  void Set(float r,float g,float b);
  Color * Select();
};

extern Color *white,*black,*red,*green,*blue,*yellow;

#endif

