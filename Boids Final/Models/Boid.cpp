#include "Boid.h"
	



	Boid::Boid(string idI, float boundaryI, int is2D, float coord2D)
	{
		id = idI;

		angle = 0;
		rotationAxis = Vec3(0,1,0);
		nextPosinPath = 0;
		printf("2d=%f\n",coord2D);
		// Estes parametros são dados no xml e a função irá recebelos
		boundary = boundaryI;
		sight = 20.0;
		space = 4.0;
		calcRotation = true;			//
		maxSteeringbySecond = 180;	// degrees/sec
		steeringDeadZone = 10;		// degrees
		has_path = false;
		pathLoop = true;
		pathFinished = false;
		enabled = true;
		nextPosinPath = 0;
		closeToFriends = 0;
		v_override = false;


		if(is2D == 0)
		{
			position = Vec3(rand()%(int)2*boundary-boundary, rand()%(int)2*boundary-boundary, rand()%(int)2*boundary-boundary);
		}
		else if(is2D == 1)
		{
			position = Vec3(coord2D, rand()%(int)2*boundary-boundary, rand()%(int)2*boundary-boundary);
		}
		else if(is2D == 2)
		{
			position = Vec3(rand()%(int)2*boundary-boundary, coord2D, rand()%(int)2*boundary-boundary);
		}
		else if(is2D == 3)
		{
			position = Vec3(rand()%(int)2*boundary-boundary, rand()%(int)2*boundary-boundary, coord2D);
		}

	}

	Boid::Boid(string b_id, float b_sight, float b_space, float b_boundary, float b_maxSteeringbySecond, float b_steeringDeadZone, bool b_calcRotation, int is2D, float coord2D)
	{
		id = b_id;

		angle = 0;
		rotationAxis = Vec3(0,1,0);
		nextPosinPath = 0;

		// Estes parametros são dados no xml e a função irá recebelos
		boundary = b_boundary;
		sight = b_sight;
		space = b_space;
		calcRotation = b_calcRotation;			//
		maxSteeringbySecond = b_maxSteeringbySecond;		// degrees/sec
		steeringDeadZone = b_steeringDeadZone;			// degrees
		has_path = false;
		pathLoop = true;
		pathFinished = false;
		enabled = true;
		nextPosinPath = 0;
		closeToFriends = 0;
		v_override = false;


		if(is2D == 0)
		{
			position = Vec3(rand()%(int)2*boundary-boundary, rand()%(int)2*boundary-boundary, rand()%(int)2*boundary-boundary);
		}
		else if(is2D == 1)
		{
			position = Vec3(coord2D, rand()%(int)2*boundary-boundary, rand()%(int)2*boundary-boundary);
		}
		else if(is2D == 2)
		{
			position = Vec3(rand()%(int)2*boundary-boundary, coord2D, rand()%(int)2*boundary-boundary);
		}
		else if(is2D == 3)
		{
			position = Vec3(rand()%(int)2*boundary-boundary, rand()%(int)2*boundary-boundary, coord2D);
		}

	}


	double safeAcos (double x)
	{
		if (x < -1.0)
			x = -1.0 ;
		else if (x > 1.0)
			x = 1.0 ;
		return acos (x) ;
	}



	void Boid::distanceToFriends()
	{
		closeToFriends = 0;
		for(Boid* boid : friends)
		{
			if(abs(boid->position.distance(position)) < sight)
			{
				closeToFriends++;
			}
		}
	}


	void Boid::updatePosition(float dt, int is2D, float coord2D)
	{
		
		Vec3 nextPos = position + velocity/100 *dt;

		if(is2D == 1)
		{
			nextPos.x = coord2D;
		}
		else if(is2D == 2)
		{
			nextPos.y = coord2D;
		}
		else if(is2D == 3)
		{
			nextPos.z = coord2D;
		}

		if(calcRotation)
		{
			Vec3 oldPosition = Vec3(position.x,0,position.z);
			Vec3 newPosition = Vec3(nextPos.x,0,nextPos.z);
			Vec3 newDirection = newPosition - oldPosition;
			newDirection.normalize();
			rotationAxis = Vec3(0,1,0);


			double angleInRadians = std::atan2(newDirection.z, newDirection.x);
			double angleInDegrees = (angleInRadians / 3.14) * 180.0;

			float deltaAngle = angleInDegrees-angle;

			if(deltaAngle > steeringDeadZone)
			{
				angle += maxSteeringbySecond * dt/1000;
			}
			else if(deltaAngle < -steeringDeadZone)
			{
				angle -= maxSteeringbySecond * dt/1000;
			}
			else
			{
				// Não varia
			}

		}
				
		position = nextPos;
		distanceToFriends();
	}



	// Flocking
	Vec3 Boid::flock(std::vector<Boid*> boids)
	{
		Vec3 pcJ;	// position centre of mass
		int count=0;
		//FOR EACH BOID b


		for each (Boid* boid in boids)
		{
			if(boid->position.distance(position) < sight)
			{
				pcJ = pcJ + boid->position;
				count++;
			}
		}
		

		// +0.001 to prevent division by zero
		pcJ = pcJ / (count + 0.001);

		return (pcJ - position) / 100;
	}


	// Disperse
	Vec3 Boid::disperse(std::vector<Boid*> boids)
	{
		Vec3 c = Vec3(0,0,0);

		//FOR EACH BOID b
		for each (Boid* boid in boids)
		{			
			if(boid->position.distance(position) < space)
			{
				c = c - (boid->position - position);
			}		
		}

		return c;
	}


	// Match Velocity
	Vec3 Boid::matchVelocity(std::vector<Boid*> boids)
	{
		Vec3 pvJ;
		int count = 0;
		//FOR EACH BOID b
		for each (Boid* boid in boids)
		{			
			if(boid->position.distance(position)<sight)
			{
				pvJ = pvJ + boid->velocity;
				count++;
			}
		}

		// +0.001 to prevent division by zero
		pvJ = pvJ / (count+0.001);

		return (pvJ - velocity) / 8;
	}




	void Boid::limit_Velocity(float vlim)
	{
		//printf("maxSpeed=%f\n",vlim);
		float length = velocity.length() + 0.001f;
		if( length > vlim)
		{
			velocity = (velocity / length) * vlim;
		}
	}

	void Boid::limit_Velocity(float vMin, float vMax)
	{
		float length = velocity.length() + 0.001f;
		if( length > vMax)
		{
			velocity = (velocity / length) * vMax;
		}

		else if( length < vMin)
		{
			velocity = (velocity / length) * vMin;
		}
	}

	void Boid::limit_Velocity(Vec3 newVel, float vlim, float maxAcceleration)
	{

		Vec3 acc = newVel - velocity;

		if(acc.x > maxAcceleration)
		{
			acc.x = maxAcceleration;
		}
		else if(acc.x < -maxAcceleration)
		{
			acc.x = -maxAcceleration;
		}

		if(acc.y > maxAcceleration)
		{
			acc.y = maxAcceleration;
		}
		else if(acc.y < -maxAcceleration)
		{
			acc.y = -maxAcceleration;
		}

		if(acc.z > maxAcceleration)
		{
			acc.z = maxAcceleration;
		}
		else if(acc.z < -maxAcceleration)
		{
			acc.z = -maxAcceleration;
		}


		velocity = velocity + acc;
		float length = velocity.length() + 0.001f;

		float vLimAux = vlim;
		if(v_override)
			vLimAux = maxVelocity;

		if( length > vLimAux)
		{
			velocity = (velocity / length) * vLimAux;
		}
	}


	Vec3 Boid::bound_position(int i_boundary)	// default i_boundary = -1
	{
		if(i_boundary!=-1)
		{
			boundary = i_boundary;
		}

		int Xmin = centerofBoundary.x-boundary,
			Xmax = centerofBoundary.x+boundary,
			Ymin = centerofBoundary.y-boundary,
			Ymax = centerofBoundary.y+boundary,
			Zmin = centerofBoundary.z-boundary,
			Zmax = centerofBoundary.z+boundary;

		Vec3 v;

		if(position.x < Xmin)
			v.x = 100;
		else if(position.x > Xmax)
			v.x = -100;
		//END IF
		if(position.y < Ymin)
			v.y = 100;
		else if(position.y > Ymax)
			v.y = -100;
		//END IF
		if(position.z < Zmin)
			v.z = 100;
		else if(position.z > Zmax)
			v.z = -100;
		//END IF
		
		return v;
	}



	Vec3 Boid::tend_to_place(Vec3 place)
	{
		return (place - position) / 100;
	}

	
	Vec3 Boid::run_of_place(Vec3 place,float distance)	//default distance = -1
	{
		if(distance==-1)
		{
			if(position.distance(place)<sight)
				return (position - place) / 100;
		}

		else
		{
			if(position.distance(place)<distance)
				return (place - position) / 100;
		}
	}
	
	// A unica diferença da de cima é que não testa se está dentro do raio de visão
	Vec3 Boid::avoid_place(Vec3 place)
	{
		return (position - place)*10;
	}



	// Flocking
	Vec3 Boid::followPreys()
	{
		Vec3 res;

		for each (Boid* boid in preys)
		{
			if(boid->position.distance(position) < sight*2)
			{
				res = boid->position - position;
				break;											// segue a primeira presa que lhe "passar pelos olhos"
			}
		}

		return res*10;
	}

	Vec3 Boid::getPosition()
	{
		return position;
	}

	Vec3 Boid::getVelocity()
	{
		return velocity;
	}

	float Boid::getSight()
	{
		return sight;
	}

	float Boid::getBoundary()
	{
		return boundary;
	}

	float Boid::getAngle()
	{
		return angle;
	}

	string Boid::getId()
	{
		return id;
	}



	void Boid::setPath(string s_pathId)
	{
		if(s_pathId.compare(NONE)!=0)
		{
			pathId = s_pathId;
			//path = b_path;
			has_path = true;
			nextPosinPath = 0;
		}
	}

	void Boid::setPositionX(float x)
	{
		position.x = x;
	}

	void Boid::setPositionY(float y)
	{
		position.y = y;
	}

	void Boid::setPositionZ(float z)
	{
		position.z = z;
	}


	void Boid::setSight(float s)
	{
		sight = s;
	}

	void Boid::setMaxVelocity(float s)
	{
		v_override = true;
		maxVelocity = s;
	}

	void Boid::disableVelocityOverride()
	{
		v_override = false;
	}

	void Boid::setSpace(float s)
	{
		space = s;
	}

	void Boid::setBoundary(float b)
	{
		boundary = b;
	}

	void Boid::setMaxSteering(float maxSteering)
	{
		maxSteeringbySecond = maxSteering;
	}

	void Boid::setSteeringDeadZone(float steerDeadZone)
	{
		steeringDeadZone = steerDeadZone;
	}

	void Boid::setCalcRotation(bool mode)
	{
		calcRotation = mode;
	}

	void Boid::setDimension(int b_is2D)
	{
		dim2D = b_is2D;
	}

	void Boid::setCenterofBoundary(Vec3 centerBoundary)
	{
		centerofBoundary = centerBoundary;
	}

	void Boid::nextPos()
	{
		nextPosinPath++;
	}

	void Boid::addFriend(Boid *b_friend)
	{
		friends.push_back(b_friend);
	}

	void Boid::addPrey(Boid *b_prey)
	{
		preys.push_back(b_prey);
	}

	void Boid::setProperties(map<string,string> b_properties)
	{
		properties = b_properties;
	}

	void Boid::AddProperty(string key, string value)
	{
		properties[key] = value;
	}

	string Boid::getProperty(string key)
	{
		string res;
		try
		{
			res = properties.at(key);
		}
		catch (const std::out_of_range& oor)		// caso não exista tal propriedade guardada
		{
			res = NONE;
		}

		return res;
	}

	vector<string> Boid::getProperties()
	{
		vector<string> res;
		for(auto iter=properties.begin(); iter!=properties.end(); iter++) {
			res.push_back(iter->second);
		}
		return res;
	}

	Vec3 Boid::getNextTarget(Caminho pathI)
	{
		Vec3 target;
		if(has_path)
		{
			std::vector<Vec3> path = pathI.getCaminho();
			target = path.at(nextPosinPath);
		}
		return target;
	}

	bool Boid::hasPath()
	{
		return has_path;
	}

	bool Boid::haspathFinished()
	{
		return pathFinished;
	}


	Vec3 Boid::followPath(Caminho pathI)
	{
		std::vector<Vec3> path = pathI.getCaminho();
		//printf("PathSize2: %d\n", path.size());
		Vec3 result;

		//printf("BOID:%f:%f:%f\n",boid.position.x,boid.position.y,boid.position.z);
		if(nextPosinPath < path.size())
		{
			Vec3 targetAux = path.at(nextPosinPath);
			//printf("TARGET:%f:%f:%f\n",targetAux.x,targetAux.y,targetAux.z);

			// this distance depends of the velocity
			// higher vel -> higher distance test
			float dist = 12;
			//printf("distance to target=%f\n",position.distance(targetAux));
			if(position.distance(targetAux)<=dist)
			{
				//printf("ENTREI COM D=%f\n",position.distance(targetAux));
				nextPosinPath++;
			}
				
			if(position.distance(targetAux)>dist)
			{
				result = tend_to_place(targetAux);
				//nextPosinPath++;
			}
		}

		if(nextPosinPath >= path.size())
		{
			if(pathLoop)
			{
				nextPosinPath = 0;
			}
			else
			{
				pathFinished;
			}
		}

		return result;

	}
