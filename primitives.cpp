/*
  Uses SDL and OpenGL to draw primitive objects and an animation using these objects.
  Screens can be toggled by pressing the keys 'a' to 'e'.

  The number of sections that each primitive is controlled by the 'SECTIONS' global variable.

  The number of frames per second is controlled by the 'FPS' global variable.
  Animation speed is independent of machine speed; it will run at the same rate on all platforms.

  Calculation of vertices and colours is carried out as part of the initialisation, not on each redisplay.

  Alternative homogeneous functions for OpenGL's 'glTranslatef', 'glScalef' and 'glRotatef' functions are provided.
*/

#include <math.h>
#include <SDL.h>
#include <GL/GLew.h>
#include <SDL_opengl.h>

//Constants.
#define PI 3.141592
#define SECTIONS 30
#define FPS 60



//*****************************************
//             Global Data
//*****************************************
//Drawing data.
float sphereV[SECTIONS][SECTIONS][4][3];
float sphereNormalsV[SECTIONS][SECTIONS][2][3];
float coneBaseV[SECTIONS][3];
float coneStripsV[SECTIONS][3][3];

//Colour data.
float sphereC[SECTIONS][SECTIONS][4][3];
float sphereNormalsC[SECTIONS][SECTIONS][2][3];
float sphereShadedC[SECTIONS][SECTIONS][4][3];
float coneBaseC[SECTIONS][3];
float coneStripsC[SECTIONS][3][3];

//Which screen gets drawn.
int state = 1;

//Set the start angles of the animation.
int a = 0, b = 30, c = 180, d = 270;


//*****************************************
//      Homogeneous Matrix Functions
//*****************************************
//Matrices are column before row.
void translate(float x, float y, float z) {
  float m[] = {1, 0, 0, 0,
           0, 1, 0, 0,
           0, 0, 1, 0,
           x, y, z, 1};

  glMultMatrixf(m);
}

void scale(float x, float y, float z) {
  float m[] = {x, 0, 0, 0,
         0, y, 0, 0,
         0, 0, z, 0,
         0, 0, 0, 1};

  glMultMatrixf(m);
}

void rotateX(float angle) {
  //Convert angle to radians.
  angle *= PI / 180;
  float m[] = {1, 0,           0,          0,
         0, cos(angle),  sin(angle), 0,
         0, -sin(angle), cos(angle), 0,
         0, 0,           0,          1};

  glMultMatrixf(m);
}

void rotateY(float angle) {
  //Convert angle to radians.
  angle *= PI / 180;
  float m[] = {cos(angle), 0, -sin(angle), 0,
         0,      1, 0,       0,
         sin(angle), 0, cos(angle),  0,
         0,          0, 0,           1};

  glMultMatrixf(m);
}

void rotateZ(float angle) {
  angle *= PI / 180;
  float m[] = {cos(angle),  sin(angle), 0, 0,
         -sin(angle), cos(angle), 0, 0,
         0,       0,      1, 0,
         0,       0,      0, 1};

  glMultMatrixf(m);
}


//*****************************************
//         Calculation Functions
//*****************************************
//Vertex calculation.
void calculateV() {
  //Calculate the increment.
  float incr = 2 * PI / SECTIONS;

  //We use cosArr and sinArr to reuse the sine and cosine calculation.
  float cosArr[SECTIONS + 1], sinArr[SECTIONS + 1];
  for (int i = 0; i < SECTIONS + 1; i++) {
    cosArr[i] = cos(i * incr);
    sinArr[i] = sin(i * incr);
  }

  //Sphere calculation.
  //Loop over the sphere and calculate points using sphere equation.
  for (int i = 0; i < SECTIONS; i++)
    for (int j = 0; j < SECTIONS; j++) {
      sphereV[i][j][0][0] = sinArr[i] * cosArr[j];
      sphereV[i][j][0][1] = cosArr[i] * cosArr[j];
      sphereV[i][j][0][2] = sinArr[j];

      sphereV[i][j][1][0] = sinArr[i + 1] * cosArr[j];
      sphereV[i][j][1][1] = cosArr[i + 1] * cosArr[j];
      sphereV[i][j][1][2] = sinArr[j];

      sphereV[i][j][2][0] = sinArr[i + 1] * cosArr[j + 1];
      sphereV[i][j][2][1] = cosArr[i + 1] * cosArr[j + 1];
      sphereV[i][j][2][2] = sinArr[j + 1];

      sphereV[i][j][3][0] = sinArr[i] * cosArr[j + 1];
      sphereV[i][j][3][1] = cosArr[i] * cosArr[j + 1];
      sphereV[i][j][3][2] = sinArr[j + 1];

      sphereNormalsV[i][j][0][0] = sphereV[i][j][0][0];
      sphereNormalsV[i][j][0][1] = sphereV[i][j][0][1];
      sphereNormalsV[i][j][0][2] = sphereV[i][j][0][2];

      sphereNormalsV[i][j][1][0] = sphereNormalsV[i][j][0][0] * 1.1;
      sphereNormalsV[i][j][1][1] = sphereNormalsV[i][j][0][1] * 1.1;
      sphereNormalsV[i][j][1][2] = sphereNormalsV[i][j][0][2] * 1.1;
    }

  //Cone calculation.
  //Base of cone.
  for (int i = 0; i < SECTIONS; i++) {
    coneBaseV[i][0] = cosArr[i];
    coneBaseV[i][1] = -1;
    coneBaseV[i][2] = sinArr[i];
  }

  //Vertical line strips of cone.
  for (int i = 0; i < SECTIONS; i++) {
    coneStripsV[i][0][0] = 0;
    coneStripsV[i][0][1] = 1;
    coneStripsV[i][0][2] = 0;

    coneStripsV[i][1][0] = cosArr[i];
    coneStripsV[i][1][1] = -1;
    coneStripsV[i][1][2] = sinArr[i];

    coneStripsV[i][2][0] = 0;
    coneStripsV[i][2][1] = -1;
    coneStripsV[i][2][2] = 0;
  }
}

