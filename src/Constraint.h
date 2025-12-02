/************************************************************************
* Pierre Morel - 2003
*
* @name Constraint.h
* Defines some constraints between points (for instance, to translate 
* several points when user translates only one)
*
* @author Pierre Morel
* date march 2003
************************************************************************/

#ifndef CONSTRAINT
#define CONSTRAINT

#include "Function.h"
#include "Vector.h"
#include "World.h"

#define S_MAX_CONSTRAINT F_MAX_CONSTRAINT

class Vector;
class World;

enum COenum {
  CO_TRANSLATION,
  CO_HOMOTHETY
};

/**
 * @name Constraint
 * Defines some constraints between points (for instance, to translate 
 * several points when user translates only one)
 * @TODO C++ is an object langage. So why there are no derivated classes
 * @TODO to implement translation and homothety.
 */
class Constraint {
protected:
  Vector *A,*B;
  float alpha,beta;
  Vector *O;
  int mode;
  static World * world;

  /**
   * Refresh the alpha and beta value in the homothety mode
   * @see refresh
   */
  void Init();

public:
  /**
   * Set the world for every constraints
   * @TODO this attribute is not really used ? to remove it
   */
  static void SetWorld(World * world);

  Constraint();

  ~Constraint();

  /**
   * a constructor for a translation
   */
  Constraint(Vector *A,Vector *B);

  /**
   * a constructor for an homothety
   */
  Constraint(Vector *A,Vector *B,Vector *O);

  /**
   * The constraint is a translation
   * @param V the translation
   */
  void SetTranslation(Vector*V);

  /**
   * The constraint is an homothety
   * @param V the centre of the honothety
   * @param angle the angle of the homothety
   * @param scale the scale of the homothety
   * @see Vector::Homothety
   */
  void SetHomothety(Vector*V,float angle,float scale);

  /**
   * Refresh the constraint, ie the angle, the scale for the homothety
   * and O for the translation
   */
  void Refresh();

  /**
   * @return the A vector
   */
  Vector* GetVectorA();

  /**
   * @return the B vector
   */
  Vector* GetVectorB();

  /**
   * @return the O vector
   */
  Vector* GetVectorO();

  /**
   * Apply the constraint to the vector v
   * the contraint is only apply if v and A are the same (the id must be the same)
   * @param v the vector to constraint (so it is the vector which can move)
   */
  void ApplyTransform(Vector* v);
  
};

#endif
