#include "Swarm.h"


Swarm::Swarm(string id, int n_Elements, float speed, float acceleration, int dim_2D, float coordDim2D, int cycleTime, string liderId, std::vector<string> boids_ids, std::vector<string> predators_ids, BWorld *bw)
{
	bwolrd = bw;
	s_predators_ids = predators_ids;
	s_lider_id = liderId;
	s_boids_ids = boids_ids;
	nElements = n_Elements;
	s_id = id;
	maxSpeed = speed;
	maxAceleration = acceleration;
	randomWeight = 0.0;
	constantForce = Vec3(0.0,0,0.0);
	has_externalForce = false;
	has_target = false;
	dim2D = dim_2D;
	coord2D = coordDim2D;
	enabled = true;
	updateVelCycleTime = cycleTime;

	oldTimeSinceStart = 0;
	deltaTimeSinceLastVelUpdate = 0;

	s_boidsptr = bwolrd->getBoids(s_boids_ids);
	s_predatorsptr = bwolrd->getBoids(s_predators_ids);


	

	// se não existir retornará NULL
	s_lider = bwolrd->getBoid(s_lider_id);
	if(s_lider == NULL)
	{
		printf("INSIDE\n");
		has_lider = false;
	}

	else
	{
		has_lider = true;
	}

	/*
	printf("boidsptrSIZE=%d\n",s_boidsptr.size());

	for(Boid* ptr : s_boidsptr)
	{
		printf("Boid=%s bound=%f\n",ptr->getId().c_str(),ptr->getBoundary());
	}
	*/
}


void Swarm::addBoid(Boid* b){
	s_boidsptr.push_back(b);
}



Boid* Swarm::getLider()
{
	return s_lider;
}


std::vector<string> Swarm::getPredatorsIds()
{
	return s_predators_ids;
}

std::vector<Boid*> Swarm::getPredators()
{
	return s_predatorsptr;
}


std::vector<string> Swarm::getBoidsIds()
{
	return s_boids_ids;
}

std::vector<Boid*> Swarm::getBoids()
{
	return s_boidsptr;
}

string Swarm::getId()
{
	return s_id;
}

int Swarm::getSize()
{
	return s_boidsptr.size();
}

bool Swarm::isEnabled()
{
	return enabled;
}


float Swarm::getCoord2D()
{
	return coord2D;
}


void Swarm::setId(string id)
{
	s_id = id;
}

void Swarm::setSpeed(float speed)
{
	maxSpeed = speed;
}

void Swarm::setCycleTime(float cycleTime)
{
	updateVelCycleTime = cycleTime;
}

void Swarm::setLiderId(string id)
{
	s_lider_id = id;
	s_lider = bwolrd->getBoid(id);
	has_lider = true;
}

void Swarm::setDimension(int dim_2D)
{
	dim2D = dim_2D;
}

void Swarm::setCoord2D(float s_coord2D)
{
	coord2D = s_coord2D;
}

void Swarm::setEnabled(bool mode)
{
	enabled = mode;
}


void Swarm::setTarget(Vec3 targetPos)
{
	has_target = true;
	target = targetPos;
}

void Swarm::disableTarget()
{
	has_target = false;
}

void Swarm::setflockWeight(float weight)
{
	flockWeight = weight;
}
		
void Swarm::setdisperseWeight(float weight)
{
	disperseWeight = weight;
}

void Swarm::setmatchWeight(float weight)
{
	matchWeight = weight;
}

void Swarm::setboundaryWeight(float weight)
{
	boundaryWeight = weight;
}

void Swarm::setfollowLiderWeight(float weight)
{
	followLiderWeight = weight;
}

void Swarm::setfollowPathWeight(float weight)
{
	followPathWeight = weight;
}

void Swarm::setrandomWeight(float weight)
{
	randomWeight = weight;
}

void Swarm::setavoidObstaclesWeight(float weight)
{
	avoidObstaclesWeight = weight;
}

void Swarm::setavoidPredatorsWeight(float weight)
{
	avoidPredatorsWeight = weight;
}

void Swarm::setTargetWeight(float weight)
{
	targetWeight = weight;
}


//PROCEDURE move_all_boids_to_new_positions()

