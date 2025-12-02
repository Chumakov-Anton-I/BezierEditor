/************************************************************************
* Pierre Morel - 2003
*
* @name BezierEditor.cc
* @memo A Bezier curve Editor. It is possible to define many curves at the
* @meno same time, add new points, new curves, select one or several points
* @meno and move them, load and save in a text file
*
* @author Pierre Morel
* date march 2003
************************************************************************/

#include <stdio.h>
#include <GL/glut.h>
#include "Function.h"
#include "Curve.h"
#include "Vector.h"
#include "World.h"
#include "Selection.h"
#include "TextFile.h"
#include "Color.h"
#include "Constraint.h"

#define RELOAD_ITEM 10 
#define SAVE_ITEM 11
#define RESET_ITEM 12

#define UNDO_ITEM 20
#define COPY_ITEM 21
#define PASTE_ITEM 22
#define ADD_ITEM 23
#define RMV_ITEM 24
#define ADDC_ITEM 25

#define SELECTC_ITEM 30
#define SELECTP_ITEM 31

#define ZIN_ITEM 40
#define ZOUT_ITEM 41
#define ZRST_ITEM 42
#define ZBOX_ITEM 43

#define HELP_ITEM 1
#define QUIT_ITEM 2

GLuint selectBuf[F_MAX_VECTOR];
GLint windW = 500, windH = 500;
GLint vp[4];
World * world=NULL;
Selection * selection;
int mouseactivated=0;
TextFile * textfile;
float zoom=1.0;
float maxX,maxY,minX,minY;
float MoveX=0,MoveY=0;
float moveXi=0,moveYi=0;
float InitX,InitY;
float CursorX,CursorY;
int file_menu, edit_menu, mode_menu, view_menu,visible_menu;
int select_mode=SELECTP_ITEM;
Vector* vectoradd=NULL;
int constraints_activated=1;
int zoom_box=0;


/**
 * This function is called when user modifies the window
 * @param width the new width of the window
 * @param height the new heigh of the window
 */
static void Reshape(int width, int height) {
  windW = width;
  windH = height;
  glViewport(0, 0, windW,windH );
  glGetIntegerv(GL_VIEWPORT, vp);
}

/**
 * Set the zoom and modify the Step value in the Vector Class
 * @param f the new zoom factor
 */
static void SetZoom(float f) {
  zoom=f;
  Vector::SetStep(2/zoom);
}

/**
 * Refresh the visible menu
 * ie, one item for each curve
 */
static void RefreshVisible() {
  int i,j;
  glutSetMenu(visible_menu);
  j=glutGet(GLUT_MENU_NUM_ITEMS);
  for(i=1;i<=j;i++) {
    glutRemoveMenuItem(1);
  }
  for(i=0;i<world->GetNbCurve();i++) {
    char s[50];
    sprintf(s,"x %d",world->GetCurve(i+1)->GetId());
    glutAddMenuEntry(s, 1000+i+1 );
  }
}

/**
 * Some initialisations: essentially concerning init Class
 */
static void EditorInit() {
  if (world!=NULL) {
    delete world;
  }
  world=new World();
  selection=new Selection(world);
  Vector::SetSelection(selection);
  Curve::SetSelection(selection);
  Constraint::SetWorld(world);
  RefreshVisible();
}

/**
 * Compute and change the zoom factor
 * with the new zoom factor, the window show at least the world included
 * from minX to maxX along x-axe and from minY to maxY along y-axe
 * "at least" because this function keeps the scale of the data (it is
 * an orthonormal grid)
 * @param minX
 * @param maxX
 * @param minY
 * @param maxY
 */
void ComputeZoom(float minX,float maxX,float minY,float maxY) {
  float zoom=1;
  float x,y;
  if (maxX==minX) {
    if (maxY!=minY) {
      zoom=windH/(maxY-minY);
    }
  }
  else {
    if (maxY==minY) {
      zoom=windW/(maxX-minX);
    }
    else {
      x=windW/(maxX-minX);
      y=windH/(maxY-minY);
      if (x<y) {
	zoom=x;
      }
      else {
	zoom=y;
      }    
    }
  }
  InitX=0;
  InitY=0;
  MoveX=-0.95*zoom*(minX+maxX)/2;
  MoveY=-0.95*zoom*(minY+maxY)/2;
  SetZoom(zoom*0.95);
}

