#ifndef _BWORLD
#define _BWORLD

#include "Boid.h"
#include "Swarm.h"
#include "Caminho.h"
#include <GL/freeglut.h>
#include "Scene.h"
#include "Paths.h"

#include <list>
#include <vector>
#include <iostream>
#include <string>
#include <tinyxml.h>
#include <atomic>
#include <map>
#include <sstream>

using namespace std;


class Swarm;

class BWorld
{
	private:

		Paths paths;
		Scene cenario;
		std::map<string,Swarm*> s_teams;
		std::map<string,Boid*> s_boidsCollection;

		int nextId;
		bool constantForceEnabled;
		Vec3 constantForce;

		string getNextId();				

	public:	
		BWorld::BWorld();					// inicializa sem boids
		BWorld::BWorld(string file);		// inicializa lendo o ficheiro XML
        void Update();
		void AddBoid(string id, float speed, float acceleration, int dim_2D, float coordDim2D, float b_sight, float space, Vec3 centerofBoundary, int b_boundary, float maxSteeringbySecond, float steeringDeadZone, bool calcRotation, string pathId, std::vector<string> identifiedFriends, map<string,string> properties);
		void AddSwarm(string id, int nElements, float speed, float acceleration, int dim_2D, float coordDim2D, int cycleTime, float b_sight, float space, bool has_target, Vec3 targetPos, Vec3 centerofBoundary, int b_boundary, float maxSteeringbySecond, float steeringDeadZone, bool calcRotation, string lider, std::vector<string> predators, std::vector<string> identifiedBoids, float flockWeight,
						float disperseWeight,
						float matchWeight,
						float boundaryWeight,
						float followLiderWeight,
						float followPathWeight,
						float randomWeight,
						float avoidObstaclesWeight,
						float avoidPredatorsWeight,
						float targetWeight);

		void BWorld::setconstantForce(Vec3 force);
		void enableconstantForce();
		void disableconstantForce();
		std::vector<Swarm*> BWorld::getSwarms();
		void BWorld::loadXML(string input);

		//void AddSwarm(int nElements, float speed, int id, int dim_2D, int cycleTime, string lider, std::vector<string> predators);
		Boid* BWorld::getBoid(string id);
		Caminho BWorld::getPath(string id);
		std::vector<Boid*> BWorld::getBoids(std::vector<string>);
		std::vector<Boid*> BWorld::getBoids();
};



#endif