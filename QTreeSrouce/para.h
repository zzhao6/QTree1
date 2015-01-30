#ifndef PARA_H_
#define PARA_H_

struct Para
{
	// basic (BS) parameters
	double Maturity;
	int Steps;
	double StartPrice;
	double Strike;
	double Interest;
	double Sigma;
	double PrevSigma;	// for calculation in the terminal nodes

	// Heston parameters
	double Kappa;
	double Theta;
	double VolOfVol;
	double Vol0;
	double Lambda;
	double Rho;

	// other parameters
	double Prob;	// input probability in the tree
	double grid;	// grid for the TS containing this Node
	double drift;	// drift for the TS containing this Node
};

#endif