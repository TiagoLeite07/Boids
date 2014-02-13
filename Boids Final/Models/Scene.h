#ifndef _SCENE
#define _SCENE
//#pragma once
#include "RTree.h"
#include "Obstacle.h"
#include <vector>
#include <map>
#include <string>

//#
using namespace std;


class Scene
{

private:

	float min[3];
	float max[3];
	RTree<string, float, 3, float> obstacles_tree;
	map<string,Obstacle*> obstacles_list;// =  std::list<Boid>();  

	bool rtree;

public:
	Scene(void);
	~Scene(void);
	void Add(Obstacle *to_add, string id);
	std::vector<Obstacle*> getNeighbours(Obstacle search_bounds);
	std::vector<Obstacle*> getObstacles(vector<string> ids);
	RTree<string, float, 3, float>* Scene::getTree();
	//bool MySearchCallback(int id, void* arg);

};

#endif