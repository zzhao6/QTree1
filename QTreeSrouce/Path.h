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
	double gridCorr(double sigma, double deltaT);
	double driftCorr(double sigma, double deltaT);

	double gridUnCorr(double sigma, double deltaT);
	double driftUnCorr(double sigma, double deltaT);

	double genDriftsAndGrids(double sigma, double deltaT);
};

#endif
