#include <GL/glut.h>
#include <string>
using namespace std;

//From algorithmicbotany.org/papers/abop/abop-ch1.pdf
string rules[] = {
  "B-F+CFC+F-D&F^D-F+&&CFC+F+B//",
  "A&F^CFB^F^D^^-F-D^|F^B|FC^F^A//",
  "|D^|F^B-F+C^F^A&&FA&F^C+F+B^F^D//",
  "|CFB-F+B|FA&F^A&&FB-F+B|FC//"
};

int deg = 0; //Rotate scene

//Change colors
float blue;
bool up;

//Enable lighting and various settings
void init(void)
{
  glClearColor(0.5, 0.5, 0.5, 0);
  glShadeModel(GL_FLAT);
  glEnable(GL_DEPTH_TEST);

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
}

//Fun method for changing colors
void changeColor()
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

//Recursively parse and draw the given string based on the rules above
void parseStr(string str, int level)
{
  for(int i = 0; i < str.length(); ++i)
  {
    char c = str[i];
    if('A' <= c && c <= 'D') //c is a variable
    {
      if(level != 0)
      {
        parseStr(rules[c-'A'], level-1);
      }
    }
    else //c is an action
    {
      switch(c)
      {
        case '+':
          glRotatef(90, 0, 1, 0); break;
        case '-':
          glRotatef(-90, 0, 1, 0); break;
        case '&':
          glRotatef(90, 1, 0, 0); break;
        case '^':
          glRotatef(-90, 1, 0, 0); break;
        case '\\':
          glRotatef(90, 0, 0, 1); break;
        case '/':
          glRotatef(-90, 0, 0, 1); break;
        case '|':
          glRotatef(180, 0, 1, 0); break;
        case 'F':
          changeColor();
          glTranslatef(0, 0, 1);
          glutSolidCube(1);
          glTranslatef(0, 0, 1);
          glutSolidCube(1);
          break;
      }
    }
  }
}

//Handy method for drawing a coordinate system
void coord(float len)
{
  glLineWidth(3);

  glBegin(GL_LINES);
    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(len, 0, 0);

    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, len, 0);

    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, len);
  glEnd();
}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  
  glPushMatrix();

  gluLookAt(8, 8, 8, 0, 0, 0, -1, 2, -1);

  glRotatef(deg, 0, 1, 0); //Rotate scene

  coord(2); //Draw coordinate system for reference

  //Initialize color settings
  blue = 0.9;
  up = false;
  
  //Draw!
  parseStr("A", 3);

  glPopMatrix();
  glutSwapBuffers();
}

//Handle window resizing
void reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60, (GLfloat)w/h, 1, 40);

  glMatrixMode(GL_MODELVIEW);
}

//Rotate scene with keyboard controls (a and l)
void keyboard(unsigned char key, int x, int y)
{
  switch(key)
  {
    case 'a':
      deg = (deg+5) % 360;
      glutPostRedisplay();
      break;
    case 'l':
      deg = (deg-5) % 360;
      glutPostRedisplay();
      break;
  }
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(300, 300);
  glutInitWindowPosition(200, 200);
  glutCreateWindow("Hilbert curve!");

  init();

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);

  glutMainLoop();

  return 0;
}
