#include <math.h>
#include <GL/glut.h>

#define PI 3.14159265

float offset = 0.0;

void init() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(-9.0, 369.0, -1.1, 1.1);
  glMatrixMode(GL_MODELVIEW);

  glClearColor(1.0, 1.0, 1.0, 1.0);
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT);

  //Draw lines between sine and cosine.
  glLineWidth(1.0);
  glBegin(GL_LINES);
  for (int i = 0; i < 180; i++) {
    glColor3f(1 - ((float)i / 180), 0.0, (float)i / 180);
    glVertex2f(2 * i, sin((2 * (i + offset)) * PI / 180));
    glVertex2f(2 * i, cos((2 * (i + offset)) * PI / 180));
  }
  glEnd();

  //Draw sine.
  glLineWidth(2.0);
  glColor3f(0.0, 0.0, 0.0);
  glBegin(GL_LINE_STRIP);
  for (int i = 0; i < 180; i++)
    glVertex2f(2 * i, sin((2 * (i + offset)) * PI / 180));
  glEnd();

  //Draw cosine.
  glLineWidth(2.0);
  glColor3f(0.0, 0.0, 0.0);
  glBegin(GL_LINE_STRIP);
  for (int i = 0; i < 180; i++)
    glVertex2f(2 * i, cos((2 * (i + offset)) * PI / 180));
  glEnd();

  //Draw (sine + cosine) / 2
  glLineWidth(2.0);
  glColor3f(0.5, 0.5, 0.5);
  glBegin(GL_LINE_STRIP);
  for (int i = 0; i < 180; i++)
    glVertex2f(2 * i, (sin((2 * (i + offset)) * PI / 180) + cos((2 * (i + offset)) * PI / 180)) / 2);
  glEnd();

  glutSwapBuffers();
}

void timer(int n) {
  offset++;
  if (offset >= 360.0) offset -= 360.0;
  glutPostRedisplay();
  glutTimerFunc(1000 / 60, timer, 0);
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(1024, 400);
  glutCreateWindow("Moving sine and cosine");
  init();
  glutDisplayFunc(display);
  glutTimerFunc(1000 / 60, timer, 0);
  glutMainLoop();
  return 0;
}