/**
 * Autoscale chooses the maximum zoom factor to see all data
 * @TODO Autoscale must be applied only on visible curves
 */
void AutoScale() {
  float x,y;
  Vector P1,*P2;
  int i;
  if (world->GetNbVector()>0) {
    P2=world->GetVector(1);
    x=P2->GetX();
    y=P2->GetY();
    minX=x; maxX=x;
    minY=y; maxY=y;
  }
  for(i=1;i<world->GetNbVector();i++) {
    P2=world->GetVector(i+1);
    x=P2->GetX();
    y=P2->GetY();
    if (x>maxX) maxX=x;
    if (y>maxY) maxY=y;
    if (x<minX) minX=x;
    if (y<minY) minY=y;
  }
  ComputeZoom(minX,maxX,minY,maxY);
}

/**
 * change the zoom factor
 */
void ZoomIn() {
  minX=(-windW/2/ZOOM_FACTOR-MoveX)/zoom;
  maxX=(+windW/2/ZOOM_FACTOR-MoveX)/zoom;
  minY=(-windH/2/ZOOM_FACTOR-MoveY)/zoom;
  maxY=(+windH/2/ZOOM_FACTOR-MoveY)/zoom;
  ComputeZoom(minX,maxX,minY,maxY);
  //SetZoom(zoom*1.25);
}

/**
 * change the zoom factor
 */
void ZoomOut() {
  minX=(-windW/2*ZOOM_FACTOR-MoveX)/zoom;
  maxX=(+windW/2*ZOOM_FACTOR-MoveX)/zoom;
  minY=(-windH/2*ZOOM_FACTOR-MoveY)/zoom;
  maxY=(+windH/2*ZOOM_FACTOR-MoveY)/zoom;
  ComputeZoom(minX,maxX,minY,maxY);
  //SetZoom(zoom/1.25);
}

/**
 * display a char string on the screen
 * @param s the char string to display
 * @param x the x-coordinate where to put s
 * @param y the y-coordinate where to put s
 */
static void printg( float x, float y, char *s) {
  int len, i;
  glRasterPos2f(x, y);
  white->GlColor3fv();
  len = (int) strlen(s);
  for (i = 0; i < len; i++) {
    glutBitmapCharacter(GLUT_BITMAP_8_BY_13, s[i]);
  }
}

/**
 * Draw the world, ie every curve and its control points,
 * information (position, number of points, name of curve...)
 * and select or zoom box if there is one.
 * @see DoSelect
 */
static void Draw(void) {
  int i;
  char s[50];
  glPushMatrix();
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(-windW/2, windW/2, -windH/2, windH/2);
  glMatrixMode(GL_MODELVIEW);
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glTranslatef(InitX,InitY,0);
  glTranslatef(MoveX,MoveY,0);
  glScalef(zoom, zoom, zoom);
  for(i=0;i<world->GetNbCurve();i++)
    world->GetCurve(i+1)->Draw(BE_RENDER);
  if (vectoradd!=NULL) {
    vectoradd->Draw(BE_RENDER,white);
  }
  if (mouseactivated==4 || mouseactivated==5) {
    selection->DrawBox();
  }
  glPopMatrix();
  // Status :
  // nb of vector
  sprintf(s,"nbv:%d   ",Vector::NbVector());
  //printg(-windW/2+10,windH/2-62,s);
  // x y
  sprintf(s,"x:%4.2f y:%4.2f     ",CursorX,CursorY);
  printg(-windW/2+10,windH/2-20,s);
  // constraints
  if (constraints_activated==1) {
    sprintf(s,"constraints");
  }
  else {
    sprintf(s,"no constraint");
  }
  printg(-windW/2+10,windH/2-34,s);
  // curve id
  if (select_mode==SELECTC_ITEM) {
    Curve *c =selection->GetCurve(0);
    if (c!=NULL) {
      sprintf(s,"idc:%d   ",c->GetId());
      printg(-windW/2+10,windH/2-48,s);
    }
  }
  glutSwapBuffers();
}


