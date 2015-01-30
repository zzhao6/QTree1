#include "Node.h"

Node::Node(Para& para)
{
	this->nodePara = para;
	
	try
	{
		this->x = log(nodePara.StartPrice);
	}
	catch (int e)
	{
		this->x = 0;
	}
	
	this->q = 0;
	this->optValueNode = 0;
	this->myCase = caseNA;

	childNodePtrs.reserve(NCHILDREN);
	for (int i = 0; i < NCHILDREN; i++)
	{
		// initialize ptrs and probs to the 4 successors
		Node* tmpNodePtr = NULL;
		childNodePtrs.push_back(tmpNodePtr);
		probArr[i] = 0;
	}
}

vector<Node> Node::genChildren(Para& nextPara)
{
	double xChildren[4];
	int j = int(ceil(x / nextPara.grid));

	xChildren[0] = (j + 1) * nextPara.grid;
	xChildren[1] = j * nextPara.grid;
	xChildren[2] = (j - 1) * nextPara.grid;
	xChildren[3] = (j - 2) * nextPara.grid;

	double Q = (x - xChildren[2]) / nextPara.grid;
	if (Q > 0.5) {
		this->q = Q - 1;
		myCase = case1;
	}
	else if (Q < 0.5) {
		this->q = Q;
		myCase = case2;
	}
	else 
	{   
		// Q == 0.5
		this->q = 0;
		myCase = case2;
	}

	double p = this->nodePara.Prob;	// input probability for 4 successors

	if (myCase == case1) {
		this->probArr[0] = 0.5 * (1 + q + q*q) - p;
		this->probArr[1] = 3 * p - q*q;
		this->probArr[2] = 0.5 * (1 - q + q*q) - 3 * p;
		this->probArr[3] = p;
	}
	else if (myCase == case2) {
		this->probArr[0] = p;
		this->probArr[1] = 0.5 * (1 + q + q*q) - 3 * p;
		this->probArr[2] = 3 * p - q*q;
		this->probArr[3] = 0.5 * (1 - q + q*q) - p;
	}
	else { // theCase == caseNA
		try
		{
			throw 1;
		}
		catch (int e)
		{
			cout << "Case NA error, Exception No." << e << "!\n";
		}
	}

	vector<Node> childNodes;
	Node tmpNode(nextPara);
	
	for (int i = 0; i < NCHILDREN; i++)
	{
		tmpNode.x = xChildren[i] + nextPara.drift;
		childNodes.push_back(tmpNode);
	}

	return childNodes;
}

// two functions calculating Payoff
// for Nodes not in the last TS
void Node::baCalc()
{
	double optResult = 0;
	optResult += this->probArr[0] * (this->childNodePtrs[0]->optValueNode);
	optResult += this->probArr[1] * (this->childNodePtrs[1]->optValueNode);
	optResult += this->probArr[2] * (this->childNodePtrs[2]->optValueNode);
	optResult += this->probArr[3] * (this->childNodePtrs[3]->optValueNode);

	// discount
	// this->optValueNode = optResult * exp(-INTEREST*T/STEPS);
	// discount in class Path
	this->optValueNode = optResult;
}


//// for Nodes in the last TS
//void Node::calcPayOff()
//{
//	double price = exp(x);
//	optValueNode = max(price - nodePara.Strike, 0.0);		// European Call
//	//optValueNode = max(this->nodePara.Strike - price, 0.0);	// European Put
//}


void Node::calcPayOff()
{
	double price = 0.0;
	if (this->nodePara.Rho == 0)
	{
		price = exp(x);
	}
	else
	{
		//cout << "Rho is not zero" << endl;
		// Rho != 0, generated Y_t and get back to X_t here
		price = exp(x + nodePara.Rho * nodePara.PrevSigma / nodePara.VolOfVol);
	}
	//optValueNode = max(price - nodePara.Strike, 0.0);			// European Call
	optValueNode = max(this->nodePara.Strike - price, 0.0);	// European Put
}


double Node::getOpt(){
	return optValueNode;
}

double Node::getX(){
	return x;
}

Para Node::getPara()
{
	return this->nodePara;
}

vector<Node*> Node::getNodePtrs() {
	return childNodePtrs;
}

void Node::setNodePtrs(vector<Node*>& newNodePtrs_) {
	childNodePtrs = newNodePtrs_;
}