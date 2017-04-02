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

  //A third fractal plant
  string plant3Vars = "XF";
  string plant3Rules[] = {
    "F[&X]////[&X]////[&X]FX",
    "FF"
  };
  string plant3Start = "X";
  float plant3Angle = 30;

  //Same as Plant1 but with leaves
  string plant4Vars = "AFS";
  string plant4Rules[] = {
    "[&F[--$]A]/////[&F[-$]A]///////[&F[---$]A]",
    "S/////F",
    "F[--$]",
  };
  string plant4Start = "A";
  float plant4Angle = 22.5;

  float sideLen = 1;

  Hilbert h(plant4Start, plant4Vars, plant4Rules, plant4Angle, sideLen);
  h.enableDebug(false);
  h.setShape(h.CYLINDER);
  h.draw(argc, argv, 5); //Must be called last!

  return 0;
}
