#include "BWorld.h"



BWorld::BWorld()
{
	constantForceEnabled = false;
	constantForce = Vec3(0,0,0);
}

BWorld::BWorld(string file)
{
	constantForceEnabled = false;
	constantForce = Vec3(0,0,0);
	loadXML(file);
}

void BWorld::Update()
{

	for(auto swarm_iter = s_teams.begin(); swarm_iter != s_teams.end(); swarm_iter++) {
		swarm_iter->second->Update(&cenario);
    }

}
		


string convertInt(int number)
{
   stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}


string BWorld::getNextId()
{
	nextId++;

	return convertInt(nextId-1);
}


void BWorld::AddBoid(string s_id, float speed, float acceleration, int dim_2D, float coordDim2D, float b_sight, float space, Vec3 centerofBoundary, int b_boundary, float maxSteeringbySecond, float steeringDeadZone, bool calcRotation, string pathId, std::vector<string> identifiedFriends, map<string,string> properties)
{	

	Boid* boid = new Boid(s_id, b_boundary, dim_2D, coordDim2D);
	boid->setSight(b_sight);
	boid->setSpace(space);
	boid->setMaxVelocity(speed);
	if(speed==0){
		boid->disableVelocityOverride();
	}
	boid->setDimension(dim_2D);
	boid->setMaxSteering(maxSteeringbySecond);
	boid->setSteeringDeadZone(steeringDeadZone);
	boid->setCalcRotation(calcRotation);
	boid->setPath(pathId);
	boid->setProperties(properties);
	boid->setCenterofBoundary(centerofBoundary);

	for(string b_id : identifiedFriends)
	{
		Boid* friendb = getBoid(b_id);

		if(boid)
		{
			boid->addFriend(friendb);				// guarda apontador no boid
		}
	}
			
	s_boidsCollection[s_id] = boid;				// guarda apontador para o boid no map


}


void BWorld::AddSwarm(string s_id, int nElements, float speed, float acceleration, int dim_2D, float coordDim2D, int cycleTime, float b_sight, float space, bool has_target, Vec3 targetPos, Vec3 centerofBoundary, int b_boundary, float maxSteeringbySecond, float steeringDeadZone, bool calcRotation, string liderid, std::vector<string> predators, std::vector<string> identifiedBoids,
						float flockWeight,
						float disperseWeight,
						float matchWeight,
						float boundaryWeight,
						float followLiderWeight,
						float followPathWeight,
						float randomWeight,
						float avoidObstaclesWeight,
						float avoidPredatorsWeight,
						float targetWeight)
{			
	// lista de ids dos boids pertencetes ao grupo
	vector<string> s_boids;	

	int i;
    for (i = 0; i < nElements; i++)
    {
		string b_id = getNextId();
		Boid* aux = new Boid(b_id, b_boundary, dim_2D, coordDim2D);			// 40 é o boundarie
		aux->setCenterofBoundary(centerofBoundary);
		s_boidsCollection[b_id] = aux;				// guarda apontador para o boid no map
		s_boids.push_back(b_id);					// guarda id/key no swarm

    }

	for(string b_id : identifiedBoids)
	{
		Boid* boid = getBoid(b_id);						// verificar se o id é válido
		if(boid)										
		{
			s_boids.push_back(b_id);					// guarda id/key no swarm
		}
	}


	Swarm* toAdd = new Swarm(s_id, nElements, speed, acceleration, dim_2D, coordDim2D, cycleTime, liderid, s_boids, predators, this);
	if(has_target)
	{
		toAdd->setTarget(targetPos);
	}
	toAdd->setflockWeight(flockWeight);
	toAdd->setdisperseWeight(disperseWeight);
	toAdd->setboundaryWeight(boundaryWeight);
	toAdd->setfollowLiderWeight(followLiderWeight);
	toAdd->setfollowPathWeight(followPathWeight);
	toAdd->setrandomWeight(randomWeight);
	toAdd->setavoidObstaclesWeight(avoidObstaclesWeight);
	toAdd->setavoidPredatorsWeight(avoidPredatorsWeight);
	toAdd->setTargetWeight(targetWeight);

	s_teams[s_id] = toAdd;	// guarda apontador para o swarm no map


	// adiciona todos os boids deste grupo como presas dos respectivos predadores
	for(string p_id : predators)
	{
		printf("predador=%s\n",p_id.c_str());
		Boid* predator = getBoid(p_id);						// verificar se o id é válido
		if(predator)										
		{
			printf("predador=%s\n",predator->getId().c_str());
			for(string b_id : s_boids)
			{
				Boid* prey = getBoid(b_id);						// verificar se o id é válido
				printf("presa=%s\n",prey->getId().c_str());
				predator->addPrey(prey);				// guarda id/key no swarm
			}
		}
	}

}

