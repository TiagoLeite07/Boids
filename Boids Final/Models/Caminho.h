#ifndef _CAMINHO
#define _CAMINHO

#include "Vec3.h"
#include <vector>



//std::vector<Vec3> catmullRomSplineTest(Vec3 p0, Vec3 p1, Vec3 p2, Vec3 p3);

class Caminho
{
	private:
		std::vector<Vec3> caminho;
		void Caminho::catmullRomSpline(Vec3 p0, Vec3 p1, Vec3 p2, Vec3 p3);
		void Caminho::juntaSplines(std::vector<Vec3> pontos);
	public:
		Caminho::Caminho(std::vector<Vec3> pontos);
		Caminho::Caminho();
		std::vector<Vec3> Caminho::getCaminho();

};


#endif