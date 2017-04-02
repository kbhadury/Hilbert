#include "hilbert.h"
#include <iostream>
#include <cmath>

/***** CONTENTS *****\


   Public methods
   Color and drawing
   Parsing
   OpenGL functions


\*******************/


//Part of the member function workaround (see the .h file for details)
Hilbert* Hilbert::curObj = 0;

//Vars for changing the color
float color;
bool up;

//Var for scene rotation
int deg = 0;


/***** Public methods *****/

//Constructor, describes L-system
Hilbert::Hilbert(string start, string vars, string* rules, float ang, float side)
{
  consts = "F+-&^\\/|[]`~$"; //Characters that we recognize as constants
  debug = false;

  startStr = start;
  varsStr = vars;
  rulesArr = rules;
  angle = ang;
  sideLen = side;

  level = 0;
  userShape = CUBE; //Default shape
}

//Enable drawing a coordinate system and printing the rotation angle
void Hilbert::enableDebug(bool enable)
{
  debug = enable;
}

//Allow user to specify the shape used in constructing the L-system
void Hilbert::setShape(int shape)
{
  if(shape < 0 || shape > 2)
  {
    userShape = CUBE; //Default to cube
    cout << "Shape must be Hilbert.CUBE, Hilbert.PYRAMID, or Hilbert.CYLINDER" << endl;
    cout << "Defaulting to cube" << endl;
  }
  else
  {
    userShape = shape;
  }
}

//Public-facing draw function, calls private gl_main function
void Hilbert::draw(int argc, char** argv, int lvl)
{
  level = lvl;
  gl_main(argc, argv);
}


/***** Color and drawing *****/

//Draw a pyramid of the given height (height is equal to base length)
//When centered at the origin, it points along the positive y-axis
void Hilbert::drawPyra(float height)
{
  //Vertices
  GLfloat vertexA[] = {-height/2, -height/2, -height/2};
  GLfloat vertexB[] = {height/2, -height/2, -height/2};
  GLfloat vertexC[] = {height/2, -height/2, height/2};
  GLfloat vertexD[] = {-height/2, -height/2, height/2};
  GLfloat vertexE[] = {0, height/2, 0};

  //Base
  glBegin(GL_QUADS);
    glNormal3f(0, -1, 0);
    glVertex3fv(vertexA);
    glVertex3fv(vertexB);
    glVertex3fv(vertexC);
    glVertex3fv(vertexD);
  glEnd();

  //Sides
  glBegin(GL_TRIANGLES);
    glNormal3f(-1.414214, 1, 0);
    glVertex3fv(vertexA);
    glVertex3fv(vertexD);
    glVertex3fv(vertexE);
  glEnd();

  glBegin(GL_TRIANGLES);
    glNormal3f(0, 1, 1.414214);
    glVertex3fv(vertexD);
    glVertex3fv(vertexC);
    glVertex3fv(vertexE);
  glEnd();

  glBegin(GL_TRIANGLES);
    glNormal3f(1.414214, 1, 0);
    glVertex3fv(vertexC);
    glVertex3fv(vertexB);
    glVertex3fv(vertexE);
  glEnd();

  glBegin(GL_TRIANGLES);
    glNormal3f(0, 1, -1.414214);
    glVertex3fv(vertexB);
    glVertex3fv(vertexA);
    glVertex3fv(vertexE);
  glEnd();
}

