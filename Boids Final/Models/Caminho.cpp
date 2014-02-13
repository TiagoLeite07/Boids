#include "Caminho.h"

/* Coefficients for Matrix M */
#define M11	 0.0
#define M12	 1.0
#define M13	 0.0
#define M14	 0.0
#define M21	-0.5
#define M22	 0.0
#define M23	 0.5
#define M24	 0.0
#define M31	 1.0
#define M32	-2.5
#define M33	 2.0
#define M34	-0.5
#define M41	-0.5
#define M42	 1.5
#define M43	-1.5
#define M44	 0.5





std::vector<Vec3> catmullRomSplineTest(Vec3 p0, Vec3 p1, Vec3 p2, Vec3 p3)
{
	std::vector<Vec3> res;

	float p0x = p0.x;
	float p0y = p0.y;
	float p0z = p0.z;
	float p1x = p1.x;
	float p1y = p1.y;
	float p1z = p1.z;
	float p2x = p2.x;
	float p2y = p2.y;
	float p2z = p2.z;
	float p3x = p3.x;
	float p3y = p3.y;
	float p3z = p3.z;

	double c1,c2,c3,c4;

	float pontox,pontoy,pontoz;

	for(int k = 0;  k < 50; k++){    //50 points
		float t = k*0.02;  //Interpolation parameter

		c1 =  	       M12*p1x;
		c2 = M21*p0x           + M23*p2x;
		c3 = M31*p0x + M32*p1x + M33*p2x + M34*p3x;
		c4 = M41*p0x + M42*p1x + M43*p2x + M44*p3x;

		pontox = (((c4*t + c3)*t +c2)*t + c1);

		c1 =  	       M12*p1y;
		c2 = M21*p0y           + M23*p2y;
		c3 = M31*p0y + M32*p1y + M33*p2y + M34*p3y;
		c4 = M41*p0y + M42*p1y + M43*p2y + M44*p3y;

		pontoy = (((c4*t + c3)*t +c2)*t + c1);

		c1 =  	       M12*p1z;
		c2 = M21*p0z           + M23*p2z;
		c3 = M31*p0z + M32*p1z + M33*p2z + M34*p3z;
		c4 = M41*p0z + M42*p1z + M43*p2z + M44*p3z;

		pontoz = (((c4*t + c3)*t +c2)*t + c1);

		Vec3 ponto = Vec3(pontox,pontoy,pontoz);

		res.push_back(ponto);
	}
	return res;
}





void Caminho::catmullRomSpline(Vec3 p0, Vec3 p1, Vec3 p2, Vec3 p3)
{
	float p0x = p0.x;
	float p0y = p0.y;
	float p0z = p0.z;
	float p1x = p1.x;
	float p1y = p1.y;
	float p1z = p1.z;
	float p2x = p2.x;
	float p2y = p2.y;
	float p2z = p2.z;
	float p3x = p3.x;
	float p3y = p3.y;
	float p3z = p3.z;

	double c1,c2,c3,c4;

	float pontox,pontoy,pontoz;

	for(int k = 0;  k < 50; k++){    //50 points
		float t = k*0.02;			//Interpolation parameter

		c1 =  	       M12*p1x;
		c2 = M21*p0x           + M23*p2x;
		c3 = M31*p0x + M32*p1x + M33*p2x + M34*p3x;
		c4 = M41*p0x + M42*p1x + M43*p2x + M44*p3x;

		pontox = (((c4*t + c3)*t +c2)*t + c1);

		c1 =  	       M12*p1y;
		c2 = M21*p0y           + M23*p2y;
		c3 = M31*p0y + M32*p1y + M33*p2y + M34*p3y;
		c4 = M41*p0y + M42*p1y + M43*p2y + M44*p3y;

		pontoy = (((c4*t + c3)*t +c2)*t + c1);

		c1 =  	       M12*p1z;
		c2 = M21*p0z           + M23*p2z;
		c3 = M31*p0z + M32*p1z + M33*p2z + M34*p3z;
		c4 = M41*p0z + M42*p1z + M43*p2z + M44*p3z;

		pontoz = (((c4*t + c3)*t +c2)*t + c1);

		Vec3 ponto = Vec3(pontox,pontoy,pontoz);

		caminho.push_back(ponto);
	}

}

void Caminho::juntaSplines(std::vector<Vec3> pontos)
{
	Vec3 ponto0,ponto1,ponto2,ponto3;
	int i = 0;
	int size = pontos.size();
	while(i < size-3)
	{
		ponto0 = pontos.at(i++);
		ponto1 = pontos.at(i++);
		ponto2 = pontos.at(i++);
		ponto3 = pontos.at(i);

		catmullRomSpline(ponto0,ponto1,ponto2,ponto3);

		i = i-2;
	}
}

Caminho::Caminho(std::vector<Vec3> pontos)
{
	juntaSplines(pontos);
}

Caminho::Caminho()
{

}

std::vector<Vec3> Caminho::getCaminho()
{
	return caminho;
}