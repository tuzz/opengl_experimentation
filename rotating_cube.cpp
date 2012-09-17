#include <GL/glut.h>

GLfloat vertices[8][3] = {{-1, -1, -1}, {1, -1, -1},
{1, 1, -1}, {-1, 1, -1}, {-1, -1, 1}, {1, -1, 1},
{1, 1, 1}, {-1, 1, 1}};

GLfloat colors[8][3] = {{0, 0, 0}, {1, 0, 0},
{1, 1, 0}, {0, 1, 0}, {0, 0, 1}, {1, 0, 1},
{1, 1, 1}, {0, 1, 1}};

//Faces defined counter-clockwise to indicate the front
GLubyte cubeIndices[24] = {0, 3, 2, 1,
						   2, 3, 7, 6,
						   0, 4, 7, 3,
						   1, 2, 6, 5,
						   4, 5, 6, 7,
						   0, 1, 5, 4};

GLfloat theta[3] = {30, 45, 60};

GLint axis = 0;

void colorcube() {

	glEnableClientState(GL_COLOR_ARRAY);
	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, cubeIndices); //Draw the sides using custom color array

	glDisableClientState(GL_COLOR_ARRAY); 
	glColor3f(0, 0, 0); 
	glLineWidth(3);
	for (int i = 0; i < 6; i++)
		glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, &cubeIndices[4 * i]); //Draw black lines of width 3
}

void init() {

	glClearColor(1, 1, 1, 1);

	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2, 2, -2, 2, 2, -2);
	glMatrixMode(GL_MODELVIEW);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glColorPointer(3, GL_FLOAT, 0, colors);
}

void display() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glRotatef(theta[0], 1, 0, 0);
	glRotatef(theta[1], 0, 1, 0);
	glRotatef(theta[2], 0, 0, 1);
	colorcube();
	glutSwapBuffers();
}

void timer(int n) {

	theta[axis] += 2;
	if (theta[axis] > 360) theta[axis] -= 360;
	glutPostRedisplay();
	glutTimerFunc(1000 / 60, timer, 0);
}

void mouse(int button, int state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) axis = 0;
	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) axis = 1;
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) axis = 2;
}

void reshape(int width, int height) {

	int size = (width > height) ? height : width;
	glViewport((width - size) / 2, (height - size) / 2, size, size);
}

void keyboard(unsigned char key, int mousex, int mousey) {

	if (key == 'q' || key == 'Q') exit(0);
}

int main(int argc, char **argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Color Cube");
	init();
	glutDisplayFunc(display);
	glutTimerFunc(1000 / 60, timer, 0);
	glutMouseFunc(mouse);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}