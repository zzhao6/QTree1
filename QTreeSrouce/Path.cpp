#include "Path.h"

Path::Path(Para& outPara, int sigmaType)
{
	this->pathPara = outPara;
	this->optValuePath = 0;
	//this->paraVec.resize(pathPara.Steps + 1);

	this->paraVec = vector<Para>(pathPara.Steps + 1, pathPara);

	//for (int i = 0; i < pathPara.Steps + 1;i++)
	//{
	//	this->paraVec[i] = this->pathPara;
	//}
	
	//generate sigma vector
	switch (sigmaType)
	{
	case 0:	// type 0, fixed sigma
		genSigmaConstant();
		break;
	case 1:
		genSigmaHeston();
		//displaySigma();
		break;
	default:
		try
		{
			throw 3;
		}
		catch (int e)
		{
			cout << "Sigma Type Error in Path!!" << endl;
		}
	}

	// calculate gridVec
	
	// add dt to path
	double dt = 0.0;
	try
	{
		dt = pathPara.Maturity / pathPara.Steps;
	}
	catch (int e)
	{
		cout << "Unable to calculate dt" << endl;
		throw 5;
	}

	//pathPara.grid = paraVec[0].Sigma * sqrt(dt);
	//pathPara.drift = (pathPara.Interest - paraVec[0].Sigma * paraVec[0].Sigma / 2) * dt;

	//for (int i = 1; i < pathPara.Steps + 1; i++)	// totally 
	//{
	//	paraVec[i].grid = paraVec[i].Sigma * sqrt(dt);
	//	paraVec[i].drift = (pathPara.Interest - paraVec[i].Sigma * paraVec[i].Sigma / 2) * dt;
	//}


	///////////////////////////////////////////////////////////////
	// test drift and grid functions
	this->genDriftsAndGrids();
	///////////////////////////////////////////////////////////////

	// seeds inside paths are obsolete
	//srand(pathSeed);
}

/////////////////////////////////////////
void Path::genDriftsAndGrids()
{
	double dt = this->pathPara.Maturity / this->pathPara.Steps;

	if (this->pathPara.Rho == 0)
	{
		// tree and vol are uncorrelated
		pathPara.grid = paraVec[0].Sigma * sqrt(dt);
		pathPara.drift = (pathPara.Interest - paraVec[0].Sigma * paraVec[0].Sigma / 2) * dt;

		for (int i = 1; i < pathPara.Steps + 1; i++)
		{
			paraVec[i].grid = paraVec[i].Sigma * sqrt(dt);
			paraVec[i].drift = (pathPara.Interest - paraVec[0].Sigma * paraVec[0].Sigma / 2) * dt;
		}
	}
	else
	{
		// tree and vol are correlated
		pathPara.grid = sqrt(1 - pathPara.Rho * pathPara.Rho) * paraVec[0].Sigma * sqrt(dt);

		pathPara.drift =
			(
			pathPara.Interest - paraVec[0].Sigma * paraVec[0].Sigma / 2 -
			pathPara.Rho * pathPara.Kappa * (pathPara.Theta - paraVec[0].Sigma * paraVec[0].Sigma) / pathPara.VolOfVol
			) * dt;

		for (int i = 1; i < pathPara.Steps + 1; i++)
		{
			paraVec[i].grid = sqrt(1 - pathPara.Rho * pathPara.Rho) * paraVec[i].Sigma * sqrt(dt);
			paraVec[i].drift =
				(
				pathPara.Interest - paraVec[i].Sigma * paraVec[0].Sigma / 2 -
				pathPara.Rho * pathPara.Kappa * (pathPara.Theta - paraVec[i].Sigma * paraVec[i].Sigma) / pathPara.VolOfVol
				) * dt;
		}
	}



	for (int i = 0; i < pathPara.Steps; i++)
	{
		LOGGER->Log("%f\n%f\n", paraVec[i].drift, paraVec[i].grid);
	}
}


Path::~Path()
{
	for (int i = 0; i < TSPtrVec.size(); i++)
	{
		delete TSPtrVec[i];
	}
}

void Path::buildAndCalc()
{
	this->buildTree();
	this->calcOptValue();
}

