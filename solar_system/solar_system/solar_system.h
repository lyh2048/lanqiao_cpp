#pragma once
#include "config.h"
#include "stars.h"
#include <GL/glut.h>


class SolarSystem {
public:
	SolarSystem();
	~SolarSystem();

	void onDisplay();
	void onUpdate();
	void onKeyboard(unsigned char key, int x, int y);
private:
	Star* stars[STARS_NUM];

	GLdouble viewX, viewY, viewZ;
	GLdouble centerX, centerY, centerZ;
	GLdouble upX, upY, upZ;
};