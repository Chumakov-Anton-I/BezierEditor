/************************************************************************
* Pierre Morel - 2003
*
* @name World.h
* contains curves, points and constraints of the work space
*
* @author Pierre Morel
* @date march 2003
************************************************************************/

#ifndef WORLD
#define WORLD

#include "Function.h"

#define W_MAX_VECTOR F_MAX_VECTOR
#define W_MAX_CURVE F_MAX_CURVE
#define W_MAX_SELECTION F_MAX_SELECTION
#define W_MAX_CONSTRAINT F_MAX_CONSTRAINT

class Curve;
class Vector;
class Constraint;

/**
 *
 * @name World 
 * @doc contains curves, points and constraints of the work space
 * there are probably only one world int the application but, we can
 * imagine opening more than one file and so having more than one work space
 *
 * @see Vector
 * @see Curve
 * @see Constraint
 */
class World {

protected:
  Vector* vectorlist[W_MAX_VECTOR];
  int nbvector;

  Curve* curvelist[W_MAX_CURVE];
  int nbcurve;

  Constraint* constraintlist[W_MAX_CONSTRAINT];
  int nbconstraint;

  /**
   * reset the three integers 
   */
  void Init();

public:
  World();

  /**
   *
   * Add Vector in the workspace
   * @doc Add a vector in the workspace and Set its Id (the id
   * @doc of the vector become the indice+1 in the array)
   * @param v the vector to add
   * @see GetVector
   * @see GetNbVector
   */
  void AddVector(Vector * v);

  /**
   *
   * return the vector with the id specified in parameter
   * @param id the vector id
   * @return the vector if it is a correct id
   * @return NULL if no vector with the specified id exists
   * @see AddVector
   * @see GetNbVector
   */
  Vector* GetVector(const int id);

  /**
   * remove a vector from the world. Becareful, this method
   * change the id of the other vectors !!
   * Actually the id of each vector is the indice in an array
   * that's why delete a vector changes id of other vectors
   * (only the last one)
   * @param id the id of the vector to delete
   * @return 1 if the vector was in the world 
   * @return 0 otherwise
   */
  int RmvVector(const int id);


  /**
   *
   * return the number of Vector in the workspace
   * @return the number of Vector
   * @see GetVector
   * @see AddVector
   */
  int GetNbVector();

  /**
   *
   * Add a curve in the workspace
   * @doc only copy a reference of this curve and Set Its ID
   * @param c the curve
   * @see GetCurve
   * @see GetNbCurve
   */
  void AddCurve(Curve * c);

  /**
   * remove a curve from the world. Becareful, this method
   * change the id of the other curvess !!
   * @param id the id of the curve to delete
   * @return 1 if the curve was in the world 
   * @return 0 otherwise
   */
  int RmvCurve(const int id);

  /**
   *
   * return the curve with the specified ID
   * @param id the curve id (it must be between 1 and getNbCurve())
   * @return the vector if it is a correct id
   * @return NULL if no curve with the specified id exists
   * @see AddCurve
   * @see GetNbCurve
   */
  Curve* GetCurve(const int id);


  /**
   *
   * return the number of Curves in the workspace
   * @return the number of Curves
   * @see AddCurve
   * @see GetCurve
   */
  int GetNbCurve();

  /**
   *
   * Add a constraint in the workspace
   * @doc only copy a reference of this constraint
   * @param c the constraint to add
   * @see GetConstraint
   * @see GetNbConstraint
   * @see ApplyConstraints
   * @see SearchConstraint
   * @see RefreshConstraints
   */
  void AddConstraint(Constraint * c);

  /**
   * return the constraint with the specified ID
   * @param id the constraint id (it must be between 1 and getNbConstraint())
   * @return the constraint if it is a correct id
   * @return NULL if there is no constraint with the specified id
   * @see AddConstraint
   * @see GetNbConstraint
   * @see ApplyConstraints
   * @see SearchConstraint
   * @see RefreshConstraints
   */
  Constraint* GetConstraint(const int id);

  /**
   *
   * return the number of Constraints in the workspace
   * @return the number of Constraints
   * @see AddConstraint
   * @see GetConstraint
   * @see ApplyConstraints
   * @see SearchConstraint
   * @see RefreshConstraints
   */
  int GetNbConstraint();

  /**
   *
   * Apply all the constraints of the workspace
   * @see AddConstraint
   * @see GetConstraint
   * @see GetNbConstraint
   * @see SearchConstraint
   * @see RefreshConstraints
   * @see Constraint::ApplyTransform
   */
  void ApplyConstraints();

  /**
   *
   * search a constraint in the workspace
   * @doc return the first constraints which match with the two
   * @doc parameters
   * @param v1 the first vector of the constraint
   * @param v2 the second vector of the constraint
   * @see AddConstraint
   * @see GetConstraint
   * @see GetNbConstraints
   * @see ApplyConstraints
   * @see RefreshConstraints
   */
  Constraint* SearchConstraint(Vector *v1,Vector *v2);

  /**
   *
   * refresh all the constraints
   * @see AddConstraint
   * @see GetConstraint
   * @see GetNbConstraints
   * @see ApplyConstraints
   * @see SearchConstraints
   * @see Constraint::Refresh
   */
  void RefreshConstraints();

  /**
   *
   * Remove all the constraints where v appears
   * @param v the vector 
   */
  void RemoveConstraint(Vector *v);


};

#endif