void BWorld::setconstantForce(Vec3 force)
{
	constantForce = force;
}


void BWorld::enableconstantForce()
{
	constantForceEnabled = true;
}

void BWorld::disableconstantForce()
{
	constantForceEnabled = false;
}

std::vector<Swarm*> BWorld::getSwarms()
{
	std::vector<Swarm*> res;
	for(auto swarm_iter = s_teams.begin(); swarm_iter != s_teams.end(); ++swarm_iter) {
		res.push_back(swarm_iter->second);
    }
	return res;
}


Boid* BWorld::getBoid(string id)
{
	Boid *ptr = NULL;
	try
	{
		ptr = s_boidsCollection.at(id);
	}
	catch (const std::out_of_range& oor)
	{
		std::cerr << "getBoid::Out of Range error: " << oor.what() << '\n';
	}

	return ptr;
}


Caminho BWorld::getPath(string id)
{
	Caminho ptr;// = NULL;
		try
		{
			ptr = paths.getPath(id);
		}
		catch (const std::out_of_range& oor)
		{
			std::cerr << "getPath::Out of Range error: " << oor.what() << '\n';
		}

	return ptr;
}



std::vector<Boid*> BWorld::getBoids(std::vector<string> ids)
{
	std::vector<Boid*> res;
	for(auto key : ids)
	{
		try
		{
			Boid *ptr = s_boidsCollection.at(key);
			printf("Key=%s	Boid=%f\n",key.c_str(),ptr->getBoundary());
			res.push_back(ptr);
		}
		catch (const std::out_of_range& oor)
		{
			std::cerr << "getBoids::Out of Range error: " << oor.what() << '\n';
		}

	}

	return res;
}

std::vector<Boid*> BWorld::getBoids()
{
	std::vector<Boid*> res;

	for( map <string, Boid*>::iterator it = s_boidsCollection.begin(); it != s_boidsCollection.end(); ++it ) {
    	res.push_back( it->second );
    }

	return res;
}

