/************************************************************************
* Pierre Morel - 2003
*
* @name Function.h
* @memo a small toolkit
*
* @author Pierre Morel
* date march 2003
************************************************************************/

#ifndef FUNCTION
#define FUNCTION

extern char * filei;
extern char * fileo;
extern int usage;

/**
 * extract information from argv parameters
 * for the moment, only two options :
 * -i file or --input=file, the file input
 * -o file or --output=file, the file output
 * @param argc the number of arguments
 * @param argv the list of all the arguments
 */
void get_param(int argc, char **argv);

#define F_MAX_VECTOR 10000
#define F_MAX_CURVE 100
#define F_MAX_SELECTION 1
#define F_MAX_CONSTRAINT 100000
#define ZOOM_FACTOR 2

enum BEenum {
  BE_RENDER,
  BE_SELECT_CURVE,
  BE_SELECT_POINT
};

/**
 * @return the maximim of two values
 * @param a the first value to compare
 * @param b the second value to compare
 */
float max(float a,float b);

/**
 * @return the minimum of two values
 * @param a the first value to compare
 * @param b the second value to compare
 */
float min(float a,float b);

/**
 * compute the square of a float
 * @param a the number
 * @return the result
 */
float sqr(float a);

#endif
