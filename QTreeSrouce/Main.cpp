#include "StdAfx.h"
#include "Path.h"
#include "AnalyticFormulae.h"
#include "myLogger.h"

double MCHeston(Para& para, int numPath, long seedPath, int sigmaType = 1)
{
	if (sigmaType != 1)
	{
		try
		{
			throw 4;
		}
		catch (int e)
		{
			cout << "Sigma Type has to be 2 in MC Heston!" << endl;
		}
	}
	double resultSum = 0;
	for (int i = 0; i < numPath; i++)
	{
		long newSeed = seedPath + i;
		Path onePath(para, newSeed, sigmaType);
		onePath.buildAndCalc();
		double tmpPrice = onePath.getOptionValue();
		resultSum += tmpPrice;

		printf("MC, %d, %f\n", i, tmpPrice);
	}
	
	return resultSum / numPath;
}


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
	long seedPath = timer;

	srand(seedPath);
	//srand(1000);

	int sigmaType = 1;

	// Black Shcoles Tests

	// first test, one path
	 //Path onePath(para1, seedPath, sigmaType);
	 //onePath.buildAndCalc();
	 //double priceQTree = onePath.getOptionValue();
	 //double priceBS = eropeanCallPrice_BS(para1.StartPrice, para1.Maturity, para1.Strike, para1.Sigma, para1.Interest);

	 //cout << priceBS << endl;
	 //cout << priceQTree << endl;

	// second test, different interest rate
	//double priceQTreeVec[15];
	//double priceBSVec[15];
	//LOGGER->Log("Interest Rate, Tree Call, BS Call, Tree Put, BS Put\n");
	//for (int i = 1; i <= 15; i++)
	//{
	//	double tmpInterest = i / 100.0;
	//	para1.Interest = tmpInterest;
	//	Path onePath(para1, seedPath, sigmaType);
	//	onePath.buildAndCalc();
	//	double priceQTree = onePath.getOptionValue();
	//	double priceBSCall = eropeanCallPrice_BS(para1.StartPrice, para1.Maturity, para1.Strike, para1.Sigma, para1.Interest);
	//	double priceBSPut = eropeanPutPrice_BS(para1.StartPrice, para1.Maturity, para1.Strike, para1.Sigma, para1.Interest);
	//	cout << i << ", " << priceQTree << endl;
	//	LOGGER->Log("%f,%f,%f,%f\n", tmpInterest, priceQTree, priceBSCall, priceBSPut);
	//}


	// third test
// 	int stepVec[5] = { 52, 252, 504, 1008, 2016 };
// 	LOGGER->Log("Steps, %d,%d,%d,%d,%d", stepVec[0], stepVec[1], stepVec[2], stepVec[3], stepVec[4]);
// 	for (int iSigma = 1; iSigma <= 5; iSigma++)
// 	{
// 		// sigmas = 0.1, 0.2, 0.3, 0.4, 0.5
// 		para1.Sigma = double(iSigma) / 10;
// 		LOGGER->Log("\nSigma = %f", para1.Sigma);
// 		for (int iK = 0; iK < 5; iK++)
// 		{
// 			// strike = 90, 95, 100, 105, 110
// 			para1.Strike = 90 + iK * 5;
// 			LOGGER->Log("\nK = %f", para1.Strike);
// 			for (int iStep = 0 ; iStep < 5; iStep++)
// 			{
// 				para1.Steps = stepVec[iStep];
// 				cout << para1.Sigma << " " << para1.Strike << " " << para1.Steps << endl;
// 				Path onePath(para1, seedPath, sigmaType);
// 				onePath.buildAndCalc();
// 				double priceQTree = onePath.getOptionValue();
// 				LOGGER->Log(",%f", priceQTree);
// 			}
// 			double priceBSCall = eropeanPutPrice_BS(para1.StartPrice, para1.Maturity, para1.Strike, para1.Sigma, para1.Interest);
// 			LOGGER->Log(",%f", priceBSCall);
// 		}
// 	}



	// Test Heston
	// sigmaType = 1;
	// test 1, single tree

 	//Path onePath(para1, seedPath, sigmaType);
 	//onePath.buildAndCalc();
 	//double priceQTree = onePath.getOptionValue();
 	//cout << priceQTree << endl;
 	//
 	//cout << EuroCall_Heston(para1) << endl;



	// test 2: test seed
	//double seedPriceQTree = MCHeston(para1, 100, seedPath);
	//cout << seedPriceQTree << endl;

	
	// test 3
	// Interest Rate, Single Tree