void BWorld::loadXML(string input)
{
	TiXmlDocument doc;

	if(!doc.LoadFile(input.c_str()))
	{
		printf( "ERRO: %s\n",doc.ErrorDesc());
		return;
	}
	
	TiXmlElement* root = doc.FirstChildElement();
	if(root == NULL)
	{
		printf("Failed to load file: No root element.\n");
		doc.Clear();
		return;
	}
	
	for(TiXmlElement* elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
	{

		std::string elemName = elem->Value();
		
        const char* attr;
		if(elemName == "Group")
		{
			int number=0,i_2D=0,cycleTime=10;
			float vel = 0.5,
				acceleration = 0.1,
				space = 10, sight = 40,
				maxSteeringbySecond = 180,
				steeringDeadZone = 10,
				boundary = 50,
				coordSpace = 0,
				flockWeight = 1,
				disperseWeight = 1,
				matchWeight = 1,
				boundaryWeight = 1,
				followLiderWeight = 1,
				followPathWeight = 1,
				randomWeight = 0,
				avoidObstaclesWeight = 1,
				avoidPredatorsWeight = 1,
				targetWeight = 1;

			bool calcRotation;
			Vec3 centerOfBoundary;
			bool has_target = false;
			Vec3 targetPos;
			string s_lider(NONE);
			string s_id(NONE);
			vector<string> predators;
			vector<string> identifiedBoids;
			char aux[3][10];

			printf( "\n Got Group\n");
			
			attr = elem->Attribute("id");
			if(attr != NULL){
				printf( "Got id: %s \n",attr);
				s_id = string(attr);
				printf( "Got id: %s \n",s_id.c_str());

			}

			for(TiXmlElement* atributo = elem->FirstChildElement(); atributo != NULL; atributo = atributo->NextSiblingElement()){
				attr = atributo->Attribute("cycleTime");
				if(attr != NULL){
					printf( "Got cycleTime: %s \n",attr);
					cycleTime = atoi(attr);
					printf( "Got cycleTime: %d \n",cycleTime);

				}


				
				attr = atributo->Attribute("number");
				if(attr != NULL){
					printf( "Got number: %s \n",attr);
					number = atoi(attr);
					printf( "Got number: %d \n",number);

				}

				attr = atributo->Attribute("velocity");
				if(attr != NULL){
					printf( "Got velocity: %s \n",attr);
					std::string fs(attr);
					vel=std::stof(fs);//this is much better way to do it
					//vel = atof(attr);
					printf( "Got velocity: %f \n",vel);
				}

				attr = atributo->Attribute("acceleration");
				if(attr != NULL){
					printf( "Got acceleration: %s \n",attr);
					std::string fs(attr);
					acceleration=std::stof(fs);//this is much better way to do it
					//vel = atof(attr);
					printf( "Got acceleration: %f \n",vel);
				}


				attr = atributo->Attribute("space");
				if(attr != NULL){
					printf( "Got Space: %s \n",attr);
					std::string fs(attr);
					space = std::stof(fs);
					printf( "Got Space: %f \n",space);
				}


				attr = atributo->Attribute("sight");
				if(attr != NULL){
					printf( "Got Sight: %s \n",attr);
					std::string fs(attr);
					sight = std::stof(fs);
					printf( "Got Sight: %f \n",sight);
				}


				attr = atributo->Attribute("steeringDeadZone");
				if(attr != NULL){
					printf( "Got steeringDeadZone: %s \n",attr);
					std::string fs(attr);
					steeringDeadZone = std::stof(fs);
					printf( "Got steeringDeadZone: %f \n",steeringDeadZone);
				}


				attr = atributo->Attribute("maxSteeringbySecond");
				if(attr != NULL){
					printf( "Got maxSteeringbySecond: %s \n",attr);
					std::string fs(attr);
					maxSteeringbySecond = std::stof(fs);
					printf( "Got maxSteeringbySecond: %f \n",maxSteeringbySecond);
				}

				attr = atributo->Attribute("boundary");
				if(attr != NULL){
					printf( "Got boundary: %s \n",attr);
					std::string fs(attr);
					boundary = std::stof(fs);
					printf( "Got boundary: %f \n",boundary);
				}

				attr = atributo->Attribute("centerOfBoundary");
				if(attr != NULL){
					printf( "Got center: %s \n",attr);
					sscanf (attr,"( %s , %s , %s )",aux[0],aux[1],aux[2]);
					centerOfBoundary.x=std::stof(aux[0]);
					centerOfBoundary.y=std::stof(aux[1]);
					centerOfBoundary.z=std::stof(aux[2]);
				}

				attr = atributo->Attribute("targetPos");
				if(attr != NULL){
					printf( "Got targetPos: %s \n",attr);
					sscanf (attr,"( %s , %s , %s )",aux[0],aux[1],aux[2]);
					targetPos.x=std::stof(aux[0]);
					targetPos.y=std::stof(aux[1]);
					targetPos.z=std::stof(aux[2]);
					has_target = true;
				}

				attr = atributo->Attribute("dim");
				if(attr != NULL){
					printf( "Got 2D: %s \n",attr);
					i_2D = atoi(attr);
					printf( "Got 2D: %d \n",i_2D);
				}

				attr = atributo->Attribute("coord2D");
				if(attr != NULL){
					printf( "Got 2DCoord: %s \n",attr);
					std::string fs(attr);
					coordSpace = std::stof(fs);
					printf( "Got 2DCoord: %f \n",coordSpace);
				}

				attr = atributo->Attribute("liderId");
				if(attr != NULL){
					printf( "Got IDLider: %s \n",attr);
					s_lider = string(attr);
					printf( "Got IDLider: %s \n",s_lider.c_str());

				}

				attr = atributo->Attribute("predatorId");
				if(attr != NULL){
					printf( "Got IDPredator: %s \n",attr);
					string predator(attr);
					predators.push_back(predator);
					printf( "Got IDPredator: %s \n",predator.c_str());

				}


				attr = atributo->Attribute("predatorsGroup");
				if(attr != NULL){
					printf( "Got PredatorsGroup: %s \n",attr);
					string groupId(attr);
					vector<string> s_predators = s_teams[groupId]->getBoidsIds();
					for(string id : s_predators)
					{
						predators.push_back(id);
						printf( "Got Predator: %s \n",id.c_str());

					}
					printf( "Got PredatorGroup: %s \n",groupId.c_str());

				}

				attr = atributo->Attribute("add");
				if(attr != NULL){
					printf( "Got add id: %s \n",attr);
					string boid(attr);
					identifiedBoids.push_back(boid);
					printf( "Got add id: %s \n",boid.c_str());

				}

				attr = atributo->Attribute("flockWeight");
				if(attr != NULL){
					printf( "Got flockWeight: %s \n",attr);
					std::string weight(attr);
					flockWeight = std::stof(weight);
					printf( "Got flockWeight: %f \n",flockWeight);
				}

				attr = atributo->Attribute("disperseWeight");
				if(attr != NULL){
					printf( "Got disperseWeight: %s \n",attr);
					std::string weight(attr);
					disperseWeight = std::stof(weight);
					printf( "Got disperseWeight: %f \n",disperseWeight);
				}

				attr = atributo->Attribute("matchWeight");
				if(attr != NULL){
					printf( "Got matchWeight: %s \n",attr);
					std::string weight(attr);
					matchWeight = std::stof(weight);
					printf( "Got matchWeight: %f \n",matchWeight);
				}

				attr = atributo->Attribute("boundaryWeight");
				if(attr != NULL){
					printf( "Got boundaryWeight: %s \n",attr);
					std::string weight(attr);
					boundaryWeight = std::stof(weight);
					printf( "Got boundaryWeight: %f \n",boundaryWeight);
				}

				attr = atributo->Attribute("followLiderWeight");
				if(attr != NULL){
					printf( "Got followLiderWeight: %s \n",attr);
					std::string weight(attr);
					followLiderWeight = std::stof(weight);
					printf( "Got followLiderWeight: %f \n",followLiderWeight);
				}

				attr = atributo->Attribute("followPathWeight");
				if(attr != NULL){
					printf( "Got followPathWeight: %s \n",attr);
					std::string weight(attr);
					followPathWeight = std::stof(weight);
					printf( "Got followPathWeight: %f \n",followPathWeight);
				}

				attr = atributo->Attribute("randomWeight");
				if(attr != NULL){
					printf( "Got randomWeight: %s \n",attr);
					std::string weight(attr);
					randomWeight = std::stof(weight);
					printf( "Got randomWeight: %f \n",randomWeight);
				}

				attr = atributo->Attribute("avoidObstaclesWeight");
				if(attr != NULL){
					printf( "Got avoidObstaclesWeight: %s \n",attr);
					std::string weight(attr);
					avoidObstaclesWeight = std::stof(weight);
					printf( "Got avoidObstaclesWeight: %f \n",avoidObstaclesWeight);
				}

				attr = atributo->Attribute("avoidPredatorsWeight");
				if(attr != NULL){
					printf( "Got avoidPredatorsWeight: %s \n",attr);
					std::string weight(attr);
					avoidPredatorsWeight = std::stof(weight);
					printf( "Got avoidPredatorsWeight: %f \n",avoidPredatorsWeight);
				}

				attr = atributo->Attribute("targetWeight");
				if(attr != NULL){
					printf( "Got targetWeight: %s \n",attr);
					std::string weight(attr);
					targetWeight = std::stof(weight);
					printf( "Got targetWeight: %f \n",targetWeight);
				}
				
			}

			//AddSwarm(s_id,number,vel,acceleration,i_2D,cycleTime,float b_sight, float space, int b_boundary, float maxSteeringbySecond, float steeringDeadZone, bool calcRotation,s_lider,predators,identifiedBoids);
			AddSwarm(s_id,
				number,
				vel,
				acceleration,
				i_2D,
				coordSpace,
				cycleTime,
				sight,
				space,
				has_target,
				targetPos,
				centerOfBoundary,
				boundary,
				maxSteeringbySecond,
				steeringDeadZone,
				calcRotation,
				s_lider,
				predators,
				identifiedBoids,
				flockWeight,
				disperseWeight,
				matchWeight,
				boundaryWeight,
				followLiderWeight,
				followPathWeight,
				randomWeight,
				avoidObstaclesWeight,
				avoidPredatorsWeight,
				targetWeight);
			//AddSwarm(number,vel,s_id,i_2D,cycleTime,s_lider,predators);
			//Swarm::Swarm(int n_Elements, float speed, int id, int dim_2D, int cycleTime, int lider, std::vector<int> predators)

		}

		else if(elemName == "Boid")
		{

			string b_id(NONE);
			string id_caminho(NONE);
			int i_2D=0,cycleTime=10;
			float vel = 0, acceleration = 0.1, space = 10, sight = 20, maxSteeringbySecond = 180, steeringDeadZone = 10, boundary = 50, coordSpace = 0;
			bool calcRotation = true;
			vector<string> b_friends;
			map<string,string> properties;
			char aux[3][10];
			Vec3 centerOfBoundary;

			printf( "\n Got Boid\n");
			
			attr = elem->Attribute("id");
			if(attr != NULL){
				printf( "Got add id: %s \n",attr);
				b_id = string(attr);
				printf( "Got add id: %s \n",b_id.c_str());

			}

			for(TiXmlElement* atributo = elem->FirstChildElement(); atributo != NULL; atributo = atributo->NextSiblingElement()){
				
				attr = atributo->Attribute("velocity");
				if(attr != NULL){
					printf( "Got velocity: %s \n",attr);
					std::string fs(attr);
					vel=std::stof(fs);//this is much better way to do it
					//vel = atof(attr);
					printf( "Got velocity: %f \n",vel);
				}

				attr = atributo->Attribute("acceleration");
				if(attr != NULL){
					printf( "Got acceleration: %s \n",attr);
					std::string fs(attr);
					acceleration=std::stof(fs);//this is much better way to do it
					//vel = atof(attr);
					printf( "Got acceleration: %f \n",vel);
				}


				attr = atributo->Attribute("space");
				if(attr != NULL){
					printf( "Got Space: %s \n",attr);
					std::string fs(attr);
					space = std::stof(fs);
					printf( "Got Space: %f \n",space);
				}


				attr = atributo->Attribute("sight");
				if(attr != NULL){
					printf( "Got Sight: %s \n",attr);
					std::string fs(attr);
					sight = std::stof(fs);
					printf( "Got Sight: %f \n",sight);
				}


				attr = atributo->Attribute("steeringDeadZone");
				if(attr != NULL){
					printf( "Got steeringDeadZone: %s \n",attr);
					std::string fs(attr);
					steeringDeadZone = std::stof(fs);
					printf( "Got steeringDeadZone: %f \n",steeringDeadZone);
				}


				attr = atributo->Attribute("maxSteeringbySecond");
				if(attr != NULL){
					printf( "Got maxSteeringbySecond: %s \n",attr);
					std::string fs(attr);
					maxSteeringbySecond = std::stof(fs);
					printf( "Got maxSteeringbySecond: %f \n",sight);
				}

				attr = atributo->Attribute("boundary");
				if(attr != NULL){
					printf( "Got boundary: %s \n",attr);
					std::string fs(attr);
					boundary = std::stof(fs);
					printf( "Got maxSteeringbySecond: %f \n",boundary);
				}


				attr = atributo->Attribute("centerOfBoundary");
				if(attr != NULL){
					printf( "Got center: %s \n",attr);
					sscanf (attr,"( %s , %s , %s )",aux[0],aux[1],aux[2]);
					centerOfBoundary.x=std::stof(aux[0]);
					centerOfBoundary.y=std::stof(aux[1]);
					centerOfBoundary.z=std::stof(aux[2]);
				}

				attr = atributo->Attribute("dim");
				if(attr != NULL){
					printf( "Got 2D: %s \n",attr);
					i_2D = atoi(attr);
					printf( "Got 2D: %d \n",i_2D);
				}

				attr = atributo->Attribute("coord2D");
				if(attr != NULL){
					printf( "Got 2DCoord: %s \n",attr);
					std::string fs(attr);
					coordSpace = std::stof(fs);
					printf( "Got 2DCoord: %f \n",coordSpace);
				}


				attr = atributo->Attribute("path_id");
				if(attr != NULL){
					printf( "Got PathID: %s \n",attr);
					id_caminho = string(attr);
					printf( "Got PathID: %d \n",id_caminho);
				}



				attr = atributo->Attribute("property");
				if(attr != NULL){
					char key[50],value[50];
					printf( "Got Property: %s \n",attr);
					string  skey(key),
							svalue(value);

					sscanf (attr,"( %s : %s )",key,value);
					properties[skey] = svalue;
					printf( "Got Property: %s : %s \n",skey.c_str(),svalue.c_str());
				}
			}

			AddBoid(b_id, vel, acceleration, i_2D, coordSpace, sight, space, centerOfBoundary, boundary, maxSteeringbySecond, steeringDeadZone, calcRotation, id_caminho, b_friends, properties);
			//Boid liderAux = Boid(100, idLider, 0);
			//liders.addBoid(liderAux);
		}

		else if(elemName == "Friends")
		{
			Boid *A,*B;

			int type;

			printf( "\n Got Obstacle\n");
			

			attr = elem->Attribute("A");
			if(attr != NULL){
				printf( "Got Friend: %s \n",attr);
				std::string friend_Id(attr);
				A = getBoid(friend_Id);
				printf( "Got Friend: %s \n",friend_Id.c_str());
			}

			attr = elem->Attribute("B");
			if(attr != NULL){
				printf( "Got Friend: %s \n",attr);
				std::string friend_Id(attr);
				B = getBoid(friend_Id);
				printf( "Got Friend: %s \n",friend_Id.c_str());
			}

			A->addFriend(B);

		}
		
		else if(elemName == "Obstacle")
		{
			Vec3 center;
			string id(NONE);
			float min[3],max[3],radius;
			char aux[3][10];

			int type;

			printf( "\n Got Obstacle\n");
			

			attr = elem->Attribute("center");
			if(attr != NULL){
				printf( "Got center: %s \n",attr);
				sscanf (attr,"( %s , %s , %s )",aux[0],aux[1],aux[2]);
				center.x=std::stof(aux[0]);
				center.y=std::stof(aux[1]);
				center.z=std::stof(aux[2]);
				type = SPHERE;
			}


			attr = elem->Attribute("radius");
			if(attr != NULL){
				printf( "Got radius: %s \n",attr);
				std::string fs(attr);
				radius=std::stof(fs);//this is much better way to do it
				//vel = atof(attr);
				printf( "Got radius: %f \n",radius);
			}

			attr = elem->Attribute("min");
			if(attr != NULL){
				printf( "Got min: %s \n",attr);
				std::string fs(attr);
				sscanf (attr,"( %s , %s , %s )",aux[0],aux[1],aux[2]);
				min[0]=std::stof(aux[0]);
				min[1]=std::stof(aux[1]);
				min[2]=std::stof(aux[2]);
				printf( "Got min: [%f,%f,%f] \n",min[0],min[1],min[2]);
				type = BOX;
				//printf( "Got min: [%f] \n",min[0]);
				//printf( "Got mins: [%s] \n",aux[0]);
			}
			
			attr = elem->Attribute("max");
			if(attr != NULL){
				printf( "Got max: %s \n",attr);
				std::string fs(attr);
				sscanf (attr,"( %s , %s , %s )",aux[0],aux[1],aux[2]);
				max[0]=std::stof(aux[0]);
				max[1]=std::stof(aux[1]);
				max[2]=std::stof(aux[2]);
				printf( "Got max: [%f,%f,%f] \n",max[0],max[1],max[2]);
				//printf( "Got min: [%f] \n",max[0]);
				//printf( "Got mins: [%s] \n",aux[0]);
			}
			attr = elem->Attribute("id");
			if(attr != NULL){
				printf( "Got id: %s \n",attr);
				id = string(attr);
				printf( "Got id: %s \n",id.c_str());

			}
			if(type == SPHERE)
			{
				Obstacle *toAdd = new Obstacle(id, center,radius);
				cenario.Add(toAdd,id);
			}
			else
			{
				Obstacle *toAdd = new Obstacle(id, min[0],min[1],min[2],max[0],max[1],max[2]);
				cenario.Add(toAdd,id);
			}


		}

		else if(elemName == "Path")
		{
			string idPath(NONE);
			std::vector<Vec3> path;
			Vec3 point;
			char xyz[3][10];
			printf( "\n Got Path\n");
			
			attr = elem->Attribute("id");
			if(attr != NULL){
				printf( "Got id: %s \n",attr);
				idPath = string(attr);
				printf( "Got id: %s \n",idPath.c_str());

			}

			for(TiXmlElement* atributo = elem->FirstChildElement(); atributo != NULL; atributo = atributo->NextSiblingElement()){
				attr = atributo->Attribute("point");
				if(attr != NULL){
					printf( "Got point: %s \n",attr);
					std::string fs(attr);
					sscanf (attr,"( %s , %s , %s )",xyz[0],xyz[1],xyz[2]);
					point.x=std::stof(xyz[0]);
					point.y=std::stof(xyz[1]);
					point.z=std::stof(xyz[2]);
					printf( "Got point: [%f,%f,%f] \n",point.x,point.y,point.z);
					path.push_back(point);
					printf("Path Size: %d \n", path.size());
				}
			}
			Caminho caminho = Caminho(path);
			paths.Add(caminho,idPath);
			printf("PathSize: %d\n", paths.getPath(idPath).getCaminho().size());

		}

	}
    

    doc.Clear();
	
	return;
}