//Colour calculation.
void calculateC() {
  //Set the sphere colour to white.
  for (int i = 0; i < SECTIONS; i++)
    for (int j = 0; j < SECTIONS; j++)
      for (int k = 0; k < 4; k++) {
        sphereC[i][j][k][0] = 1;
        sphereC[i][j][k][1] = 1;
        sphereC[i][j][k][2] = 1;
      }

  //Set the sphere normals colour to green.
  for (int i = 0; i < SECTIONS; i++)
    for (int j = 0; j < SECTIONS; j++)
      for (int k = 0; k < 2; k++) {
        sphereNormalsC[i][j][k][0] = 0;
        sphereNormalsC[i][j][k][1] = 1;
        sphereNormalsC[i][j][k][2] = 0;
      }

  //Set the shaded sphere colours to the red component of the dot product, co-axial to the x-axis.
  //Include an ambient light factor of 0.1.
  for (int i = 0; i < SECTIONS; i++)
    for (int j = 0; j < SECTIONS; j++)
      for (int k = 0; k < 4; k++) {
        sphereShadedC[i][j][k][0] = sphereV[i][j][k][0] * 1 + 0.1;
        sphereShadedC[i][j][k][1] = sphereV[i][j][k][0] * 0 + 0.1;
        sphereShadedC[i][j][k][2] = sphereV[i][j][k][0] * 0 + 0.1;
      }

  //Set the cone base colour to yellow.
  for (int i = 0; i < SECTIONS; i++) {
    coneBaseC[i][0] = 1;
    coneBaseC[i][1] = 1;
    coneBaseC[i][2] = 0;
  }

  //Set the cone line strips colour to yellow.
  for (int i = 0; i < SECTIONS; i++)
    for (int j = 0; j < 3; j++) {
      coneStripsC[i][j][0] = 1;
      coneStripsC[i][j][1] = 1;
      coneStripsC[i][j][2] = 0;
    }
}



//*****************************************
//           Drawing Functions
//*****************************************
void drawWireSphere() {
  //Draw the sphere.
  glVertexPointer(3, GL_FLOAT, 0, sphereV);
  glColorPointer(3, GL_FLOAT, 0, sphereC);
  for (int i = 0; i < SECTIONS * SECTIONS; i++)
    glDrawArrays(GL_LINE_LOOP, i * 4, 4);
}

void drawWireCone() {
  //Draw the base.
  glVertexPointer(3, GL_FLOAT, 0, coneBaseV);
  glColorPointer(3, GL_FLOAT, 0, coneBaseC);
  glDrawArrays(GL_LINE_LOOP, 0, SECTIONS);

  //Draw the line strips.
  glVertexPointer(3, GL_FLOAT, 0, coneStripsV);
  glColorPointer(3, GL_FLOAT, 0, coneStripsC);
  for (int i = 0; i < SECTIONS; i++)
    glDrawArrays(GL_LINE_STRIP, i * 3, 3);
}

