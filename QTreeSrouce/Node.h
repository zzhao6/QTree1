#ifndef NODE_H_
#define NODE_H_


#include "StdAfx.h"

class Node
{
private:
	double x;				// return value
	double optValueNode;	// option value
	double optValueNode_variance_std;
	double optValueNode_variance_sim;
	double q;				// q in the paper
	Para nodePara;

	enum Case
	{
		caseNA = 0,
		case1,
		case2
	}myCase;

	vector<Node*> childNodePtrs;	// vector of pointers points to children Nodes
	double probArr[NCHILDREN];		// probs and the x value of 4 children

public:
	Node(){};
	Node(Para& para);
	~Node(){};

	vector<Node> genChildren(Para& nextPara);

	// payoff
	void baCalc();		// the optionValueNode must be computed through one parent Node and four children ptrs
	void calcPayOff();	// if Node belongs to the last TS, use this function

	void baCalc_variance_std();	//standard return variance
	void baCalc_variance_sim(); //


	// get functions
	double getOpt();
	double getOpt_variance_std();
	double getOpt_variance_sim();
	double getX();
	Para getPara();
	vector<Node*> getNodePtrs();
	//float getGrid();
	void setNodePtrs(vector<Node*>&);
	//void setGrid(float&);
};

#endif
