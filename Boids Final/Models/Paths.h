#ifndef _PATHS
#define _PATHS

#include "Caminho.h"
#include <map>
#include <string>

using namespace std;


class Paths
{
	private:
		std::map<string,Caminho> paths_list;
		
	public:
		Paths::Paths();
		void Paths::Add(Caminho path, string id);
		Caminho Paths::getPath(string id);

};


#endif