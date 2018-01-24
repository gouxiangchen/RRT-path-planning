#include "RRTstar.h"
#include <time.h>


int RRTstar::getCost(tree_node * t)
{
	if (t->father == NULL)
	{
		return 0;
	}
	else
	{
		return (getDistance(t->pos,t->father->pos)+ getCost(t->father));
	}
}

void RRTstar::createTree()
{
	using namespace std;
	using namespace cv;
	int i=0;
	srand(time(0));
	bool findpath=true;
	std::vector<tree_node * > isline;
	while(i++ <K  && findpath)
	{
		isline.clear();
		int x=rand()%sizeX;
		int y=rand()%sizeY;

		while(map[x][y] != 0)
		{
			x=rand()%sizeX;
			y=rand()%sizeY;
		}
		point temp(x,y);
		int flag=-1;

		int dist=999999;
		point p=start;

		for (int i=0;i<graph.size();i++)
		{
			int d=getDistance(temp,graph[i]->pos);
			if (d< dist)
			{
				dist=d;
				flag=i;
			}
		}
		p.x=graph[flag]->pos.x;
		p.y=graph[flag]->pos.y;
		
		int num=9999;
		int pos=0;
		while (! if_line(p,temp) && num >minstep)
		{
			num=line.size();
			pos = rand()%num;
			num=pos;
			temp=line[pos];
		}
		num=line.size();
		if (num < minstep || ! if_line(p,temp))
		{
			continue;
		}
		for (int i=0;i<num;i++)
		{
			circle( img,Point(line[i].x,line[i].y), 0.1,  Scalar(0,10,255), 0.1, 1, 0 );
		}

		int cost = getCost(graph[flag]);
		if ( (cost+getDistance(temp,p)+getDistance(temp,goal)) < 1.1*(cost + getDistance(graph[flag]->pos,goal)) )
		{
			tree_node * t=new tree_node(temp,graph[flag]);
			graph.push_back(t);
			if ((getDistance(temp,goal) < minstep+10 ) && if_line(temp,goal))
			{
				
				path.push_back(goal);
				tree_node * temp_ = graph[graph.size()-1];
				circle( img,Point(start.x,start.y), 3,  Scalar(0,0,0), 3, 1, 0 );
				circle( img,Point(goal.x,goal.y), 3,  Scalar(0,0,0), 3, 1, 0 );

			/*	for (int i=0;i<graph.size();i++)
				{
					if (temp_ != NULL)
					{
						path.push_back(temp_->pos);
						temp_=temp_->father;
					}
					
				}*/

				for (;temp_!=NULL;temp_=temp_->father)
				{
					path.push_back(temp_->pos);
				}
				findpath=false;
			}
		}

		

	}
}