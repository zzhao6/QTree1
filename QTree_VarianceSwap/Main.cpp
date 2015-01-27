#include "StdAfx.h"
#include "Path.h"
#include "AnalyticFormulae.h"
#include "myLogger.h"


inline double random01Uniform();
vector <double> monteCarlo_Heston_VarianceSwap(Para& para, int numPath);


vector <double> MCHeston(Para& para, int numPath, long seedPath, int sigmaType = 1)
{
	vector <double> results;
	results.clear();
	results.reserve(numPath+1);

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
	double resultSum = 0.0;
	//for (int i = 0; i < numPath; i++)
	for (int i = 0; i < numPath+1; i++)
	{
		long newSeed = seedPath + i;
		Path onePath(para, newSeed, sigmaType);
		onePath.buildAndCalc_variance_sim();
		double tmpPrice = onePath.getOptionValue_variance_sim();

			resultSum += tmpPrice;
			printf("Tree, %d, %f\n", i, tmpPrice);

	}
	
	double mean = (resultSum-results[0]) / numPath;

	double variance = 0.0;
	for (int i=1; i < numPath+1; i++)
	{
		variance += (results[i]-mean)*(results[i]-mean);
	}
	variance = variance / numPath;
	double stddev = sqrt(variance);
	double stderr = stddev / sqrt(numPath);

	vector <double> statistics;
	statistics.push_back(mean);
	statistics.push_back(stderr);


	return statistics;
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

	// test 1, single tree

// 	Path onePath(para1, seedPath, sigmaType);
// 	onePath.buildAndCalc_variance_sim();
// 	double priceQTree = onePath.getOptionValue_variance_sim();
// 	cout << priceQTree << endl;
//
// 	cout << fairStrike_simVarianceSwap_Heston_ZL(para1) << endl;



	// test 2: test seed
	//double seedPriceQTree = MCHeston(para1, 100, seedPath);
	//cout << seedPriceQTree << endl;

	
	// test 3
	// Interest Rate, Single Tree
// 	LOGGER->Log("MC = 100\n");
// 	LOGGER->Log("Interest Rate, Tree Sim, Heston Sim, HestonStd\n");
// 	for (int i = 1; i <= 3; i++)
// 	{
// 		double tmpInterest = i / 100.0;
// 		para1.Interest = tmpInterest;
// 		double priceQTree = MCHeston(para1, 6, seedPath);
// 		double priceHestonSim = fairStrike_simVarianceSwap_Heston_ZL(para1);
// 		//double priceHestonPut = ...;
// 		//double priceHestonCall = 0;
// 		double priceHestonStd = 0;
// 		cout << i << ", " << priceQTree << endl;
// 		LOGGER->Log("%f,%f,%f,%f\n", tmpInterest, priceQTree, priceHestonSim, priceHestonStd);
// 	}


	// fourth test
	// MC = 50, 100, 200, 500
	// K = 90, 95, 100, 105, 110
	// Steps = 52, 252, 504

// 	int MCVec[4] = { 50, 100, 200, 500 };
// 	double KVec[5] = { 90, 95, 100, 105, 110 };
// 	int stepVec[3] = { 52, 252, 504 };
// 	LOGGER->Log("Steps, %d,%d,%d", stepVec[0], stepVec[1], stepVec[2]);
// 	for (int iMC = 0; iMC < 4; iMC++)
// 	{
// 		int numMC = MCVec[iMC];
// 		LOGGER->Log("\nMC = %d", numMC);
// 		for (int iK = 0; iK < 5; iK++)
// 		{
// 			para1.Strike = KVec[iK];
// 			LOGGER->Log("\nK = %f", para1.Strike);
// 			for (int iStep = 1; iStep < 2; iStep++)		// only one step, step = 252
// 			{
// 				para1.Steps = stepVec[iStep];
// 				cout << numMC << ',' << para1.Strike << ',' << para1.Steps << endl;
// 				double priceQTree = MCHeston(para1, numMC, seedPath);
// 				//double priceQTree = 100;
// 				LOGGER->Log(",%f", priceQTree);
// 			}
// 			double priceHestonCall = EuroCall_Heston(para1);
//			LOGGER->Log(",%f", priceHestonCall);
//
//			//double priceHestonPut = EuroPut_Heston(para1);
// 			//LOGGER->Log(",%f", priceHestonPut);
// 		}
// 	}


	// fifth test on p
	// 1/12 < p < 1/6
	// 1/12 = 0.083, 1/6 = 0.1667
	// p = 0.085~0.165

//	double Probs[9] = { 0.085, 0.095, 0.105, 0.115, 0.125, 0.135, 0.145, 0.155, 0.165 };
//	int numMC = 6;
//
//	LOGGER->Log("MC = 100\n");
//	LOGGER->Log("Imput Probability, Tree Sim, Heston Sim, HestonStd\n");
//
//	for (int iProb = 0; iProb < 9; iProb++)
//	{
//		para1.Prob = Probs[iProb];
//		double priceQTree = MCHeston(para1, numMC, seedPath);
//		double priceHestonSim = fairStrike_simVarianceSwap_Heston_ZL(para1);
//		double priceHestonStd = 0;
//
//		cout << iProb << ", " << priceQTree << endl;
//		LOGGER->Log("%f,%f,%f,%f\n", Probs[iProb], priceQTree, priceHestonSim, priceHestonStd);
//
//	}
//

	//sixth test on Monte Carlo simulation
	LOGGER->Log("Monte Carlo simulation on simple Realized Variance with Heston\n");
	LOGGER->Log("MC = 100\n");
	int numMC = 100;

	vector <double> MC_statis = monteCarlo_Heston_VarianceSwap(para1,numMC);
	vector <double> Tree_statis = MCHeston(para1, numMC, seedPath);

	LOGGER->Log("MC Sim, MC StdErr, Tree Sim, Tree StdErr\n");
	LOGGER->Log("%f,%f,%f,%f\n", MC_statis[0], MC_statis[1], Tree_statis[0], Tree_statis[1]);

	//system("pause");
	return 0;
}