/**
 * Similar to the draw function. This function allows to know
 * where the user point the mouse, on which curve or point.
 * @param x the x-coordinate of the mouse
 * @param y the y-coordinate of the mouse
 * @see DoSelect
 */
static GLint DoSelect(GLint x, GLint y) {
  GLint hits;
  int i;

  glSelectBuffer(F_MAX_VECTOR, selectBuf);
  glRenderMode(GL_SELECT);
  glInitNames();
  glPushName(~0);

  glPushMatrix();

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPickMatrix(x, windH - y, 4, 4, vp);
  gluOrtho2D(-windW/2,windW/2, -windH/2, windH/2);
  glMatrixMode(GL_MODELVIEW);

  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glTranslatef(InitX,InitY,0);
  glTranslatef(MoveX,MoveY,0);

  glScalef(zoom, zoom, zoom);
  
  for(i=0;i<world->GetNbCurve();i++) {
    if (select_mode==SELECTP_ITEM) {
      world->GetCurve(i+1)->Draw(BE_SELECT_POINT);
    }
    else {
      world->GetCurve(i+1)->Draw(BE_SELECT_CURVE);
    }
  }
  glPopMatrix();

  hits = glRenderMode(GL_RENDER);
  if (hits <= 0) {
    return -1;
  }
  for(i=0;i<hits;i++) {
    if (selectBuf[4*i+3])
      return selectBuf[4*i+3];
  }
  return -1;
}

/**
 * Add a new point to the select curve or to the
 * first curve of the world
 * @TODO if no curve is selected or if we aren't in select curve mode
 * @TODO the point must be added to the lastest curve the user has selected
 * @TODO or the curve contained the latest point the user has selected
 * @see addpoint2
 */
void addpoint(float x,float y) {
  Curve *c;
  if (select_mode==SELECTC_ITEM) {
    c=selection->GetCurve(0);
  }
  else {
    c=world->GetCurve(1);
  }
  if (vectoradd!=NULL) {
    delete vectoradd; }
  vectoradd=c->ComputePoint(x,y);
}

/**
 * Add definitely the point 
 * @see addpoint
 */
void addpoint2(float x,float y) {
  Curve *c;
  if (select_mode==SELECTC_ITEM) {
    c=selection->GetCurve(0);
  }
  else {
    c=world->GetCurve(1);
  }
  if (vectoradd!=NULL) {
    delete vectoradd; }
  vectoradd=NULL;
  c->ComputePoint2(x,y);
}

/**
 * Call back function when the mouse is moving in the passive mode (no button is hold)
 * @TODO This call back is expensive because it implies drawing the world but
 * @TODO it is necessary if user wants to know the position of the mouse (CursorX and CursorY)
 * @see glutPassiveMotionFunc
 */
void mouseMove2(int x,int y) {
  CursorX=(x-windW/2-MoveX)/zoom;
  CursorY=(windH/2 -y-MoveY)/zoom;
  glutPostRedisplay();
}

/**
 * Call back function when the mouse is moving in the active mode (one or more button are pressed)
 * @TODO change mouseactivated variable to something more appropriate
 * @see glutMotionFunc
 */
void mouseMove(int x,int y) {
  Vector *v;
  v=new Vector((x-windW/2-MoveX)/zoom,(windH/2 -y-MoveY)/zoom,0);
  if (mouseactivated==1) {
    selection->FixEnd(v);
    if (constraints_activated==1) {
      world->ApplyConstraints(); 
    }
  }
  else if (mouseactivated==2) {
    MoveX=x-moveXi;
    MoveY=windH-y-moveYi;
  }
  else if (mouseactivated==3) {
    addpoint(v->GetX(),v->GetY());
  }
  else if (mouseactivated==4) {
    selection->SetBoxEnd(v);
  }
  else if (mouseactivated==5) {
    selection->SetBoxEnd(v);
  }
  CursorX=v->GetX();
  CursorY=v->GetY();
  delete v;
  glutPostRedisplay();
}