void Swarm::Update(Scene *cenario)
{
		// para a velocidade não variar dependendo dos fps
		long timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
		int deltaTime = timeSinceStart - oldTimeSinceStart;
		deltaTimeSinceLastVelUpdate += deltaTime; 
		oldTimeSinceStart = timeSinceStart;

		if(!enabled)
		{
			return;
		}

		if(deltaTimeSinceLastVelUpdate < updateVelCycleTime)	
		{
			//printf("dtslvu=%d\n",deltaTimeSinceLastVelUpdate);

			for (auto boid : s_boidsptr)
			{
				boid->updatePosition(deltaTime, dim2D, coord2D);
			}
		}

		else
		{
			deltaTimeSinceLastVelUpdate = 0;


			float priority = 10;

			//FOR EACH BOID b
			for each (auto boid in s_boidsptr)
			{
				Vec3 v1 = Vec3(0,0,0),
				v2 = Vec3(0,0,0),
				v3 = Vec3(0,0,0),
				v4 = Vec3(0,0,0),
				v5 = Vec3(0,0,0),
				v6 = Vec3(0,0,0),
				v7 = Vec3(0,0,0),
				v8 = Vec3(0,0,0),
				v9 = Vec3(0,0,0),
				v10 = Vec3(0,0,0),
				v11 = Vec3(0,0,0),
				randomFactor = Vec3(rand()%(3) - 1, rand()%(3) - 1 , rand()%(3) - 1) * randomWeight;
				
				

				// Calculate

				v1 = boid->flock(s_boidsptr) * flockWeight;
				v2 = boid->disperse(bwolrd->getBoids()) * disperseWeight;		//dispersa de todos, nao apenas do swarm a que pertence
				v3 = boid->matchVelocity(s_boidsptr) * matchWeight;
				v4 = boid->bound_position() * boundaryWeight;					// paredes

				if(has_externalForce)
				{
					v5 = constantForce; 
				}

				if(has_target)
				{
					v6 = boid->tend_to_place(target) * targetWeight; 
				}

							
				// se tiver um caminho
				// armazenar posição seguinte no caminho para posteriormente testar se algum obstáculo o impede	de a alcançar			
				Vec3 nextTarget;
	
				if(boid->has_path)
				{
					Caminho path = bwolrd->getPath(boid->pathId);
					if(path.getCaminho().size()>0)
					{
						nextTarget = boid->getNextTarget(path);
					}
				}

				std::vector<Obstacle*> close_obstacles = cenario->getNeighbours(Obstacle(boid->getPosition(),boid->getSight()/2));
				for (auto &obstacle : close_obstacles)
				{
					v7 = v7 + boid->avoid_place(obstacle->getCenter());
					
					if(boid->has_path && obstacle->getCenter().distance(nextTarget) < obstacle->getRadius()*1.5)
					{
						boid->nextPos();		// passa ao ponto seguinte se um obstaculo estiver a impedir que chegue perto do target
					}
					
				}

				v7 = v7 * avoidObstaclesWeight;

				if(has_lider)
				{
					v8 = boid->tend_to_place(s_lider->getPosition()) * followLiderWeight;
					// dar prioridade a seguir o lider diminuindo os outros vectores
					//v1 = v1 / priority;
					//v2 = v2 / priority;
					//v3 = v3 / priority;
					randomFactor = randomFactor / priority;
				}

				for (auto &predator : s_predatorsptr)
				{
					v9 = v9 + boid->run_of_place(predator->getPosition());
				}

				v9 = v9 * avoidPredatorsWeight;


				if(boid->hasPath() && !boid->haspathFinished())
				{
					Caminho path = bwolrd->getPath(boid->pathId);
					v10 = boid->followPath(path) * followPathWeight;
				}

				v11 = boid->followPreys();

				if(boid->getId().compare("id01")==0)
				{

				}
				Vec3 newVelocity = boid->getVelocity() + (v1 + v2 + v3 + v4 + v5 + v6 + v7 + v8 + v9 + v10 + v11 + randomFactor);
				boid->limit_Velocity(newVelocity, maxSpeed, maxAceleration);
				boid->updatePosition(deltaTime,dim2D,coord2D);
							
			
			}//END
		}
		
}//	END PROCEDURE

