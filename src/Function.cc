/************************************************************************
* Pierre Morel - 2003
*
* @name Function.cc
* @memo a small toolkit
*
* @author Pierre Morel
* date march 2003
************************************************************************/

#include "Function.h"
#include <getopt.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char *filei=NULL, *fileo=NULL;
int usage=0;

float sqr(float a) {
  return a*a;
}

void get_param(int argc,char ** argv) {
  int c;
  printf("\n\noption(s) :\n------------\n");
  while (1) {
    int option_index = 0;
    static struct option long_options[] = {
      {"input", 1, 0, 'i'},
      {"output", 2, 0, 'o'},
      {"help", 0, 0, 'h'},
    };

    c = getopt_long (argc, argv, "i:o:h",
		     long_options, &option_index);
    if (c == -1)
      break;

    switch (c) {
    case 0:
      printf ("option %s", long_options[option_index].name);
      if (optarg)
	printf (" with arg %s", optarg);
      printf ("\n");
      break;
    case 'h':
      usage=1;
      break;
    case 'i':
      printf ("   load Bezier curve from file : %s\n",optarg);
      filei=(char*)malloc(100);
      strcpy(filei,optarg);
      break;
    case 'o':
      printf ("   save Bezier curve to file : %s\n",optarg);
      fileo=(char*)malloc(100);
      strcpy(fileo,optarg);
      break;
    default:
      printf ("?? getopt returned character code 0%o ??\n", c);
    }
  }
  if (optind < argc) {
    printf ("non-option ARGV-elements: ");
    while (optind < argc)
      printf ("%s ", argv[optind++]);
    printf ("\n");
  }
  printf("\n\n\n");
}


float max(float a,float b) {
  if (a<b)
    return b;
  else
    return a;
}

float min(float a,float b) {
  if (a>b)
    return b;
  else
    return a;
}
