#include "Obstacle.h"



	Obstacle::Obstacle()
	{
	
	}

	Obstacle::Obstacle(string o_id, float a_minX, float a_minY, float a_minZ, float a_maxX, float a_maxY, float a_maxZ)
	{
		bMin = Vec3(a_minX,a_minY,a_minZ);
		bMax = Vec3(a_maxX,a_maxY,a_maxZ);
		o_type = BOX;
		o_center = bMin+(bMax-bMin)/2;
		o_radius = (bMax-o_center).length();
		id = o_id;
	}


	Obstacle::Obstacle(Vec3 i_min, Vec3 i_max)
	{
		bMin = i_min;
		bMax = i_max;	
		o_type = BOX;
		o_center = bMin+(bMax-bMin)/2;
		o_radius = (bMax-o_center).length();
		id = NONE;
	}

	Obstacle::Obstacle(string o_id, Vec3 i_min, Vec3 i_max)
	{
		bMin = i_min;
		bMax = i_max;	
		o_type = BOX;
		o_center = bMin+(bMax-bMin)/2;
		o_radius = (bMax-o_center).length();
		id = o_id;
	}


	Obstacle::Obstacle(Vec3 center, float radius)
	{
		o_center = center;
		o_radius = radius;
		o_type = SPHERE;
		id = NONE;

		bMin = o_center - o_radius;
		bMax = o_center + o_radius;
	}

	Obstacle::Obstacle(string o_id, Vec3 center, float radius)
	{
		o_center = center;
		o_radius = radius;
		o_type = SPHERE;
		id = o_id;

		bMin = center - radius;
		bMax = center + radius;
	}



	float squared(float v)
	{
		return v * v;
	}

	// ainda não está a funcionar direito			
	// assumindo que Bmin e Bmax estão "ordenados"
	bool BoxIntersectsSphereA(Vec3 Bmin, Vec3 Bmax, Vec3 S, float r)
	{		  
		float dist_squared = r * r;

		if (S.x < Bmin.x)
			dist_squared -= squared(S.x - Bmin.x);
		else if (S.x > Bmax.x)
			dist_squared -= squared(S.x - Bmax.x);
		if (S.y < Bmin.y)
			dist_squared -= squared(S.y - Bmin.y);
		else if (S.y > Bmax.y)
			dist_squared -= squared(S.y - Bmax.y);
		if (S.z < Bmin.z)
			dist_squared -= squared(S.z - Bmin.z);
		else if (S.z > Bmax.z)
			dist_squared -= squared(S.z - Bmax.z);

		return dist_squared > 0;
	}

	bool BoxIntersectsSphere(Vec3 Bmin, Vec3 Bmax, Vec3 C, float r) {
	  float r2 = r * r,
	  dmin = 0;
		if( C.x < Bmin.x ) dmin += squared( C.x - Bmin.x );
		else if( C.x > Bmax.x ) dmin += squared( C.x - Bmax.x );  
		if( C.y < Bmin.y ) dmin += squared( C.y - Bmin.y );
		else if( C.y > Bmax.y ) dmin += squared( C.y - Bmax.y );  
		if( C.z < Bmin.z ) dmin += squared( C.z - Bmin.z );
		else if( C.z > Bmax.z ) dmin += squared( C.z - Bmax.z );  
	  
	  return dmin <= r2;
	}


	bool BoxIntersectsBox(Vec3 BminA, Vec3 BmaxA, Vec3 BminB, Vec3 BmaxB)
	{	
		if( BmaxA.x < BminB.x || BminA.x > BmaxB.x ) return false;
		if( BmaxA.y < BminB.y || BminA.y > BmaxB.y ) return false;
		if( BmaxA.z < BminB.z || BminA.z > BmaxB.z ) return false;

		return true;
	}

	bool Obstacle::intersect(Obstacle *other)
	{

		if(o_type==SPHERE && other->o_type==SPHERE)
		{
			//printf("SPHERE\n");
			Vec3 distance =  o_center - other->o_center;

			float length = distance.length();

			// Sum of the radiuses
			float sumradius = o_radius + other->o_radius;

			if (length <= sumradius)
			{
					return true;
			}
		
			return false;

		}

		else if (o_type==SPHERE && other->o_type==BOX)
		{
			return BoxIntersectsSphere(other->bMin,other->bMax,o_center,o_radius);
		}

		else if (o_type==BOX && other->o_type==SPHERE)
		{
			return BoxIntersectsSphere(bMin,bMax,other->o_center,other->o_radius);
		}

		else
		{
			return BoxIntersectsBox(bMin,bMax,other->bMin,other->bMax);
		}

	}



	Vec3 Obstacle::getCenter()
	{
		return o_center;
	}

	Vec3 Obstacle::getMin()
	{
		return bMin;
	}

	Vec3 Obstacle::getMax()
	{
		return bMax;
	}

	float Obstacle::getRadius()
	{
		return o_radius;
	}

	string Obstacle::getId()
	{
		return id;
	}