void drawSphereNormals() {
  //Draw the sphere.
  drawWireSphere();

  //Draw the normals.
  glVertexPointer(3, GL_FLOAT, 0, sphereNormalsV);
  glColorPointer(3, GL_FLOAT, 0, sphereNormalsC);
  for (int i = 0; i < SECTIONS * SECTIONS; i++)
    glDrawArrays(GL_LINES, i * 2, 3);
}

void drawShadedSphere() {
  //Draw the shaded sphere.
  glVertexPointer(3, GL_FLOAT, 0, sphereV);
  glColorPointer(3, GL_FLOAT, 0, sphereShadedC);
  for (int i = 0; i < SECTIONS * SECTIONS; i++)
    glDrawArrays(GL_QUADS, i * 4, 4);
}

void drawAnimation() {
  //Rotate the SphereNormals display in the x-axis.
  scale(0.35, 0.35, 0.35);
  rotateX(a);
  rotateZ(a);
  translate(0, 0, 4);
  drawSphereNormals();
  glLoadIdentity();

  //Rotate the WireCone drawing in the y-axis.
  scale(0.35, 0.35, 0.35);
  rotateY(b);
  rotateX(b);
  translate(4, 0, 0);
  drawWireCone();
  glLoadIdentity();

  //Rotate the WireSphere drawing the the z-axis.
  scale(0.35, 0.35, 0.35);
  rotateZ(c);
  rotateY(c);
  translate(0, 4, 0);
  drawWireSphere();
  glLoadIdentity();

  //Rotate the ShadedSphere drawing (on the spot) through the x-, y- and z- axis.
  scale(0.35, 0.35, 0.35);
  rotateY(d);
  drawShadedSphere();
  glLoadIdentity();

  //Reset the camera to orthographic.
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-2, 2, -2, 2, 2, -2);
  glMatrixMode(GL_MODELVIEW);
}

//*****************************************
//             SDL Functions
//*****************************************
void init() {
  //Initialise SDL.
  if(SDL_Init(SDL_INIT_VIDEO) < 0) exit(1);

  //Use double buffering (24-bit).
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

  //Set vertical sync.
  //SDL_GL_SetSwapInterval(1);

  //Create window.
  if (SDL_SetVideoMode(800, 800, 32, SDL_OPENGL) == 0) exit(1);
  SDL_WM_SetCaption("Primitives", "Primitives");

  //Set up orthographic camera.
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-2, 2, -2, 2, 2, -2);
  glMatrixMode(GL_MODELVIEW);

  //Set the clear color.
  glClearColor(0, 0, 0, 1);

  //Use the z-buffer.
  glEnable(GL_DEPTH_TEST);

  //Use vertex and colour arrays.
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);

  //Calculate the vertex and colour data.
  calculateV();
  calculateC();
}

void display() {
  //Clear buffers and set the transformation matrix to the identity matrix.
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  //Increment global angle variables (take FPS into account).
  a += 90 / FPS; b += 90 / FPS; c += 90 / FPS; d += 90 / FPS;
  a %= 360; b %= 360; c %= 360; d %= 360;

  //Draw scene depending on program state.
  switch (state) {
    case 1:
      rotateY(a);
      rotateZ(a);
      drawWireSphere();
    break;
    case 2:
      rotateX(20);
      rotateY(b);
      drawWireCone();
    break;
    case 3:
      rotateY(a);
      rotateZ(a);
      drawSphereNormals();
    break;
    case 4:
      rotateY(d);
      drawShadedSphere();
    break;
    case 5: drawAnimation();
    break;
  }

  //Swap buffers.
  SDL_GL_SwapBuffers();
}

void events() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    //Switch on key pressed.
    switch(event.key.keysym.sym) {
      case SDLK_ESCAPE:
      case SDLK_q: event.type = SDL_QUIT;
      break;
      case SDLK_a: state = 1;
      break;
      case SDLK_b: state = 2;
      break;
      case SDLK_c: state = 3;
      break;
      case SDLK_d: state = 4;
      break;
      case SDLK_e: state = 5;
      break;
    }

    //Exit if the q is pressed or the window is closed.
    if (event.type == SDL_QUIT) exit(0);
  }
}

int main() {
  //Initialise SDL and OpenGL.
  init();

  //Loop will continue until an exit event is triggered.
  int before, after, elapsed;
  while (true) {
    before = SDL_GetTicks();

    display();
    events();

    //Lock redisplay to FPS rate.
    after = SDL_GetTicks();
    elapsed = after - before;
    if (elapsed < 1000 / FPS)
      SDL_Delay(1000 / FPS - (after - before));
  }

  return 0;
}
