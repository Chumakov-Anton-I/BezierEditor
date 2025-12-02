/************************************************************************
* Pierre Morel - 2003
*
* @name TextFile.h
* Read and write data in a simple text file
*
* @author Pierre Morel
* @date march 2003
************************************************************************/

#ifndef TEXTFILE
#define TEXTFILE

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Vector.h"
#include "Curve.h"
#include "World.h"

/**
 * @name TextFile
 * Handles text file to save and load data 
 */
class TextFile {
 protected:
  char * input;
 public:
  TextFile(char * input);

  /**
   * read the text file and create as many curves as there are in the file
   * each curve begins with this char "C" 
   * comments begin with this char "#"
   * each other lines describe control points with two floats
   * @param world the world in which we add curves
   * @see Write
   */
  int Read(World * world);

  /**
   * write the world into the file
   * @param world the world from which we read curves
   * @see Read
   */
  int Write(World * world);

};

#endif