/**
 * This function is called when a button of the mouse is pressed
 * @param button the button which was pressed
 * @param state if the button is pressed or released
 * @param mouseX the x-coordinate of the mouse
 * @param mouseY the y-coordinate of the mouse
 */
static void Mouse(int button, int state, int mouseX, int mouseY) {
  GLint hit;
  Vector *v;
  v=new Vector((mouseX-windW/2-MoveX)/zoom,(windH/2 -mouseY-MoveY)/zoom,0);
  if (state == GLUT_DOWN) {
    if (zoom_box==1) {
      mouseactivated=5;
      selection->SetBoxStart(v);
    }
    else {
      hit = DoSelect((GLint) mouseX, (GLint) mouseY);
      if (hit != -1) {
	if (button == GLUT_LEFT_BUTTON) {
	  int m=glutGetModifiers();
	  if (m==GLUT_ACTIVE_SHIFT) {
	  }
	  else if (m==GLUT_ACTIVE_CTRL) {
	    if (select_mode==SELECTC_ITEM) {
	      selection->AddCurve(hit);
	    }
	    else {
	      selection->AddVector(hit);
	    }
	    selection->FixStart(v);
	    mouseactivated=1;
	  }
	  else if (m==GLUT_ACTIVE_SHIFT) {
	    if (select_mode==SELECTC_ITEM) {
	      if (selection->GetNbCurve()>0) {
		mouseactivated=3;
		addpoint(v->GetX(),v->GetY());
	      }
	    }
	  }
	  else {
	    if (select_mode==SELECTC_ITEM) {
	      selection->SelectMode(SECURVE);
	      selection->AddCurve(hit);
	    }
	    else {
	      selection->SelectMode(SEVECTOR);
	      selection->AddVector(hit);
	    }
	    selection->FixStart(v);
	    mouseactivated=1;
	  }
	} 
	else if (button == GLUT_MIDDLE_BUTTON) {
	} 
	else if (button == GLUT_RIGHT_BUTTON) {
	}
      }
      else {
	if (button == GLUT_LEFT_BUTTON) {
	  int m=glutGetModifiers();
	  if (m==GLUT_ACTIVE_SHIFT) {
	    if (select_mode==SELECTC_ITEM) {
	      if (selection->GetNbCurve()>0) {
		mouseactivated=3;
		addpoint(v->GetX(),v->GetY());
	      }
	    }
	  }
	  else if (m==GLUT_ACTIVE_CTRL) {
	    mouseactivated=4;
	    selection->SetBoxStart(v);
	  }
	  else {
	    mouseactivated=2;
	    moveXi=-MoveX+mouseX;
	    moveYi=-MoveY+windH-mouseY;
	  }
	}
      }
    }
  }
  else if (state == GLUT_UP) {
    if (mouseactivated==1) {
      selection->Move(v);
    }
    else if (mouseactivated==3) {
      addpoint2(v->GetX(),v->GetY());
      glutPostRedisplay();
    }
    else if (mouseactivated==4) {
      int i,j;
      selection->SetBoxEnd(v);
      if (select_mode==SELECTP_ITEM) {
	for(i=0;i<world->GetNbCurve();i++) {
	  Curve *c=world->GetCurve(i+1);
	  if (c->IsVisible()==1) {
	    for(j=0;j<c->GetNbPoint();j++) {
	      if (selection->IsBoxIncluded(c->GetPoint(j))==1) {
		selection->AddVector(c->GetPoint(j)->GetId());
	      }
	    }
	  }
	}
      }
    }
    else if (mouseactivated==5) {
      float minX,minY,maxX,maxY;
      selection->SetBoxEnd(v);
      minX=min(selection->GetBoxStart()->GetX(),selection->GetBoxEnd()->GetX());
      maxX=max(selection->GetBoxStart()->GetX(),selection->GetBoxEnd()->GetX());
      minY=min(selection->GetBoxStart()->GetY(),selection->GetBoxEnd()->GetY());
      maxY=max(selection->GetBoxStart()->GetY(),selection->GetBoxEnd()->GetY());
      ComputeZoom(minX,maxX,minY,maxY);
      zoom_box=0;
    }
    mouseactivated=0;
  }
  delete v;
  glutPostRedisplay();
}

