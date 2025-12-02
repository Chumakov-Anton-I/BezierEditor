/************************************************************************
* Pierre Morel - 2003
*
* @name Vector.h
* to manipulate Vector (add, substracte, Draw ...)
*
* @author Pierre Morel
* @date march 2003
************************************************************************/

#ifndef VECTOR
#define VECTOR

#include "Function.h"

class Selection;
class Color;

/**
 *
 * @name Vector
 * to manipulate Vector
 * @TODO change selection to world because there are (not yet) many selection but only one world
 * @TODO clarify whether each method copy the vector or work directly on the instance
 * @TODO add a method equal which return 1 if two vector are equal, (ie by id or by distance ?)
 * @see Selection
 */
class Vector {

 protected:
  float x;
  float y;
  float z;
  int id;
  int selected;
  static Selection * selection;
  static int count;
  static float step;
  void Init(float x,float y,float z);
  
 public:
  Vector();
  Vector(float x,float y,float z);
  Vector(const Vector *v);
  ~Vector();

  /**
   * sets the current selection (only one for the moment). selection contains
   * several informations. In particular, the translation to operate when 
   * user is moving the selection (even there is only one point selected).
   * the vector needs to get this translate vector.
   * this is a static method since, there is only one selection to all the points
   * @param selection the current selection
   */
  static void SetSelection(Selection *selection);

  /**
   * changes the "step". The step directly corresponds to the size of objects which
   * are drawn. Actually, when a point is drawn, represented with a small square, this
   * square is the same whatever is the zoom, because the size of the square depends
   * rightly of this step.
   * once again, this method is a static one since the step is the same to every points
   * @param s the new step (normally conversely proportional to the zoom scale)
   */
  static void Vector::SetStep(float s);

  /**
   * set the three components of the vector with three floats
   * @param x x component
   * @param y y component
   * @param z z component
   */
  void Set(float x,float y,float z);

  /**
   * set the three components of the vector with one existing vector
   * @param v the source vector
   */
  void Set(const Vector *v);

  /**
   * set the id of the vector
   * vector IDs are controlled by the world class
   * @int id the new id of the vector
   * @see GetId
   */
  void SetId(int id);

  /**
   * return the id of the vector
   * @return the id of the vector
   * @see SetId
   */
  int GetId();

  /**
   * marks this vector as selected (in the current selection)
   * @see UnSelect
   * @see IsSelected
   */
  void Select();

  /**
   * marks this vector as unselected (in the current selection)
   * @see Select
   * @see IsSelected
   */
  void UnSelect();

  /**
   * return whether this vector is selected or not
   * @return 0 if the vector is not selected
   * @return 1 if the vector is selected
   * @see Select
   * @see UnSelect
   */
  int IsSelected();

  /**
   * extracts the x-component
   * @return the x-component
   */
  float GetX();

  /**
   * extracts the y-component
   * @return the y-component
   */
  float GetY();

  /**
   * extracts the z-component
   * @return the z-component
   */
  float GetZ();

  /**
   * Add the instance with the parameter and return a NEW vector
   * @param v the vector to add to the instance
   * @return the result of the operation (It is a NEW vector)
   * @see Sub
   * @see Translate
   */
  Vector* Add(const Vector *v);

  /**
   * Sub the instance to the parameter and return a NEW vector
   * @param v the vector to sub to the instance
   * @return the result of the operation (It is a NEW vector)
   * @see Add
   */
  Vector* Sub(const Vector *v);

  /**
   * computes an Homothety. It returns a NEW vector
   * @param o the center of the homothety
   * @param angle the angle of the rotation (in radian)
   * @param scale the scale of the homothety
   * @return a new vector representing the transformation of the instance by the homothety
   */
  Vector* Homothety(Vector *o,float angle,float scale);

  /**
   * computes a dot product between the instance and the parameter
   * @param v the second vector
   * @return the dot product
   */
  float DotProduct(const Vector *v);

  /**
   * computes a Cross product between the instance and the parameter and return it in a NEW vector
   * @param v the second vector
   * @return the Cross product, it is a NEW vector
   * @see ScaleCopy
   */
  Vector* CrossProduct(const Vector *v);

  /**
   * multiplies the instance by a scalar. Works directly on the instance
   * @param a the scalar
   */
  void Scale(float a);

  /**
   * multiplies the instance by a scalar and returns the result in a NEW vector
   * @param a the scalar
   * @return a new vector 
   * @see Scale
   */
  Vector* ScaleCopy(float a);

  /**
   * normalises the instance, ie divided itself by its length
   * works on the instance
   * @see Length
   */
  void Normalize();

  /**
   * computes the length of the vector
   * @return the length of the instance
   * @see Normalize
   */
  float Length();

  /**
   * translate the instance
   * it is the same method than add but works directly on the instance
   * @see Add
   */
  void Translate(const Vector *v);

  /**
   * puts the three components of the vector in a char string
   * @return a char string which containts the three components
   * @see Print
   */
  char * ToString();

  /**
   * prints to the standard output the three components
   * @see ToString
   */
  void Print();

  /**
   * Draws a point, ie a small square
   * @param color the color of the square
   * @param mode the mode of drawing (draw or select mode)
   */
  void Draw(BEenum mode,Color * color);

  /**
   * Draws a line between the instance and the first parameter
   * @param v the second end of the line
   * @param mode the mode of drawing
   * @param color the color of the line
   */
  void Draw(Vector *v,BEenum mode,Color * color);

  /**
   * compute linear interpolation between two points according to a parameter
   * @param V the second point
   * @param t the coefficient
   */
  Vector* Interpolate(Vector *V,float t);

  /**
   * the count of the number of vector is update when create and destroy
   * a vector (inside constructors and destructors)
   * @return the number of Vectors defined at the present time 
   */
  static int NbVector();

  /**
   * return if the instance is included in a box defined by two points (two
   */
  int IsIncluded(Vector *a,Vector *b);
};

#endif
