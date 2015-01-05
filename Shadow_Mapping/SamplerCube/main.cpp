#include <include/includeAll.h>
#include "scene.h"

Scene scene(SCREEN_WIDTH, SCREEN_HEIGHT);

//-----------------------------
void idle() {

	glutPostRedisplay();
}

//-----------------------------
void draw() {

	scene.draw();
}

//-----------------------------
void Mouse(int button, int state, int x, int y) {

	scene.Mouse(button, state, x, y);
}

//-----------------------------
void MouseEvent(int x, int y) {

	scene.MouseEvent(x, y);
}

//-----------------------------
int main(int argc, char** argv) {

	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Shadow Mapping");

	GLenum res = glewInit();
    if (res) {

      fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
      return 1;
    }
    
    printf("GL version: %s\n", glGetString(GL_VERSION));

	/* init */
	if(!scene.init()) return 0;

    glutDisplayFunc(draw);
	glutMouseFunc(Mouse);
    glutMotionFunc(MouseEvent);
	glutIdleFunc(idle);

    glutMainLoop();

	return 0;
}
