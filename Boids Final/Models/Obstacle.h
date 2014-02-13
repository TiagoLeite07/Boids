#ifndef _OBSTACLE
#define _OBSTACLE

#include "Vec3.h"
#include "defs.h"

#include <stdio.h>
#include <string>
//#pragma once

using namespace std;

static const int SPHERE = 0;
static const int BOX = 1;

class Obstacle
{
	

public:
	Obstacle(void);
	Obstacle::Obstacle(string o_id, float a_minX, float a_minY, float a_minZ, float a_maxX, float a_maxY, float a_maxZ);
	Obstacle::Obstacle(string o_id, Vec3 min, Vec3 max);
	Obstacle::Obstacle(Vec3 i_min, Vec3 i_max);
	Obstacle::Obstacle(string o_id, Vec3 center, float radius);
	Obstacle::Obstacle(Vec3 center, float radius);
	bool Obstacle::intersect(Obstacle *other);
	Vec3 Obstacle::getCenter();
	float Obstacle::getRadius();
	Vec3 Obstacle::getMin();
	Vec3 Obstacle::getMax();
	string Obstacle::getId();
	//~Obstacle(void);

private:


	string id;
	int o_type;
	// sphere
	Vec3 o_center;
	float o_radius;
	// box
	Vec3 bMin;
	Vec3 bMax;
};

#endif
