#ifndef PATH_H_ 
#define PATH_H_


#include "StdAfx.h"
#include "TimeSlice.h"

class Path
{
private:

	Para pathPara;

	double optValuePath;
	vector<TimeSlice*> TSPtrVec;
	vector<Para> paraVec;		// parameter vector, mainly including different sigma
								// and thus different drifts and grids
	void buildTree();
	void calcOptValue();

	// two types of volatility
	void genSigmaHeston();
	void genSigmaConstant();

	// generate drifts and grids
	// including two situations: rho = 0 and rho != 0
	void genDriftsAndGrids();
public:
	Path(){};
	Path(Para& outPara, int sigmaType);
	~Path();

public:
	void buildAndCalc();

	// get functions
	vector<TimeSlice*> getTSVec();
	double getOptionValue();
	//vector<double> getSigma();

	void displayPath();
	void displayNodeCnt();
	void displaySigma();

public:
	double gaussrand();

public:
	// for correlated stock prices and volatilities
	//double gridCorr(double sigma, double deltaT);
	//double driftCorr(double sigma, double deltaT);

	//double gridUnCorr(double sigma, double deltaT);
	//double driftUnCorr(double sigma, double deltaT);




	//pathPara.grid = paraVec[0].Sigma * sqrt(dt);
	//pathPara.drift = (pathPara.Interest - paraVec[0].Sigma * paraVec[0].Sigma / 2) * dt;

	//for (int i = 1; i < pathPara.Steps + 1; i++)	// totally 
	//{
	//	paraVec[i].grid = paraVec[i].Sigma * sqrt(dt);
	//	paraVec[i].drift = (pathPara.Interest - paraVec[i].Sigma * paraVec[i].Sigma / 2) * dt;
	//}


};

#endif
