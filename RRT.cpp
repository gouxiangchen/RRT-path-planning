#include "RRT.h"
#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <fstream>


RRT::RRT(const char * filename,point start,point goal):K(10000),basemap(filename),minstep(20)		//K 撒点数目， minstep树节点之间的最短距离
{
	using namespace cv;
	allnode.push_back(start);
	tree_node * t=new tree_node(start);
	graph.push_back(t);
	img= imread(filename);
	this->start=start;
	this->goal=goal;
	if (basemap.transformMapToGrid())
	{
		map=basemap.getGridMap2D();
		sizeX=basemap.getSizeX();
		sizeY=basemap.getSizeY();
	}

	
}

int RRT::getDistance(const point p1,const point p2)
{
	return (int)sqrt(double((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y)));
}

bool RRT::if_line(const point a,const point b)
{
	int ** walkability = map;
	line.clear();
	int dx=abs(a.x-b.x);
	int dy=abs(a.y-b.y);
	double k;
	if (dx != 0)
	{
		k=double(b.y-a.y)/double(b.x-a.x);
	}
	else if (dy ==0)
	{
		k=1;
	}
	else
	{
		k=99999;
	}
	
	point temp;

	if (dx>=dy)
	{
		if (a.x<b.x)
		{
			temp =a;
		}
		else 
			temp=b;
		for (int i=1;i<dx+1;i++)
		{
			int x=temp.x+i;
			int y=temp.y+k*i;
			point pt(x,y);
			line.push_back(pt);
		}
	}
	else
	{
		if (a.y<b.y)
		{
			temp =a;
		}
		else 
			temp=b;
		for (int i=1;i<dy+1;i++)
		{
			int x=temp.x+i/k;
			int y=temp.y+i;

			point pt(x,y);
			line.push_back(pt);
		}
	}
	int len=line.size();
	for (int i=0;i<len;i++)
	{
		if (1 == walkability[line[i].x][line[i].y])
		{
			return false;
		}
	}
	return true;
}

void RRT::createTree()
{
	using namespace std;
	using namespace cv;
	int i=0;
	srand(time(0));
	bool findpath=true;
	while (i++ < K)
	{
		
		int x=rand()%sizeX;
		int y=rand()%sizeY;
		
		while(map[x][y] != 0)
		{
			x=rand()%sizeX;
			y=rand()%sizeY;
		}
		point temp(x,y);
		int flag=0;
		int dist=999999;
		point p;
		if (findpath)
		{
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
		}
		else
		{
			for (int i=0;i<allnode.size();i++)
			{
				int d = getDistance(temp,allnode[i]);
				if (d< dist)
				{
					dist=d;
					flag=i;
				}
			}
			p.x=allnode[flag].x;
			p.y=allnode[flag].y;
		}
		

		
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
			circle( img,Point(line[i].x,line[i].y), 0.1,  Scalar(0,10,255), 0.1, 1,0 );
		}

		if (num != 0)
		{
			if (findpath)
			{
				tree_node * t=new tree_node(temp,graph[flag]);
				graph.push_back(t);
			}
			
			
			if (findpath && (getDistance(temp,goal) < minstep+10 ) && if_line(temp,goal))
			{
				findpath=false;
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
			}
			allnode.push_back(temp);
		}
		
		

	}
	
	
}

bool RRT::getPath()
{
	using namespace cv;
	if (path.size()== 0)
	{
		return false;
	}
	else
	{
		for (int i=0;i<path.size()-1;i++)
		{

			cv::line(img,Point(path[i].x,path[i].y),Point(path[i+1].x,path[i+1].y),Scalar(255,0,0),2);
		}
		return true;
	}

}

bool RRT::getPath_repair()
{
	using namespace cv;
	if (path_repair.size()== 0)
	{
		return false;
	}
	else
	{
		for (int i=0;i<path_repair.size()-1;i++)
		{
			
			cv::line(img,Point(path_repair[i].x,path_repair[i].y),Point(path_repair[i+1].x,path_repair[i+1].y),Scalar(255,0,0),2);
		}
		return true;
	}
		
}

void RRT::showResult(const char * filename)
{
	using namespace std;
	using namespace cv;
	namedWindow("map");
	imshow("map",img);
	imwrite(filename,img);
	waitKey();

/*	ofstream out;

	out.open(filename,ios::out);
	if (out.is_open())
	{
		//char a;
		//in>>a;
		for (int i=0;i<path.size();i++)
		{
			out<<path[i].x<<" "<<path[i].y<<"\n";
		}


		out.close();
	}*/
}

RRT::~RRT()
{
	for (int i=1;i<graph.size()-1;i++)
	{
		delete graph[i];
	}
}

void RRT::repair()
{
	

}