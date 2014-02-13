#include "Paths.h"


Paths::Paths()
{

}
		

void Paths::Add(Caminho path, string id)
{
	paths_list[id] = path;
}

Caminho Paths::getPath(string id)
{
	Caminho ptr;
	//try
	{
		ptr = paths_list.at(id);
	}
	//catch (const std::out_of_range& oor)
	{
	//	printf("getBoid::Out of Range error: %s \n ", oor.what());
	}
	return ptr;
}