/**
 * save the world in a file
 * save it in the outfile if available ( -o or --output= option)
 * or in the inputfile if available (-i or --input= option)
 */
static void save() {
    if (fileo!=NULL) {
      textfile=new TextFile(fileo);
      textfile->Write(world);
      delete textfile;
    }
    else if (filei!=NULL) {
      textfile=new TextFile(filei);
      textfile->Write(world);
      delete textfile;
    }
}


/**
 * load curves from the input file
 */
static void load() {
    if (filei!=NULL) {
      int i;
      textfile=new TextFile(filei);
      textfile->Read(world);
      for(i=0;i<world->GetNbCurve();i++) {
	world->GetCurve(i+1)->AddConstraints();
      }
      RefreshVisible();
      AutoScale();
      delete textfile;
    }
}

/**
 * add a curve in the world
 */
static void addCurve() {
  Curve * curve;
  Vector * V;
  curve=new Curve(world);
  world->AddCurve(curve);
  V=new Vector(0,0,0);
  curve->AddPoint(V);
  V=new Vector(1,0,0);
  curve->AddPoint(V);
  V=new Vector(2,1,0);
  curve->AddPoint(V);
  V=new Vector(3,1,0);
  curve->AddPoint(V);
  curve->AddConstraints();
  AutoScale();
  RefreshVisible();
}

static void removeSelection() {
  if (select_mode==SELECTC_ITEM) {
    while (selection->GetNbCurve()>0) {
      int id;
      id=selection->GetCurve(0)->GetId();
      selection->RmvCurve(id);
      world->RmvCurve(id);
    }
  }
  else if (select_mode==SELECTP_ITEM) {
    while (selection->GetNbVector()>0) {    
      int id;
      int i;
      id=selection->GetVector(0)->GetId();
      selection->RmvVector(id);
      for(i=0;i<world->GetNbCurve();i++) {
	if (world->GetCurve(i+1)->RmvPoint(id)==1) {
	  break;
	}
      }
    }
  }
  RefreshVisible();
}

/**
 * a simple help printed on the standart output
 */
static void help() {
    printf("---------------------------------------------------------------------\n");
    printf("|                 Help                                              |\n");
    printf("---------------------------------------------------------------------\n");
    printf("                                                                     \n");
    printf("Usage                                                                \n");
    printf("-----                                                                \n");
    printf("> BezierEditor         (can create curves but can't save them!!)     \n");
    printf("> BezierEditor -i file (read from file and save in file)             \n");
    printf("> BezierEditor -i file1 -o file2                                     \n");
    printf("                       (read from file1 and save in file2)           \n");
    printf("> BezierEditor -h      (print this help)                             \n");
    printf("                                                                     \n");
    printf("                                                                     \n");
    printf("File                                                                 \n");
    printf("----                                                                 \n");
    printf("'Q'/'ESC' : quit                                                     \n");
    printf("'S'       : save                                                     \n");
    printf("'R'       : reset all                                                \n");
    printf("'L'       : reload the input file                                    \n");
    printf("'H'       : print this help                                          \n");
    printf("                                                                     \n");
    printf("Edit                                                                 \n");
    printf("----                                                                 \n");
    printf("'CTRL'    : select more than one object                              \n");
    printf("'SHIFT'   : add a new point to the selected curve (in mode curve)    \n");
    printf("'A'       : add a new curve                                          \n");
    printf("'C'/'P'   : curve/point mode                                         \n");
    printf("'D'       : constraints or no constraints mode                       \n");
    printf("DEL/SUPPR : remove the selection                                     \n");
    printf("                                                                     \n");
    printf("Zoom                                                                 \n");
    printf("----                                                                 \n");
    printf("'Z'       : zoom box                                                 \n");
    printf("PAGE UP   : zoom in                                                  \n");
    printf("PAGE DOWN : zoom out                                                 \n");
    printf("HOME      : autoscale                                                \n");
    printf("                                                                     \n");
    printf("                                                                     \n");
    printf("                                                                     \n");
    printf("                                                                     \n");
    printf("                                                                     \n");
    printf("                                                                     \n");
    printf("                                                                     \n");
}