inline double random01Uniform(){
	//generate a number bt 0~1
	return rand()/double(RAND_MAX);
}


vector <double> monteCarlo_Heston_VarianceSwap(Para& para, int numPath)
{
	double mean = 0.0, stderr = 0.0;
	vector <double> results;
	results.reserve(numPath);

	double X0 = log(para.StartPrice);
	double T = para.Maturity;
	int steps = para.Steps;
	double Delta_t = T/steps;
	double sqrt_Delta_t = sqrt(Delta_t);

	double Y0 = para.Vol0;
	double r = para.Interest;
	double sigma_Y = para.VolOfVol;
	double theta = para.Theta;
	double kappa = para.Kappa;
	double rho = para.Rho;

	for (int i = 1;i <= numPath; i++) //path number = numPath
	{
	//within every path:

		double X_temp = X0;
		double Y_temp = Y0;

		// simple return realized variancej
		double realizedVariance = 0.0;

		for (int j = 1; j <= steps; j++)
		{
			double S_old = exp(X_temp);

			//Box-Muller to generate two standard normal rv: e1 and e2
			double U1 	= random01Uniform();
			double U2 	= random01Uniform();
			double R	= sqrt((-2)*log(U1));
			double angle =  2* M_PI *U2;
			double e1	= R*cos(angle);
			double e2	= R*sin(angle);

			//evolution
			X_temp = X_temp + ( r - Y_temp ) * Delta_t + sqrt(Y_temp) * sqrt_Delta_t * e1;
			Y_temp = Y_temp + kappa*( theta -Y_temp) * Delta_t +
						sigma_Y * sqrt(Y_temp) * ( rho*sqrt_Delta_t*e1+sqrt(1-rho*rho)*sqrt_Delta_t*e2)
						+.25*sigma_Y*sigma_Y
							* ( (rho*sqrt_Delta_t*e1+sqrt(1-rho*rho)*sqrt_Delta_t*e2)
								*(rho*sqrt_Delta_t*e1+sqrt(1-rho*rho)*sqrt_Delta_t*e2)-Delta_t);

			if (Y_temp <= 0.0){ cout<< "the variance becomes negative!\n";}

			double S_new = exp(X_temp);

			realizedVariance += ((S_new - S_old) / S_old) * ((S_new - S_old) / S_old);
		}

		realizedVariance = ( T*252 /steps)* exp(-r * T) * realizedVariance * 10000 ;

		cout << i << ", " << realizedVariance <<endl;
		mean += realizedVariance;
		results.push_back(realizedVariance);

	}

	mean = mean / numPath;

	double variance = 0.0;
	for (int i = 0; i < numPath; i++)
	{
		variance += (results[i] - mean) * (results[i] - mean);
	}
	variance = variance / numPath;
	double stddev = sqrt(variance);
	stderr = stddev / sqrt(numPath);

	vector <double> statistics;
	statistics.clear();
	statistics.push_back(mean);
	statistics.push_back(stderr);

	return statistics;
}














