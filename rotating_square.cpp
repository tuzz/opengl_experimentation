#include <GL/glut.h>
#include <Math.h>

GLfloat theta = 0.0, thetar;
int n = 60;
float speed = 1;

void display() {

  glClear(GL_COLOR_BUFFER_BIT);
  glBegin(GL_POLYGON);
    thetar = theta * 3.14159 / 180.0;
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(cos(thetar), sin(thetar));
    glColor3f(0.0, 1.0, 0.0);
    glVertex2f(-sin(thetar), cos(thetar));
    glColor3f(0.0, 0.0, 1.0);
    glVertex2f(-cos(thetar), -sin(thetar));
    glColor3f(1.0, 1.0, 1.0);
    glVertex2f(sin(thetar), -cos(thetar));
  glEnd();
  glutSwapBuffers();
}

void timer(int n) {

  theta += speed;
  if (theta >= 360.0) theta -= 360.0;
  glutPostRedisplay();
  glutTimerFunc(1000 / n, timer, n);
}

void mouse(int button, int state, int x, int y) {

  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    speed += 1;
  if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    speed -= 1;
  if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
    speed = 0;
}

void reshape(GLsizei w, GLsizei h) {

  int size = (w < h) ? w : h;
  glViewport((w - size) / 2, (h - size) / 2, size, size);
}

void main(int argc, char ** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("Rotating Square");
    glutDisplayFunc(display);
    glutTimerFunc(1000 / n, timer, n);
    glutMouseFunc(mouse);
    glutReshapeFunc(reshape);
    glutMainLoop();
}
