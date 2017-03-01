#include <GL/glut.h>
#include <string>
using namespace std;

class Hilbert
{
  private:
    string consts;
    bool debug;

    //L-system data
    float angle;
    float sideLen;
    int level;
    string startStr;
    string varsStr;
    string* rulesArr;

    //Display lists
    GLuint forwardCube;
    GLuint forwardPyra;

    //Color and drawing
    void drawPyra(float height);
    void drawCube(float height);
    void drawCoord(float len);
    void changeColor();

    //Parse
    void parseStr(string str, int level);

    //OpenGL functions
    void init();
    void display();
    void reshape(int w, int h);
    void keyboard(unsigned char key, int x, int y);
    void gl_main(int argc, char **argv);
   
    //Workaround for issue with pointer to member functions
    static Hilbert* curObj;

    static void displayWrapper()
    {
      curObj->display();
    }

    static void reshapeWrapper(int w, int h)
    {
      curObj->reshape(w, h);
    }

    static void keyboardWrapper(unsigned char key, int x, int y)
    {
      curObj->keyboard(key, x, y);
    }
    
  public:
    Hilbert(string start, string vars, string* rules, float ang, float side);
    void enableDebug(bool enable);
    void draw(int argc, char** argv, int lvl);
};
