#include <GL/glut.h>
#include "config.h"
#include "solar_system.h"


SolarSystem solarSystem;


void onDisplay()
{
	solarSystem.onDisplay();
}


void onUpdate()
{
	solarSystem.onUpdate();
}


void onKeyboard(unsigned char key, int x, int y)
{
	solarSystem.onKeyboard(key, x, y);
}


int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(WINDOW_X_POS, WINDOW_Y_POS);
	glutCreateWindow("Solar System");
	glutDisplayFunc(onDisplay);
	glutIdleFunc(onUpdate);
	glutKeyboardFunc(onKeyboard);
	glutMainLoop();
	return 0;
}