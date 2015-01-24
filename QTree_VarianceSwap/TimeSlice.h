#ifndef TIMESLICE_H_
#define TIMESLICE_H_

#include "StdAfx.h"
#include "Node.h"

class TimeSlice
{
private:
	list<Node> NodeList;
	Para tsPara;

public:
	TimeSlice(){};
	TimeSlice(Node& rootNode);	// root TS
	TimeSlice(Para& para);					// non-root TS
	~TimeSlice(){};

public:
	// generate child node for all node in this TimeSlice
	// and return a new TS
	TimeSlice* genNextTS(Para& para);

	// traverse all nodes for the payoff
	void optAllNodes();

	void optAllNodes_variance_std();
	void optAllNodes_variance_sim();

	// a pack of 4 children Nodes will be pushed into this TS NodeList
	// and it will be sorted  according to my algo
	// The second parameter is the parent Node of the child Node list.
	void mergeTS(vector<Node>& childVec, Node&);

	// get functions
	double getGridTS();
	list<Node> getNodeList();
	
	// for display
	unsigned int getNodeNumber();
	//display all TimeSlice
	void displayTSNodes();
};

#endif
