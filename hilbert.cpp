#include "hilbert.h"
#include <iostream>

//Part of the member function workaround (see the .h file for details)
Hilbert* Hilbert::curObj = 0;

//Vars for changing the color
float blue;
bool up;

//Var for scene rotation
int deg = 0;


/**** Public methods *****/

//Constructor, describes L-system
Hilbert::Hilbert(string start, string vars, string* rules, float ang, float side)
{
  consts = "F+-&^\\/|[]"; //Characters that we recognize as constants
  debug = false;

  startStr = start;
  varsStr = vars;
  rulesArr = rules;
  angle = ang;
  sideLen = side;

  level = 0;
}

//Enable drawing a coordinate system and printing the rotation angle
void Hilbert::enableDebug(bool enable)
{
  debug = enable;
}

//Public-facing draw function, calls private gl_main function
void Hilbert::draw(int argc, char** argv, int lvl)
{
  level = lvl;
  gl_main(argc, argv);
}


/**** Color and drawing *****/

//Draw a pyramid of the given height (height is equal to base length)
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

//Draw a cube of the given side length using GLUT's built-in function
void Hilbert::drawCube(float height)
{
  glutSolidCube(height);
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
  if(blue > 0.9)
  {
    up = false;
  }
  else if(blue < 0.5)
  {
    up = true;
  }

  if(up) blue += 0.05;
  else blue -= 0.05;

  glColor3f(0, blue, blue);
}


/***** Parse *****/

//Recursively parse and draw the given string based on the rules above

/*
Note: a character can be both a variable and a constant.  When level is
not equal to zero, the char will be treated as a variable.  When level
is equal to zero it will be treated as a constant (provided that it
represents a valid constant
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
          changeColor();
          glCallList(forwardCube); 
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
  glClearColor(0.5, 0.5, 0.5, 0);
  glShadeModel(GL_FLAT);
  glEnable(GL_DEPTH_TEST);
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
  forwardCube = glGenLists(1);
  glNewList(forwardCube, GL_COMPILE);
    glTranslatef(0, 0, sideLen);
    glutSolidCube(sideLen);
    glTranslatef(0, 0, sideLen);
    glutSolidCube(sideLen);
  glEndList();

  forwardPyra = glGenLists(1);
  glNewList(forwardPyra, GL_COMPILE);
    glTranslatef(0, 0, sideLen);
    drawPyra(sideLen);
    glTranslatef(0, 0, sideLen);
    drawPyra(sideLen);
  glEndList();
}

void Hilbert::display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();
  
  glPushMatrix();

  gluLookAt(20, 20, -20, 0, 0, 0, -1, 2, 1); //Corner perspective

  if(debug)
  {
    drawCoord(sideLen*4);
    cout << "Rotated by " << deg << " degrees" << endl;
  }
  
  glRotatef(deg, 0, 1, 0); //Rotate scene

  //Set up color variables
  blue = 0.9;
  up = false;
  
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
  gluPerspective(60, (GLfloat)w/h, 1, 60);

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
