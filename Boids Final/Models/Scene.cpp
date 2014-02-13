#include "Scene.h"
	


Scene::Scene(void)
{
	//rtree = false;
	rtree = true;
}


Scene::~Scene(void)
{
//	Obstacles_tree.RemoveAll();
}


void Scene::Add(Obstacle *to_add, string id)
{
	obstacles_list[id] = to_add;
	//printf("Hit data rect %d\n", id);
	float min[3];
	float max[3];

	min[0] = to_add->getMin().x;
	min[1] = to_add->getMin().y;
	min[2] = to_add->getMin().z;

	max[0] = to_add->getMax().x;
	max[1] = to_add->getMax().y;
	max[2] = to_add->getMax().z;

	//printf( "Got minADD: [%f,%f,%f] \n",to_add.min[0],to_add.min[1],to_add.min[2]);
	//printf( "Got maxADD: [%f,%f,%f] \n",to_add.max[0],to_add.max[1],to_add.max[2]);
	//printf( "Got minADD: [%d,%d,%d] \n",to_add.min[0],to_add.min[1],to_add.min[2]);
	//printf( "Got maxADD: [%d,%d,%d] \n",to_add.max[0],to_add.max[1],to_add.max[2]);

	obstacles_tree.Insert(min, max, id); // Note, all values including zero are fine in this version
}


std::vector<string> obstacles;


bool MySearchCallback(string id, void* arg)
{
	obstacles.push_back(id);
	//printf("Hit data rect %d\n", id);
	return true; // keep going
}


std::vector<Obstacle*> Scene::getObstacles(vector<string> ids)
{
	std::vector<Obstacle*> res;
	for(auto key : ids)
	{
		try
		{
			Obstacle *ptr = obstacles_list.at(key);
			res.push_back(ptr);
		}
		catch (const std::out_of_range& oor)
		{
			printf("getBoids::Out of Range error: %s \n",oor.what());
		}

	}

	return res;
}

void print(RTree<string, float, 3, float>* atree)
{
	RTree<string, float, 3, float> tree = *atree;
	// Iterator test 
	int itIndex = 0;
	RTree<string, float, 3, float>::Iterator it;
	for( tree.GetFirst(it); !tree.IsNull(it); tree.GetNext(it) )
	{
		string value = tree.GetAt(it); 
		float boundsMin[3] = {0,0,0};
		float boundsMax[3] = {0,0,0};
		it.GetBounds(boundsMin, boundsMax);
		printf("it[%d] %d = (%d,%d,%d,%d,%d,%d)\n", itIndex++, value, boundsMin[0], boundsMin[1], boundsMin[2], boundsMax[0], boundsMax[1], boundsMax[2]);
	}
}

std::vector<Obstacle*> Scene::getNeighbours(Obstacle search_bounds)
{
	//std::vector<Obstacle> vizinhosA;
	std::vector< Obstacle* > aux_vizinhos;

	
	if(!rtree)
	{
		for(auto obstacle_iter = obstacles_list.begin(); obstacle_iter != obstacles_list.end(); obstacle_iter++) {
			obstacle_iter->second;
			if(search_bounds.intersect(obstacle_iter->second))
			{
				//printf("Adding obs = %f\n",obstacle.getRadius());
				aux_vizinhos.push_back(obstacle_iter->second);
			}
		}
	}
	
	else
	{
		min[0] = search_bounds.getMin().x;
		min[1] = search_bounds.getMin().y;
		min[2] = search_bounds.getMin().z;

		max[0] = search_bounds.getMax().x;
		max[1] = search_bounds.getMax().y;
		max[2] = search_bounds.getMax().z;

		//printf("(%f,%f,%f,%f,%f,%f)\n", min[0], min[1], min[2], max[0], max[1], max[2]);

		obstacles.clear();
		int nhits = obstacles_tree.Search(min, max, MySearchCallback, NULL);
		aux_vizinhos = getObstacles(obstacles);
	}

	return aux_vizinhos;
}




/*
RTree<int, float, 3, float>* Scene::getTree()
{
	return &Obstacles_tree;
}
*/