/**
 * call back function when a key is pressed
 * @param key the ascii code of the key
 * @param x the x-coordinate of the mouse when the key was pressed
 * @param y the y-coordinate of the mouse when the key was pressed
 */
static void keyFunc(unsigned char key,int x, int y) {
  switch(key) {
  case 27: case 'q': case 'Q':
    // quit
    exit(0);
    break;
  case 8: case 127: // drop and delete
    removeSelection();
    break;
  case 's': case 'S':
    //save
    save();
    break;
  case 'a': case 'A':
    //add a curve
    addCurve();
    break;
  case 'r': case 'R':
    //reset
    EditorInit();
    break;
  case 'l': case 'L':
    //(re)load
    EditorInit();
    load();
    break;
  case 'c': case 'C':
    select_mode=SELECTC_ITEM;
    break;
  case 'p': case 'P':
    select_mode=SELECTP_ITEM;
    break;
  case 'h': case 'H':
    help();
    break; 
  case 'd': case 'D':
    constraints_activated=1-constraints_activated;
    break;
  case 'z': case 'Z':
    zoom_box=1;
    break;
  }
  glutPostRedisplay();
}

/**
 * Call back function when an item is selected in the menu
 * @param item the item the user selected
 */
static void menuFunc(int item) {
  switch(item) {
  case RESET_ITEM:
    EditorInit();
    break; 
  case RELOAD_ITEM: 
    EditorInit();
    load();
    break; 
  case SAVE_ITEM:
    save();
    break; 
  case UNDO_ITEM:
    printf("undo is not yet available, sorry\n");
    break; 
  case COPY_ITEM:
    printf("copy is not yet available, sorry\n");
    break; 
  case PASTE_ITEM:
    printf("paste is not yet available, sorry\n");
    break;
  case ADD_ITEM:
    printf("add point is not yet available, sorry\n");
    break; 
  case RMV_ITEM:
    // remove the curves in the selection
    removeSelection();
    break;
  case ADDC_ITEM: 
    addCurve();
    break;
  case SELECTC_ITEM:
    select_mode=SELECTC_ITEM;
    break;
  case SELECTP_ITEM:
    select_mode=SELECTP_ITEM;
    break;
  case ZIN_ITEM:
    ZoomIn();
    break; 
  case ZOUT_ITEM:
    ZoomOut();
    break;
  case ZRST_ITEM:
    AutoScale();
    break; 
  case ZBOX_ITEM:
    zoom_box=1;
    break; 
  case HELP_ITEM:
    help();
    break; 
  case QUIT_ITEM:
    // TODO quitter proprement (demander une confirmation de sauvegarde du fichier)
    exit(0);
    break;
  default:
    if (item>1000 && item<1000+F_MAX_CURVE) {
      char s[50];
      Curve *c=world->GetCurve(item-1000);
      int i=c->GetId();
      if (c->IsVisible()) {
	c->InVisible();
	sprintf(s,"  %d",i);
      }
      else {
	c->Visible();
	sprintf(s,"x %d",i);
      }
      glutSetMenu(visible_menu);
      glutChangeToMenuEntry(i, s, item);
    }
    break;
  }
}

/**
 * this function is called when the user presses a special key, 
 * a key without any ascii code
 * @param key the key code
 * @param x the x-coordinate of the mouse when the key was pressed
 * @param y the y-coordinate of the mouse when the key was pressed
 */