void Path::buildTree()
{
	Node rootNode(this->pathPara);
	TimeSlice* rootTS = new TimeSlice(rootNode);

	TSPtrVec.clear();
	TSPtrVec.push_back(rootTS);

	vector<TimeSlice*>::reverse_iterator r_itTS;
	for (int iSteps = 0; iSteps < this->pathPara.Steps; iSteps++)
	{
		Para nextPara = paraVec[iSteps + 1];
		r_itTS = TSPtrVec.rbegin();	// always point to the last time slice

		TimeSlice* nextTSPtr = (*r_itTS)->genNextTS(nextPara);
		TSPtrVec.push_back(nextTSPtr);
	}
}

void Path::calcOptValue()
{
	vector<TimeSlice*>::reverse_iterator rev_itTSL;

	// calculate option value through all TS
	for (rev_itTSL = TSPtrVec.rbegin(); rev_itTSL != TSPtrVec.rend(); rev_itTSL++)
	{
		(*rev_itTSL)->optAllNodes();
	}
	vector<TimeSlice*>::iterator itTSL = TSPtrVec.begin();
	list<Node> rootNodeList = (*itTSL)->getNodeList();
	Node rootNode = rootNodeList.front();
	this->optValuePath = rootNode.getOpt() * exp( - this->pathPara.Interest * this->pathPara.Maturity);
}

void Path::genSigmaHeston()
{
	paraVec[0].Sigma = pathPara.Vol0;
	for (int i = 1; i < pathPara.Steps + 1; i++)
	{
		double epsilon = gaussrand();
		paraVec[i].Sigma = paraVec[i - 1].Sigma + pathPara.Kappa* (pathPara.Theta - paraVec[i - 1].Sigma) * pathPara.Maturity / pathPara.Steps +
						   pathPara.VolOfVol*sqrt(pathPara.Maturity / pathPara.Steps * paraVec[i - 1].Sigma) * epsilon;
	}

	paraVec[0].Sigma = sqrt(paraVec[0].Sigma);
	paraVec[0].PrevSigma = -1;	// TODO: don't how to value this member
	for (int i = 1; i < pathPara.Steps + 1; i++)
	{
		paraVec[i].Sigma = sqrt(paraVec[i].Sigma);
		paraVec[i].PrevSigma = paraVec[i-1].Sigma;
	}
}

void Path::genSigmaConstant()
{
	for (int i = 1; i < pathPara.Steps + 1; i++)
	{
		// sigma vector is a constant, equals to SIGMA
		paraVec[i].Sigma = paraVec[i - 1].Sigma;
	}
}

// get functions
vector<TimeSlice*> Path::getTSVec(){
	return TSPtrVec;
}

double Path::getOptionValue()
{
	return optValuePath;
}

void Path::displayPath()
{
	vector<TimeSlice*>::iterator itPath;
	for (itPath = TSPtrVec.begin(); itPath != TSPtrVec.end(); itPath++)
	{
		(**itPath).displayTSNodes();
	}
}

void Path::displayNodeCnt()
{
	vector<TimeSlice*>::iterator itPath;
	for (itPath = TSPtrVec.begin(); itPath != TSPtrVec.end(); itPath++)
	{
		list<Node> tmpNodeList = (**itPath).getNodeList();
		cout << tmpNodeList.size() << endl;
	}
}


double Path::gaussrand()
{
	static double V1, V2, S;
	static int phase = 0;
	double X;

	if (phase == 0)
	{
		do {
			double U1 = (double)rand() / RAND_MAX;
			double U2 = (double)rand() / RAND_MAX;

			V1 = 2 * U1 - 1;
			V2 = 2 * U2 - 1;
			S = V1 * V1 + V2 * V2;
		} while (S >= 1 || S == 0);

		X = V1 * sqrt(-2 * log(S) / S);
	}
	else
		X = V2 * sqrt(-2 * log(S) / S);

	phase = 1 - phase;

	return X;
}


void Path::displaySigma()
{
	ofstream sigmaOut;
	sigmaOut.open("sigmaOut.csv", ios::app);

	if (sigmaOut.fail())
	{
		cout << "Cannot open sigma file!" << endl;
		return;
	}

	vector<Para>::iterator itSigma;
	for (itSigma = paraVec.begin(); itSigma != paraVec.end(); itSigma++)
	{
		sigmaOut << (*itSigma).Sigma << ',';
	}
	sigmaOut << endl;

	sigmaOut.close();
}
