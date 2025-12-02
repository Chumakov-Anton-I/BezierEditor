/************************************************************************
* Pierre Morel - 2003
*
* @name Selection.h
* contains a subset of curves and points selected by the user
* Allows to manipulate (for instance to move) several points or
* curves at the same time
*
* @author Pierre Morel
* @date march 2003
************************************************************************/

#ifndef SELECTION
#define SELECTION

#include "Function.h"

#define S_MAX_VECTOR F_MAX_VECTOR
#define S_MAX_CURVE F_MAX_CURVE

enum SEnum {
  SEVECTOR,
  SECURVE };

class Curve;
class Vector;
class World;

/**
 * @name Selection
 * Handle a selection, ie manipulate several points or curves to move them
 * at the same time.
 * @see Curve
 * @see Vector
 * @see World
 */
class Selection {
protected:
  Vector* vectorlist[F_MAX_VECTOR];
  int nbvector;

  Curve* curvelist[F_MAX_CURVE];
  int nbcurve;

  int mode;

  Vector *start,*translation;

  Vector *box1,*box2;

  World * world;

  void Init(World * world);

public:
  /**
   * create a new selection
   * @param world the world to which the selection is attached
   */
  Selection(World * world);

  /**
   * set the world of the selection
   * @param world the world to which the selection is attached
   */
  void SetWorld(World * world);

  /**
   * there are two mode in a selection : SE_VECTOR and SE_CURVE
   * In SE_VECTOR mode, the selection contains vector !!
   * In SE_CURVE mode, the selection contains curves !!
   * @param s the new mode 
   */
  void SelectMode(SEnum s);

  /**
   * Add a vector (identified with its ID) to the selection
   * if the mode was SE_CURVE, then it will be changed to SE_VECTOR
   * @param id the id of the vector we add
   */
  void AddVector(int id);

  /**
   * @return ith vector in the selection 
   * @return NULL if there is no ith vector in the selection 
   * @param i indice of the vector we want to extract
   */
  Vector * GetVector(int i);

  /**
   * @return the number of vectors in the selection
   */
  int GetNbVector();

  /**
   * remove a vector from a selection. The vector itself is not deleted
   * @param id the id of the vector to remove
   */
  int RmvVector(int id);

  /**
   * add a curve (identified with its id) to the selection
   * if the mode was SE_VECTOR, then it will be changed to SE_CURVE 
   * @param id the id of the vector to add
   */
  void AddCurve(int id);

  /**
   * @return NULL if the selection doesn't contain the ith curve
   * @return the ith curve 
   * @param i indice of the curve in the selection
   */
  Curve * GetCurve(int i);

  /**
   * @return the number of curves in the selection
   */
  int GetNbCurve();

  /**
   * remove a curve from the selection. The curve is not deleted
   * @param id the id of the curve to remove
   */
  int RmvCurve(int id);

  /**
   * A selection can be move. This method fix the origin of this translation
   * @param v the origin of the translation
   * @see FixEnd
   */
  void FixStart(Vector*v);

  /**
   * this method set the end of this translation. The translate vector is given
   * with the substraction of The End vector by the Start vector
   * @param v the end of the translation
   * @see FixStart
   * @see Move
   */
  void FixEnd(Vector*v);

  /**
   * @return the translation vector
   */
  Vector* GetTranslation();

  /**
   * Apply the translation to each point or curve contained in the selection
   * @param v fix the end of the translation
   * @see FixEnd
   */
  void Move(Vector*v);

  /**
   * define a box with two points. Used with the zoom box function and multiple
   * selection
   * @param v the first vector of the box
   */
  void SetBoxStart(Vector *v);

  /**
   * @return the first vector of the box (always defined)
   */
  Vector* GetBoxStart();

  /**
   * define a box with two points. Used with the zoom box function and multiple
   * selection
   * @param v the second vector of the box
   */
  void SetBoxEnd(Vector *v);

  /**
   * @return the second vector of the box (always defined)
   */
  Vector* GetBoxEnd();

  /**
   * @return 1 if the vector is included in the box
   * @return 0 otherwise
   * @param v the vector to test
   * @see SetBoxStart
   * @see SetBoxEnd
   */
  int IsBoxIncluded(Vector *v);

  /**
   * Draw the box (a simple rectangle in white)
   */
  void DrawBox();
};

#endif
