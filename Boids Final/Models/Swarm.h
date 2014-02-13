#ifndef _SWARM
#define _SWARM

#include "Boid.h"
#include "Caminho.h"
#include "RTree.h"
#include "Scene.h"
#include "BWorld.h"
#include "defs.h"

#include <GL/freeglut.h>

#include <vector>
#include <atomic>


using namespace std;


class BWorld;


class Swarm
{
	private:
		float maxSpeed;			// velocidade máxima para todos os elementos do grupo
		float maxAceleration;	// aceleração máxima para todos os elementos do grupo


		int dim2D;				// 0 -> 3D 1,2,3 -> 2D diferenciando os eixos
		float coord2D;			// coordenada para o eixo fixo
		int nElements;			// numero de elementos
		int lider_id;			// id (funciona como apontador)
		int updateVelCycleTime; // ms


		float flockWeight;
		float disperseWeight;
		float matchWeight;
		float boundaryWeight;
		float followLiderWeight;
		float followPathWeight;
		float randomWeight;
		float avoidObstaclesWeight;
		float avoidPredatorsWeight;
		float targetWeight;


		bool has_lider;
		bool has_target;
		bool has_externalForce;
		bool enabled;

		Vec3 target;
		Vec3 constantForce;		// substitui o wind, sendo mais geral. Pode significar a gravidade ou o que for necessário.

		int deltaTimeSinceLastVelUpdate;
		long oldTimeSinceStart;

		vector<Boid*> s_boidsptr;
		vector<Boid*> s_predatorsptr;
		Boid* s_lider;

		BWorld *bwolrd;
		vector<string> s_predators_ids;
		vector<string> s_boids_ids;
		string s_lider_id;

		string s_id;

	public:
		
		
		Swarm::Swarm(string id, int n_Elements, float speed, float acceleration, int dim_2D, float coordDim2D, int cycleTime, string liderId, std::vector<string> boids_ids, std::vector<string> predators_ids, BWorld *bw);
        
		void Swarm::Update(Scene *cenario);		// Move boids

		void Swarm::addBoid(Boid* b);
		Boid* Swarm::getLider();
		string Swarm::getId();
		bool Swarm::isEnabled();
		int Swarm::getSize();
		float Swarm::getCoord2D();

		std::vector<string> Swarm::getPredatorsIds();
		std::vector<string> Swarm::getBoidsIds();
		std::vector<Boid*> Swarm::getPredators();
		std::vector<Boid*> Swarm::getBoids();
			
		
		void Swarm::setId(string id);
		void Swarm::setSpeed(float speed);
		void Swarm::setCycleTime(float cycleTime);
		void Swarm::setLiderId(string id);
		void Swarm::setDimension(int dim_2D);
		void Swarm::setEnabled(bool mode);
		void Swarm::setCoord2D(float coord2D);
		void Swarm::setTarget(Vec3 targetPos);
		void Swarm::disableTarget();

		void Swarm::setflockWeight(float weight);
		void Swarm::setdisperseWeight(float weight);
		void Swarm::setmatchWeight(float weight);
		void Swarm::setboundaryWeight(float weight);
		void Swarm::setfollowLiderWeight(float weight);
		void Swarm::setfollowPathWeight(float weight);
		void Swarm::setrandomWeight(float weight);
		void Swarm::setavoidObstaclesWeight(float weight);
		void Swarm::setavoidPredatorsWeight(float weight);
		void Swarm::setTargetWeight(float weight);
	

};


#endif