#ifndef MAPSEARCHNODE_H
#define MAPSEARCHNODE_H

#include "../../core/gamingarea.h"
#include "../../def/point.h"
#include "stlastar.h" // See header for copyright and usage information


class MapSearchNode
{
public:
	unsigned int x;	 // the (x,y) positions of the node
	unsigned int y;	
	
	MapSearchNode() { x = y = 0; }
	MapSearchNode( unsigned int px, unsigned int py ) { x=px; y=py; }

	float GoalDistanceEstimate( MapSearchNode &nodeGoal );
	bool IsGoal( MapSearchNode &nodeGoal );
    bool GetSuccessors( AStarSearch<MapSearchNode, CGamingArea> *astarsearch, MapSearchNode *parent_node );
    float GetCost( AStarSearch<MapSearchNode, CGamingArea> *astarsearch, MapSearchNode &successor );
	bool IsSameState( MapSearchNode &rhs );

	void PrintNodeInfo(); 

    unsigned int GetNodePositionX();
    unsigned int GetNodePositionY();
};

#endif