//Draw a cylinder with diameter and height equal to len
//Sides are constructed with 10 faces
//Centered at the origin with the z-axis passing through the circular faces
void Hilbert::drawCyl(float len)
{
  GLfloat bottomFace[3*10];
  GLfloat topFace[3*10];
  
  //This can probably be made more efficient...

  float convert = 3.14159/180; //Convert from degrees to radians

  //Initialize vertices
  for(int i = 0; i < 10; ++i)
  {
    float angle = 36*i*convert;
    bottomFace[3*i] = topFace[3*i] = (len/2)*sin(angle);
    bottomFace[3*i+1] = topFace[3*i+1] = (len/2)*cos(angle);
    bottomFace[3*i+2] = -len/2;
    topFace[3*i+2] = len/2;
  }

  //Construct bottom face
  glBegin(GL_POLYGON);
    glNormal3f(0, 0, -1);
    for(int i = 0; i < 10; ++i) //CCW winding
    {
      glVertex3f(bottomFace[3*i], bottomFace[3*i+1], bottomFace[3*i+2]);
    }
  glEnd();

  //Construct top face
  glBegin(GL_POLYGON);
    glNormal3f(0, 0, 1);
    for(int i = 9; i >= 0; --i) //CCW winding
    {
      glVertex3f(topFace[3*i], topFace[3*i+1], topFace[3*i+2]);
    }
  glEnd();

  //Construct sides
  float offset = 18*convert;
  float dtheta = 36*convert;
  glBegin(GL_QUADS);
    for(int i = 0; i <= 8; ++i)
    {
      glNormal3f(sin(offset + i*dtheta), cos(offset + i*dtheta), 0);
      glVertex3f(topFace[3*i], topFace[3*i+1], topFace[3*i+2]);
      glVertex3f(topFace[3*i+3], topFace[3*i+4], topFace[3*i+5]);
      glVertex3f(bottomFace[3*i+3], bottomFace[3*i+4], bottomFace[3*i+5]);
      glVertex3f(bottomFace[3*i], bottomFace[3*i+1], bottomFace[3*i+2]);
    }
    
    //Add the last rectangle
    glNormal3f(sin(offset + 9*dtheta), cos(offset + 9*dtheta), 0);
    glVertex3f(topFace[3*9], topFace[3*9+1], topFace[3*9+2]);
    glVertex3f(topFace[0], topFace[1], topFace[2]);
    glVertex3f(bottomFace[0], bottomFace[1], bottomFace[2]);
    glVertex3f(topFace[3*9], topFace[3*9+1], topFace[3*9+2]);
  glEnd();
}

//Draw a cube of the given side length using GLUT's built-in function
void Hilbert::drawCube(float height)
{
  glutSolidCube(height);
}

//Draw a 6-sided green leaf in the xz plane with one tip at the origin
void Hilbert::drawLeaf(float len)
{
  glColor3f(0, 1, 0); //Full green
  glNormal3f(0, 1, 0);
  glBegin(GL_POLYGON);
    glVertex3f(0, 0, 0);
    glVertex3f(1.732*0.5*len, 0, 0.5*len);
    glVertex3f((1.732*0.5 + 1)*len, 0, 0.5*len);
    glVertex3f((1.732 + 1)*len, 0, 0);
    glVertex3f((1.732*0.5 + 1)*len, 0, -0.5*len);
    glVertex3f(1.732*0.5*len, 0, -0.5*len);
  glEnd();
  glColor3f(0.675, 0.451, 0.224); //Light brown
  //Ideally we would specify colors for different sections
}

//Draw a coordinate system with axes of the given length
void Hilbert::drawCoord(float len)
{
  glLineWidth(3); //Make the axes a bit more visible

  glBegin(GL_LINES);
    glColor3f(1,0,0);
    glVertex3f(0,0,0);
    glVertex3f(len,0,0);

    glColor3f(0,1,0);
    glVertex3f(0,0,0);
    glVertex3f(0,len,0);

    glColor3f(0,0,1);
    glVertex3f(0,0,0);
    glVertex3f(0,0,len);
  glEnd();

  glLineWidth(1);
}

//Fun method for changing the drawing color
void Hilbert::changeColor()
{
  if(color > 0.9)
  {
    up = false;
  }
  else if(color < 0.5)
  {
    up = true;
  }

  if(up) color += 0.05;
  else color -= 0.05;

  glColor3f(0, color, 0);
}


/***** Parse *****/

//Recursively parse and draw the given string based on the rules above

/*
Note: a character can be both a variable and a constant.  When level is
not equal to zero, the char will be treated as a variable.  When level
is equal to zero it will be treated as a constant (provided that it
represents a valid constant)
*/

