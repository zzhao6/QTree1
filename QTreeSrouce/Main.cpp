#include "StdAfx.h"
#include "Path.h"
#include "AnalyticFormulae.h"



#include <time.h>
void sleep(unsigned int mseconds)
{
	clock_t goal = mseconds + clock();
	while (goal > clock());
}

void BSTests(Para& para1, int sigmaType = 0);			// test 1
void BSTestsInterest(Para& para1, int sigmaType = 0);	// test 2
void BSTestsProb(Para& para1, int sigmaType = 0);		// test 3
void BSTestsFull(Para& para1, int sigmaType = 0);		// test 4

double MCHeston(Para& para, int numPath, int sigmaType = 1);
void HestonTestsOne(Para& para1, int sigmaType = 1);					// test 1
void HestonTestMC(Para& para1, int numPath, int sigmaType = 1);			// test 2
void HestonTestProb(Para& para1, int numPath, int sigmaType = 1);		// test 3
void HestonTestFull(Para& para1, int numPath, int sigmaType = 1);		// test 5
void HestonRhoAnalytical(Para& para1, int sigmaType = 1);
//void HestonTestRho(Para& para1, int numPath, int sigmaType = 1);		// test 4

int main()
{
	cout << "Calculating..." << endl;

	Para para1;
	para1.Interest = INTEREST;
	para1.Maturity = MATURITY;
	para1.StartPrice = STARTPRICE;
	para1.Prob = PROB;
	para1.Strike = STRIKE;
	para1.Steps = STEPS;
	para1.Sigma = SIGMA;

	para1.Theta = THETA;
	para1.Kappa = KAPPA;
	para1.Rho = RHO;
	para1.Vol0 = VOL0;
	para1.VolOfVol = VOLOFVOL;
	para1.Lambda = LAMBDA;
	para1.Rho = RHO;

	time_t timer;
	time(&timer);
	long timerSeed = timer;

	long glbSeed = GLOBALSEED;
	//srand(glbSeed);
	srand(timerSeed);

	//BSTests(para1);			// BS Test 1
	//BSTestsInterest(para1);	// BS Test 2
	//BSTestsProb(para1);			// BS Test 3
	//BSTestsFull(para1);

	HestonTestsOne(para1);
	//HestonTestMC(para1, 10);
	//HestonTestProb(para1, 100);
	//HestonTestFull(para1, 100);
	//HestonRhoAnalytical(para1);

	return 0;
}


double MCHeston(Para& para, int numPath, int sigmaType)
{
	double resultSum = 0;
	for (int i = 0; i < numPath; i++)
	{
		Path onePath(para, sigmaType);
		onePath.buildAndCalc();
		double tmpPrice = onePath.getOptionValue();
		resultSum += tmpPrice;

		printf("MC, %d, %f\n", i, tmpPrice);
	}

	return resultSum / numPath;
}

