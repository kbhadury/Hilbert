#include "hilbert.h"

int main(int argc, char** argv)
{
  //Draw a 3D Hilbert curve
  string hilbertVars = "ABCD";
  string hilbertRules[] = {
    "B-F+CFC+F-D&F^D-F+&&CFC+F+B//",
    "A&F^CFB^F^D^^-F-D^|F^B|FC^F^A//",
    "|D^|F^B-F+C^F^A&&FA&F^C+F+B^F^D//",
    "|CFB-F+B|FA&F^A&&FB-F+B|FC//"
  };
  string hilbertStart = "A";
  float hilbertAngle = 90;

  //Draw a fractal plant (not terribly realistic at the moment)
  string plant1Vars = "AFC";
  string plant1Rules[] = {
    "[&FA]/////[&FA]///////[&FA]",
    "C/////F",
    "F"
  };
  string plant1Start = "A";
  float plant1Angle = 22.5;

  //A different type of fractal plant
  string plant2Vars = "F";
  string plant2Rules[] = {
    "F[//&F]F[////&F][\\\\&F]F"
  };
  string plant2Start = "F";
  float plant2Angle = 25.7;

  float sideLen = 1;

  Hilbert h(plant1Start, plant1Vars, plant1Rules, plant1Angle, sideLen);
  h.enableDebug(false);
  h.setShape(h.CYLINDER);
  h.draw(argc, argv, 5); //Must be called last!

  return 0;
}