static void keySpecialFunc(int key,int x,int y) {
  switch(key) {
  case GLUT_KEY_PAGE_UP: 
    ZoomIn();
    break; 
  case GLUT_KEY_PAGE_DOWN:
    ZoomOut();
    break;
  case GLUT_KEY_HOME:
    AutoScale();
    break; 
  }
  glutPostRedisplay();
}

/**
 * Create the menu structure
 */
void makeMenu()
{
	file_menu = glutCreateMenu( menuFunc );
	glutAddMenuEntry( "reset  ", RESET_ITEM );
	glutAddMenuEntry( "reload  ", RELOAD_ITEM );
	glutAddMenuEntry( "save  ", SAVE_ITEM );

	edit_menu = glutCreateMenu( menuFunc );
	//glutAddMenuEntry( "undo  ", UNDO_ITEM );
	//glutAddMenuEntry( "copy  ", COPY_ITEM );
	//glutAddMenuEntry( "paste  ", PASTE_ITEM );
	//glutAddMenuEntry( "add point  ", ADD_ITEM );
	glutAddMenuEntry( "remove selection  ", RMV_ITEM );
	glutAddMenuEntry( "add curve  ", ADDC_ITEM );

	mode_menu = glutCreateMenu( menuFunc );
	glutAddMenuEntry( "select curve  ", SELECTC_ITEM );
	glutAddMenuEntry( "select point  ", SELECTP_ITEM );

	view_menu = glutCreateMenu( menuFunc );
	glutAddMenuEntry( "zoom in  ", ZIN_ITEM );
	glutAddMenuEntry( "zoom out ", ZOUT_ITEM );
	glutAddMenuEntry( "zoom reset  ", ZRST_ITEM );
	glutAddMenuEntry( "zoom box  ", ZBOX_ITEM );

	visible_menu = glutCreateMenu( menuFunc );

	glutCreateMenu( menuFunc );
	glutAddMenuEntry( "Help    ", HELP_ITEM );
	glutAddSubMenu(   "File... ", file_menu );
	glutAddSubMenu(   "Edit... ", edit_menu );
	glutAddSubMenu(   "Mode... ", mode_menu );
	glutAddSubMenu(   "View... ", view_menu );
	glutAddSubMenu(   "Visible... ", visible_menu );
	glutAddMenuEntry( "Quit  ", QUIT_ITEM );

	glutAttachMenu( GLUT_RIGHT_BUTTON );
}


/**
 * the main function
 * runs the init functions and starts the mainloop of glut
 */
int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  glutInitWindowSize(windW,windH);
  glutCreateWindow("Bezier Editor");
  get_param(argc,argv);
  if (usage==1) {
    help();
    exit(0);
  }
  makeMenu();
  EditorInit();
  load();
  glutReshapeFunc(Reshape);
  glutKeyboardFunc(keyFunc);
  glutSpecialFunc(keySpecialFunc);    
  glutMotionFunc(mouseMove);
  glutPassiveMotionFunc(mouseMove2);
  glutMouseFunc(Mouse);
  glutDisplayFunc(Draw);
  glutMainLoop();
  return 0;
}

/*
BUGS :
-----

- sur trois points consécutifs P1,P2 et P3. Si P2 et P3 sont sélectionnés et qu'on les bouge, les deux contraintes sont appliquées, le résultat est moche

- quand on fait un reset, le menu "visible" n'est pas rafaraichi

TODO :
------

-Pouvoir supprimer des points des courbe.

-changer la résolution (la constante STEP) avec le zoom, le rayon de courbure

-sélectionner une courbe et mettre le autres en arrière plan

-importer une courbe à partir du longue série de points (et éliminer ceux qui sont inutiles)

-charger/sauvegarder à partir d'un fichier xml (permet de nommer les courbes/points, de leur assigner des couleurs ... rajouter d'autre contraintes)

-exporter en (e)ps.

-magnétisme (sur la grille, sur les points)

-rajouter des axes

-étirement local du temps

-copier/coller, cycle

-undo (assez difficile je pense)

-ajouter un identifiant pour les contraintes

-toujours de la perte de vecteurs (augmente au moment ou l'on relache la souris
quand on déplace un point)

*/