// Tests for Black Scholes
void BSTests(Para& para1, int sigmaType /* = 0 */)
{
	assert(para1.Rho == 0);
	Path onePath(para1, sigmaType);
	onePath.buildAndCalc();
	double priceQTree = onePath.getOptionValue();
	double priceBSCall = eropeanCallPrice_BS(para1.StartPrice, para1.Maturity, para1.Strike, para1.Sigma, para1.Interest);
	double priceBSPut = eropeanPutPrice_BS(para1.StartPrice, para1.Maturity, para1.Strike, para1.Sigma, para1.Interest);
	cout << "QTree = " << priceQTree << endl;
	cout << "BS Call = " << priceBSCall << endl;
	cout << "BS Put = " << priceBSPut << endl;
}
void BSTestsInterest(Para& para1, int sigmaType /* = 0 */)
{
	assert(para1.Rho == 0);
	LOGGER->Log("Interest Rate, Tree Call, BS Call, Tree Put, BS Put\n");
	for (int i = 1; i <= 15; i++)
	{
		double tmpInterest = i / 100.0;
		para1.Interest = tmpInterest;
		Path onePath(para1, sigmaType);
		onePath.buildAndCalc();
		double priceQTree = onePath.getOptionValue();
		double priceBSCall = eropeanCallPrice_BS(para1.StartPrice, para1.Maturity, para1.Strike, para1.Sigma, para1.Interest);
		double priceBSPut = eropeanPutPrice_BS(para1.StartPrice, para1.Maturity, para1.Strike, para1.Sigma, para1.Interest);
		cout << i << ", " << priceQTree << ", " << priceBSCall << ", " << priceBSPut << endl;
		LOGGER->Log("%f,%f,%f,%f\n", tmpInterest, priceQTree, priceBSCall, priceBSPut);
	}
}
void BSTestsProb(Para& para1, int sigmaType /* = 0 */)
{
	// 1/12 < p < 1/6
	// 1/12 = 0.083, 1/6 = 0.1667
	// p = 0.085~0.165
	assert(para1.Rho == 0);
	const int IDXBOUND = 9;
	double Probs[9] = { 0.085, 0.095, 0.105, 0.115, 0.125, 0.135, 0.145, 0.155, 0.165 };

	LOGGER->Log("Step = %d,Interest Rate = %f\n", para1.Steps, para1.Interest);
	LOGGER->Log("Prob,QTree,BSCall,BSPut\n");

	for (int i = 0; i < IDXBOUND; i++)
	{
		cout << "i = " << i << ", Prob = " << Probs[i] << endl;
		
		para1.Prob = Probs[i];
		Path onePath(para1, sigmaType);
		onePath.buildAndCalc();
		double priceQTree = onePath.getOptionValue();

		double priceBSCall = eropeanCallPrice_BS(para1.StartPrice, para1.Maturity, para1.Strike, para1.Sigma, para1.Interest);
		double priceBSPut = eropeanPutPrice_BS(para1.StartPrice, para1.Maturity, para1.Strike, para1.Sigma, para1.Interest);
		
		LOGGER->Log("%f, %f, %f, %f\n", para1.Prob, priceQTree, priceBSCall, priceBSPut);
	}
}
void BSTestsFull(Para& para1, int sigmaType /* = 0 */)
{
	assert(para1.Rho == 0);
	int stepVec[5] = { 52, 252, 504, 1008, 2016 };
	//int stepVec[5] = { 52, 52, 52, 1008, 2016 };	// for faster test
	int strikeVec[5] = { 90, 95, 100, 105, 110 };
	double sigmaVec[5] = { 0.1, 0.2, 0.3, 0.4, 0.5 };
	const int IDXBOUND = 5;
	
	LOGGER->Log("Steps, %d,%d,%d,BSCall,BSPut", stepVec[0], stepVec[1], stepVec[2], stepVec[3], stepVec[4]);
	for (int iSigma = 0; iSigma < IDXBOUND; iSigma++)
	{
		para1.Sigma = sigmaVec[iSigma];
		LOGGER->Log("\nSigma = %f", para1.Sigma);
		for (int iK = 0; iK < IDXBOUND; iK++)
		{
			para1.Strike = strikeVec[iK];
			LOGGER->Log("\nK = %f", para1.Strike);
			for (int iStep = 0; iStep < 3; iStep++)
			{
				para1.Steps = stepVec[iStep];
				cout << para1.Sigma << " " << para1.Strike << " " << para1.Steps << endl;
				Path onePath(para1, sigmaType);
				onePath.buildAndCalc();
				double priceQTree = onePath.getOptionValue();
				LOGGER->Log(",%f", priceQTree);
			}
			double priceBSCall = eropeanCallPrice_BS(para1.StartPrice, para1.Maturity, para1.Strike, para1.Sigma, para1.Interest);
			LOGGER->Log(",%f", priceBSCall);

			double priceBSPut = eropeanPutPrice_BS(para1.StartPrice, para1.Maturity, para1.Strike, para1.Sigma, para1.Interest);
			LOGGER->Log(",%f", priceBSPut);
		}
	}
}