// 	LOGGER->Log("MC = 100\n");
// 	LOGGER->Log("Interest Rate, Tree Call, Heston Call, TreePut, HestonPut\n");
// 	for (int i = 1; i <= 15; i++)
// 	{
// 		double tmpInterest = i / 100.0;
// 		para1.Interest = tmpInterest;
// 		double priceQTree = MCHeston(para1, 100, seedPath);
// 		//double priceHestonCall = EuroCall_Heston(para1);
// 		//double priceHestonPut = EuroPut_Heston(para1);
// 		double priceHestonCall = 0;
// 		double priceHestonPut = 0;
// 		cout << i << ", " << priceQTree << endl;
// 		LOGGER->Log("%f,%f,%f,%f\n", tmpInterest, priceQTree, priceHestonCall, priceHestonPut);
// 	}


	// fourth test
	// MC = 50, 100, 200, 500
	// K = 90, 95, 100, 105, 110
	// Steps = 52, 252, 504

 	//int MCVec[4] = { 50, 100, 200, 500 };
 	//double KVec[5] = { 90, 95, 100, 105, 110 };
 	//int stepVec[3] = { 52, 252, 504 };
 	//LOGGER->Log("Steps, %d,%d,%d", stepVec[0], stepVec[1], stepVec[2]);
 	//for (int iMC = 0; iMC < 4; iMC++)
 	//{
 	//	int numMC = MCVec[iMC];
 	//	LOGGER->Log("\nMC = %d", numMC);
 	//	for (int iK = 0; iK < 5; iK++)
 	//	{
 	//		para1.Strike = KVec[iK];
 	//		LOGGER->Log("\nK = %f", para1.Strike);
 	//		for (int iStep = 1; iStep < 2; iStep++)		// only one step, step = 252
 	//		{
 	//			para1.Steps = stepVec[iStep];
 	//			cout << numMC << ',' << para1.Strike << ',' << para1.Steps << endl;
 	//			double priceQTree = MCHeston(para1, numMC, seedPath);
 	//			//double priceQTree = 100;
 	//			LOGGER->Log(",%f", priceQTree);
 	//		}
 	//		double priceHestonCall = EuroCall_Heston(para1);
		//	LOGGER->Log(",%f", priceHestonCall);
		//	
		//	//double priceHestonPut = EuroPut_Heston(para1);
 	//		//LOGGER->Log(",%f", priceHestonPut);
 	//	}
 	//}


	// fifth test on p
	// 1/12 < p < 1/6
	// 1/12 = 0.083, 1/6 = 0.1667
	// p = 0.085~0.165

	double Probs[9] = { 0.085, 0.095, 0.105, 0.115, 0.125, 0.135, 0.145, 0.155, 0.165 };
	int numMC = 10;
	for (int iProb = 0; iProb < 2; iProb++)
	{
		para1.Steps = Probs[iProb];
		double priceQTree = MCHeston(para1, numMC, seedPath);
		LOGGER->Log(",%f", priceQTree);

		double priceHestonCall = EuroCall_Heston(para1);
		LOGGER->Log(",%f", priceHestonCall);
		
		//double priceHestonPut = EuroPut_Heston(para1);
		//LOGGER->Log(",%f", priceHestonPut);
	}


 	
	//system("pause");
	return 0;
}