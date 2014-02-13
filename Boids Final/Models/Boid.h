#ifndef _BOID
#define _BOID

#include "Vec3.h"
#include "RTree.h"
#include "Caminho.h"
#include "defs.h"

#include <math.h>
#include <vector>
#include <string>
#include <map>


using namespace std;

class Boid
{
	private:
		string id;

		float sight;
		float space;
		float boundary;
		float maxVelocity;
		Vec3 centerofBoundary;
		Vec3 position;
		Vec3 velocity;
		int dim2D;				// 0 -> 3D 1,2,3 -> 2D diferenciando os eixos
		bool v_override;


		// Orientation Variables			(not in swarm beacause identified boids can have is own properties)
		bool calcRotation;					// calculate orientation or not
		float maxSteeringbySecond;			// degrees/sec
		float steeringDeadZone;				// degrees of deadzone 
		Vec3 rotationAxis;					// unused
		float angle;


		Caminho *path;
		bool pathLoop;						// loop do caminho (não implementado ainda) está sempre em loop por defeito para já
		bool pathFinished;
		bool enabled;

		vector<Boid*> friends;
		vector<Boid*> preys;
		void Boid::distanceToFriends();


		//Extra Properties
		map<string,string> properties;

	public:
		
		int closeToFriends;	
				
		bool has_path;
		string pathId;


		Boid::Boid();
		Boid::Boid(string idI, float boundaryI, int is2D = 0, float coord2D = 0);
		Boid::Boid(string b_id, float b_sight, float space, float b_boundary, float maxSteeringbySecond, float steeringDeadZone, bool calcRotation, int is2D, float coord2D);
		Vec3 getPosition();
		Vec3 getVelocity();
		float getSight();
		float getAngle();
		float getBoundary();
		string getId();
		bool hasPath();
		bool haspathFinished();
		void setPositionX(float x);
		void setPositionY(float y);
		void setPositionZ(float z);
		void setSight(float sight);
		void setSpace(float s);
		void setMaxVelocity(float s);
		void setBoundary(float b);
		void setMaxSteering(float maxSteeringbySecond);
		void setSteeringDeadZone(float steeringDeadZone);
		void setCalcRotation(bool mode);
		void setDimension(int is2D);
		void setPath(string s_pathId);
		void Boid::disableVelocityOverride();

		void Boid::setCenterofBoundary(Vec3 centerBoundary);
		void Boid::addFriend(Boid *b_friend);
		void Boid::addPrey(Boid *b_prey);
		void Boid::AddProperty(string key, string value);
		string Boid::getProperty(string key);
		void Boid::setProperties(map<string,string> b_properties);
		vector<string> Boid::getProperties();
		void Boid::nextPos();
		Vec3 Boid::getNextTarget(Caminho pathI);

		void Boid::updatePosition(float dt, int is2D, float coord2D);
		Vec3 Boid::flock(std::vector<Boid*> boids);
        Vec3 Boid::disperse(std::vector<Boid*> boids);
        Vec3 Boid::matchVelocity(std::vector<Boid*> boids);
		void Boid::limit_Velocity(float vlim);
		void Boid::limit_Velocity(float vMin, float vMax);
		void Boid::limit_Velocity(Vec3 newVel, float vlim, float maxAcceleration);
		Vec3 Boid::bound_position(int i_boundary = -1);
		Vec3 Boid::tend_to_place(Vec3 place);
		Vec3 Boid::run_of_place(Vec3 place,float sight_distance = -1);
		Vec3 Boid::avoid_place(Vec3 place);
		Vec3 Boid::followPreys();


		// Para lider
		int nextPosinPath;
		Vec3 Boid::followPath(Caminho pathI);
};


#endif

