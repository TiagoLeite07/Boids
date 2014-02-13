#ifndef _VEC3
#define _VEC3


class Vec3  
{

    public:

		float x,y,z;

        Vec3();
        Vec3(float x, float y, float z);
        Vec3(const Vec3 &v);

		bool operator==(Vec3 v);
        Vec3 operator+(Vec3 v);
        Vec3 operator-(Vec3 v);
        Vec3 operator*(Vec3 v);
        Vec3 operator/(Vec3 v);
        Vec3 operator+(float scalar);
        Vec3 operator-(float scalar);
        Vec3 operator*(float scalar);
        Vec3 operator/(float scalar);
		
        Vec3 operator-(void);	// Simetric
		//
		Vec3 cross(Vec3 v);
		Vec3 scalarMult(float a);
		float innerProduct(Vec3  v);
        float dot(Vec3 v);
        float length();
		void normalize();
		void copy(const Vec3 v);
		void set(float x,float y, float z);
		float distance(Vec3 v);
		void print();

};


#endif

