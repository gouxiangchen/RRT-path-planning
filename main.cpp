#include "RRT.h"
#include "map_pgm.h"
#include "RRTstar.h"

// created by chen gouxiang

int main()
{
	RRT	map("intel_binary.jpg",point(652,1158),point(498,312));
	map.createTree();
	map.getPath();
	map.showResult("RRT.jpg");
	return 0;
}