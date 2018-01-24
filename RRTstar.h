#ifndef RRTSTAR_H
#define RRTSTAR_H
#include "RRT.h"

class RRTstar :public RRT
{
public:
	void createTree(); //override
	RRTstar(const char * filename,point start,point goal):RRT(filename,start,goal){};

protected:

private:
	int getCost(tree_node * t);
};



#endif