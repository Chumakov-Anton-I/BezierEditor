/************************************************************************
* Pierre Morel - 2003
*
* @name Curve.h
* Manipulate a bezier curve, ie a set of control points
*
* @author Pierre Morel
* @date march 2003
************************************************************************/

#ifndef CURVE
#define CURVE

#include <GL/glut.h>
#include "Function.h"

#define NB_MAX_POINTS 100
#define STEP 0.05

class World;
class Vector;
class Constraint;
class Selection;

/**
 * @name Curve
 * Manipulate a bezier Curve, ie a set of control points
 */
class Curve{

private:
  /**
   * the set of the control points which define the bezier curve
   */
  Vector* PointList[NB_MAX_POINTS];

  /**
   * the number of points of the curve
   */
  int size;

  /**
   * the color of the curve (in the palette)
   * @TODO not used yet
   */
  int color;

  /**
   * the name of the curve
   * @TODO not used yet
   */
  char * name;

  /**
   * the id of the curve (the world class uses and modifies this id)
   */
  int id;

  /**
   * a boolean to know if the curve is selected (ie, included in a selection)
   * this data is redundant with the selection but to go through a list is expensive
   */
  int selected;

  /**
   * a boolean to kown whether the curve is visible or not
   */
  int visible;

  /**
   * a reference to the world (to reach a vector)
   */
  World* world;

  /**
   * a reference to the selection
   * @TODO there are two references: to the world and to the selection. It is redundant
   */
  static Selection* selection;

public:

  /**
   * Set some parameters of the curve. Not yet used for most parameters, 
   * load and save this parameters from the file (in an XML format)
   * @param color the color of the curve (not yet used)
   * @param name the name of the curve (not yet used)
   * @param world the world in which the curve is included (perharps not relevant with only one workspace
   * @TODO change "Init" to "Set"
   */
  void Init(const int color,const char*name,World * world);

  Curve(const int color,const char *name,World * world);

  Curve(World * world);

  ~Curve();

  /**
   * Set the selection
   * @TODO remove this selection attribute, an access to the world is enough
   */
  static void SetSelection(Selection * selection);

  /**
   * Set the id of the curve. Id is managed by the World class only
   * @param id the new id of the curve
   */
  void SetId(int id);

  /**
   * @return the id of the curve
   */
  int GetId();

  /**
   * marks this curve as selected (in the current selection)
   * @see UnSelect
   * @see IsSelected
   */
  void Select();

  /**
   * marks this curve as unselected (in the current selection)
   * @see Select
   * @see IsSelected
   */
  void UnSelect();

  /**
   * return whether this curve is selected or not
   * @return 0 if the curve is not selected
   * @return 1 if the curve is selected
   * @see Select
   * @see UnSelect
   */
  int IsSelected();

  /**
   * marks this curve as visible (in the current selection)
   * then the curve is drawn
   * @see InVisible
   * @see IsVisible
   */
  void Visible();

  /**
   * marks this curve as invisible (in the current selection)
   * then the curve is not drawn
   * @see Visible
   * @see IsVisible
   */
  void InVisible();

  /**
   * return whether this curve is visible or not
   * @return 0 if the curve is not visible
   * @return 1 if the curve is visible
   * @see Visible
   * @see InVisible
   * @see Draw
   */
  int IsVisible();

  /**
   * Add a point at the end of the curve
   * @param v the vector to add (the vector is copyed)
   */
  int AddPoint(const Vector *v);

  /**
   * Remove the point with id id of the curve and shift back next points
   * actually remove three control points
   * Can only remove green points and not blue points
   * Assume the curve is a good curve, ie the number of points is 3*k+1
   * @param id id of the point to remove
   * @return 1 if the point has been remove successfuly, 0 otherwise
   */
  int RmvPoint(int id);

  /**
   * this method is called when user wants to add a new point
   * on a curve. This method doesn't add new points, but return 
   * a vector. (this method is called when the left button of
   * the mouse is clicked but not released)
   * Algorithm : Compute the closest point P of the curve. Then
   * If P is the first point of the curve, the vector returned is (x,y,0)
   * If P is the last point of the curve, the vector returned is (x,y,0)
   * otherwise, P is returned
   * @param x the x-coordinate of the mouse
   * @param y the y-coordinate of the mouse
   * @return a vector computed as above
   * @see ComputePoint2
   */
  Vector* ComputePoint(float x, float y);

  /**
   * The same function as ComputePoint, except this function actually
   * add the new point (in fact, three control points) in the curve
   * @param x the x-coordinate of the mouse
   * @param y the y-coordinate of the mouse
   * @see ComputePoint
   */
  void ComputePoint2(float x, float y);

  /**
   * Compute the constaints of the curve :
   * when a green point is moved, the two blue points linked to it are translated too
   * when a blue point is moved, the blue one linked to it is translated too
   */
  void AddConstraints();

  /**
   * Translate the whole curve
   * @param v the vector of the move
   */
  void Translate(const Vector *v);

  /**
   * @return the ith point of the curve
   * @param i the indice of the point
   */
  Vector* GetPoint(int i);

  /**
   * @return the number of points in the curve
   */
  int GetNbPoint();

  /**
   * Draw the curve
   */
  void Draw(BEenum mode);

private: 
  /**
   * Compute the point on the curve
   * @param i i is the indice of the subcurve
   * @param t a value between 0 and 1
   * @param v the result of the calculation
   */
  void Curve::Compute(int i,float t,float v[2]);

};

#endif

