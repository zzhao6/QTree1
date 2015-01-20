#include "TimeSlice.h"

TimeSlice::TimeSlice(Node& rootNode)
{
	this->tsPara = rootNode.getPara();
	this->NodeList.clear();
	NodeList.push_back(rootNode);
}


TimeSlice::TimeSlice(Para& para)
{
	this->NodeList.clear();
	this->tsPara = para;
}


TimeSlice* TimeSlice::genNextTS(Para& nextPara)
{
	TimeSlice* nextTS = new TimeSlice(nextPara);

	list<Node>::iterator itNode;
	double currGrid = this->tsPara.grid;

	for (itNode = NodeList.begin(); itNode != NodeList.end(); itNode++)
	{
		vector<Node> tmpChildVec = (*itNode).genChildren(nextPara);
		nextTS->mergeTS(tmpChildVec, *itNode);
	}

	return nextTS;
}


void TimeSlice::mergeTS(vector<Node>& childVec, Node& parentNode)
{
	// if originally the TS is empty
	// copy the 4 children to TS, then change the ptrVec of parentNode
	if (NodeList.empty())
	{
		vector<Node>::iterator it;
		for (it = childVec.begin(); it != childVec.end(); it++)
		{
			NodeList.push_back(*it);
		}

		// re-assign pointer
		vector<Node*> newNodePtrVec = parentNode.getNodePtrs();
		int idxVec = 0;
		list<Node>::iterator itNL;
		for (itNL = NodeList.begin(); itNL != NodeList.end(); itNL++)
		{
			newNodePtrVec[idxVec] = &(*itNL);
			idxVec++;
		}
		parentNode.setNodePtrs(newNodePtrVec);

		///////////////////////////////////////////////
		//check pointer
		// 		list<Node>::iterator itCheck = NodeList.begin();
		// 		vector<Node>::iterator itCheckVec = childVec.begin();
		// 		
		// 		Node* NLcheck0 = &(*itCheck);
		// 		itCheck++;
		// 		Node* NLcheck1 = &(*itCheck);
		// 		itCheck++;
		// 		Node* NLcheck2 = &(*itCheck);
		// 		itCheck++;
		// 		Node* NLcheck3 = &(*itCheck);
		// 
		// 		Node* VecCheck0 = &(*itCheckVec);
		// 		itCheckVec++;
		// 		Node* VecCheck1 = &(*itCheckVec);
		// 		itCheckVec++;
		// 		Node* VecCheck2 = &(*itCheckVec);
		// 		itCheckVec++;
		// 		Node* VecCheck3 = &(*itCheckVec);		
		// check pointer end
		///////////////////////////////////////////////

		return;
	}
	// find out the UNION of childVec and NodeList

	list<Node>::reverse_iterator itOriginNL = NodeList.rbegin();
	//vector<Node>::iterator itChildVec = childVec.begin();
	// int childVecIdx = 0;    // an index tracking the index of childVec
	// so we can change the childNodePtr through this
	if (childVec[0].getX() < itOriginNL->getX())
	{
		NodeList.push_back(childVec[0]);
		NodeList.push_back(childVec[1]);
		NodeList.push_back(childVec[2]);
		NodeList.push_back(childVec[3]);
	}
	else if (childVec[0].getX() == itOriginNL->getX())
	{
		NodeList.push_back(childVec[1]);
		NodeList.push_back(childVec[2]);
		NodeList.push_back(childVec[3]);
	}
	else if (childVec[1].getX() == itOriginNL->getX())
	{
		NodeList.push_back(childVec[2]);
		NodeList.push_back(childVec[3]);
	}
	else if (childVec[2].getX() == itOriginNL->getX())
	{
		NodeList.push_back(childVec[3]);
	}
	else if (childVec[3].getX() == itOriginNL->getX())
	{
		// do nothing
	}
	else
	{
		//		cout << "merge error" << endl;
		//system("pause");
		//exit(1);
		//		cout << "Check merge error" << endl;
	}

	vector<Node*> newNodePtrVec = parentNode.getNodePtrs();
	list<Node>::reverse_iterator rev_itNL = NodeList.rbegin();

	newNodePtrVec[3] = &(*rev_itNL);
	rev_itNL++;
	newNodePtrVec[2] = &(*rev_itNL);
	rev_itNL++;
	newNodePtrVec[1] = &(*rev_itNL);
	rev_itNL++;
	newNodePtrVec[0] = &(*rev_itNL);

	parentNode.setNodePtrs(newNodePtrVec);

	///////////////////////////////////////////////
	//check pointer
	// 	list<Node>::iterator itCheck = NodeList.begin();
	// 	vector<Node>::iterator itCheckVec = childVec.begin();
	// 
	// 	Node* NLcheck0 = &(*itCheck);
	// 	itCheck++;
	// 	Node* NLcheck1 = &(*itCheck);
	// 	itCheck++;
	// 	Node* NLcheck2 = &(*itCheck);
	// 	itCheck++;
	// 	Node* NLcheck3 = &(*itCheck);
	// 
	// 	Node* VecCheck0 = &(*itCheckVec);
	// 	itCheckVec++;
	// 	Node* VecCheck1 = &(*itCheckVec);
	// 	itCheckVec++;
	// 	Node* VecCheck2 = &(*itCheckVec);
	// 	itCheckVec++;
	// 	Node* VecCheck3 = &(*itCheckVec);		
	// check pointer end
	///////////////////////////////////////////////
}


void TimeSlice::optAllNodes()
{
	list<Node>::iterator itNL;
	itNL = NodeList.begin();

	// vector of child Node ptr of this parent Node
	vector<Node*> tmpNodePtr = itNL->getNodePtrs();

	// if this child Node ptr is NULL, means these Nodes belongs to
	// the last TS
	if (tmpNodePtr[0] == NULL)
	{
		for (itNL; itNL != NodeList.end(); itNL++)
		{
			itNL->calcPayOff();
		}
	}
	else {
		for (itNL; itNL != NodeList.end(); itNL++)
		{
			itNL->baCalc();
		}
	}
}

double TimeSlice::getGridTS()
{
	return this->tsPara.grid;
}

list<Node> TimeSlice::getNodeList()
{
	return NodeList;
}

unsigned int TimeSlice::getNodeNumber()
{
	return NodeList.size();
}

void TimeSlice::displayTSNodes()
{
	list<Node>::iterator itNL;
	for (itNL = NodeList.begin(); itNL != NodeList.end(); itNL++)
	{
		cout << itNL->getX() << "\t";
	}
	cout << endl;
}