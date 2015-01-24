#ifndef PATH_H_ 
#define PATH_H_


#include "StdAfx.h"
#include "TimeSlice.h"

class Path
{
private:

	Para pathPara;
	long seedPath;

	double optValuePath;
	double optValuePath_variance_std;
	double optValuePath_variance_sim;
	vector<TimeSlice*> TSPtrVec;
	vector<Para> paraVec;		// parameter vector, mainly including different sigma
								// and thus different drifts and grids

	void buildTree();
	void calcOptValue();
	void calcOptValue_variance_std();
	void calcOptValue_variance_sim();

	// two types of volatility
	void genSigmaHeston();
	void genSigmaConstant();

public:

	double gaussrand();

public:
	Path(){};
	Path(Para& outPara, long pathSeed, int sigmaType);
	~Path();

public:
	void buildAndCalc();
	void buildAndCalc_variance_std();
	void buildAndCalc_variance_sim();

	// get functions
	vector<TimeSlice*> getTSVec();
	double getOptionValue();
	double getOptionValue_variance_std();
	double getOptionValue_variance_sim();
	//vector<double> getSigma();

	void displayPath();
	void displayNodeCnt();
	void displaySigma();
};

#endif