void Hilbert::parseStr(string str, int level)
{
  for(int i = 0; i < str.length(); ++i)
  {
    char c = str[i];
    int index = varsStr.find_first_of(c);

    if(level != 0 && index != string::npos) //if c is a variable
    {
      parseStr(rulesArr[index], level-1);
    }
    else if(consts.find_first_of(c) != string::npos) //if c is a constant
    {
      switch(c)
      {
        case '[':
          glPushMatrix(); break;
        case ']':
          glPopMatrix(); break;
        case '+':
          glRotatef(angle,0,1,0); break;
        case '-':
          glRotatef(-angle,0,1,0); break;
        case '&':
          glRotatef(angle,1,0,0); break;
        case '^':
          glRotatef(-angle,1,0,0); break;
        case '\\':
          glRotatef(angle,0,0,1); break;
        case '/':
          glRotatef(-angle,0,0,1); break;
        case '|':
          glRotatef(180,0,1,0); break;
        case 'F':
          //changeColor();
          glCallList(shapes[userShape]); 
          break;
        case '`':
          glColor3f(1, 0, 0);
          break;
        case '~':
          glColor3f(0.5, 0.5, 0.5);
          break;
        case '$':
          glCallList(shapes[LEAF]);
          break;
      }
    }
  }
}


/***** OpenGL functions *****/

//Set up lighting, enable settings
void Hilbert::init()
{
  //Basic setup
  glClearColor(0.6, 0.8, 1, 0);
  glShadeModel(GL_FLAT);
  glEnable(GL_DEPTH_TEST);
  //To draw leaves, culling must be disabled
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  //Lighting and color
  GLfloat light_pos[] = {5, 5, 5, 1};
  GLfloat light_diffuse[] = {0.4, 0.4, 0.4, 1};
  GLfloat light_ambient[] = {0.1, 0.1, 0.1, 1};
  glLightfv(GL_LIGHT1, GL_POSITION, light_pos);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT1);

  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);

  //Create display lists to (hopefully) speed up processing
  shapes[CUBE] = glGenLists(1);
  glNewList(shapes[CUBE], GL_COMPILE);
    glTranslatef(0, 0, sideLen);
    drawCube(sideLen);
    glTranslatef(0, 0, sideLen);
    drawCube(sideLen);
  glEndList();

  shapes[PYRAMID] = glGenLists(1);
  glNewList(shapes[PYRAMID], GL_COMPILE);
    glTranslatef(0, 0, sideLen);
    drawPyra(sideLen);
    glTranslatef(0, 0, sideLen);
    drawPyra(sideLen);
  glEndList();

  shapes[CYLINDER] = glGenLists(1);
  glNewList(shapes[CYLINDER], GL_COMPILE);
    glTranslatef(0, 0, sideLen);
    drawCyl(sideLen);
    glTranslatef(0, 0, sideLen);
    drawCyl(sideLen);
  glEndList();

  shapes[LEAF] = glGenLists(1);
  glNewList(shapes[LEAF], GL_COMPILE);
    drawLeaf(sideLen);
  glEndList();
}

void Hilbert::display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();
  
  glPushMatrix();

  gluLookAt(20, 30, -20, 0, 10, 0, -1, 2, 1); //Corner perspective

  if(debug)
  {
    drawCoord(sideLen*10);
    cout << "Rotated by " << deg << " degrees" << endl;
  }

  //Draw a green circle as our base
  glColor3f(0, 0.8, 0);
  float convert = 3.14159/180;
  glBegin(GL_POLYGON);
    for(int i = 0; i < 360; i += 15)
    {
      glVertex3f(-10*cos(i*convert), 0, 10*sin(i*convert));
    }
  glEnd();

  glRotatef(deg, 0, 1, 0); //Rotate scene

  //Set up color variables
  color = 0.4;
  up = true;

  glColor3f(0.675, 0.451, 0.224); //Light brown
  
  glRotatef(-90, 1, 0, 0); //Set pen to point upwards
  parseStr(startStr, level);

  glPopMatrix();
  glutSwapBuffers();
}

//Handle window resizing
void Hilbert::reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60, (GLfloat)w/h, 1, 80);

  glMatrixMode(GL_MODELVIEW);
}

//Rotate scene with keyboard controls
void Hilbert::keyboard(unsigned char key, int x, int y)
{
  switch(key)
  {
    case 'a':
      deg = (deg+5)%360;
      glutPostRedisplay();
      break;
    case 'l':
      deg = (deg-5)%360;
      glutPostRedisplay();
      break;
    case 'r':
      deg = 0;
      glutPostRedisplay();
      break;
  }
}

//Register functions and run main loop
void Hilbert::gl_main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(1500, 600);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("3D L-systems!");

  init();

  curObj = this;
  glutDisplayFunc(displayWrapper);
  glutReshapeFunc(reshapeWrapper);
  glutKeyboardFunc(keyboardWrapper);

  glutMainLoop();
}