// Tests for Heston
void HestonTestsOne(Para& para1, int sigmaType /* = 1 */)
{
	cout << "Rho = " << para1.Rho << endl;
	sleep(2000);
	Path onePath(para1, sigmaType);
	onePath.buildAndCalc();
	double priceQTree = onePath.getOptionValue();
	cout << priceQTree << endl;
	cout << EuroCall_Heston(para1) << endl;
}
void HestonTestMC(Para& para1, int numPath, int sigmaType /* = 1 */)
{
	cout << "Rho = " << para1.Rho << endl;
	sleep(2000);

	double priceQTree = MCHeston(para1, numPath);
	double priceHestonCall = EuroCall_Heston(para1);
	double priceHestonPut = EuroPut_Heston(para1);

	cout << "QTree = " << priceQTree << endl;
	cout << "Heston Call = " << priceHestonCall << endl;
	cout << "Heston Put = " << priceHestonPut << endl;
}
void HestonTestProb(Para& para1, int numPath, int sigmaType /* = 1 */)
{
	cout << "Rho = " << para1.Rho << endl;
	sleep(2000);

	//1/12 < p < 1/6
	//1/12 = 0.083, 1/6 = 0.1667
	//p = 0.085~0.165
	const int IDXBOUND = 9;
	double Probs[9] = { 0.085, 0.095, 0.105, 0.115, 0.125, 0.135, 0.145, 0.155, 0.165 };
	LOGGER->Log("Prob, QTree, HestonCall, HestonPut\n");
	for (int iProb = 0; iProb < IDXBOUND; iProb++)
	{
		para1.Prob = Probs[iProb];
		
		cout << "Iteration:\t" << iProb << endl;
		cout << "Prob:\t" << para1.Prob << endl;
		
		LOGGER->Log("%f", para1.Prob);
		double priceQTree = MCHeston(para1, numPath);
		LOGGER->Log(",%f", priceQTree);
		double priceHestonCall = EuroCall_Heston(para1);
		LOGGER->Log(",%f", priceHestonCall);
		double priceHestonPut = EuroPut_Heston(para1);
		LOGGER->Log(",%f\n", priceHestonPut);

		cout << "QTree:\t" << priceQTree << endl;
		cout << "Call:\t" << EuroCall_Heston(para1) << endl;
		cout << "Put: \t" << EuroPut_Heston(para1) << endl << endl;
	}
}
void HestonTestFull(Para& para1, int numPath, int sigmaType /* = 1 */)
{
	cout << "Rho = " << para1.Rho << endl;
	sleep(2000);

	const int IDXBOUND = 5;
	int MCVec[5] = { 20, 50, 100, 200, 500 };
	double KVec[5] = { 90, 95, 100, 105, 110 };
	int stepVec[5] = { 10, 20, 52, 252, 504 };
	LOGGER->Log("Steps,%d,%d,%d,HestonCall,HestonPut", stepVec[0], stepVec[1], stepVec[2]);

	for (int iMC = 0; iMC < IDXBOUND; iMC++)
	{
		int numMC = MCVec[iMC];
		LOGGER->Log("\nMC = %d", numMC);
		for (int iK = 0; iK < IDXBOUND; iK++)
		{
			para1.Strike = KVec[iK];
			LOGGER->Log("\nK = %f", para1.Strike);
			for (int iStep = 0; iStep < 3; iStep++)
			{
				para1.Steps = stepVec[iStep];
				cout << numMC << ',' << para1.Strike << ',' << para1.Steps << endl;
				double priceQTree = MCHeston(para1, numMC);
				//double priceQTree = 100;
				LOGGER->Log(",%f", priceQTree);
			}
			double priceHestonCall = EuroCall_Heston(para1);
			LOGGER->Log(",%f", priceHestonCall);
			
			double priceHestonPut = EuroPut_Heston(para1);
			LOGGER->Log(",%f", priceHestonPut);
		}
	}
}
void HestonRhoAnalytical(Para& para1, int sigmaType /* = 1 */)
{
	double rhoVec[7] = { -0.5, -0.4, -0.3, -0.2, -0.1, -0.05, 0 };
	for (int i = 0; i < 7; i++)
	{
		para1.Rho = rhoVec[i];
		cout << EuroCall_Heston(para1) << ", " << EuroPut_Heston(para1) << endl;
	}
}