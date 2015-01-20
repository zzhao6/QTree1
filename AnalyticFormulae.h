/*
 * AnalyticFormulae.h
 *
 *  Created on: Jun 24, 2014
 *      Author: luis
 */

#ifndef AF_H_
#define AF_H_

//Black-Scholes:
double normalDistribution(double x);
double eropeanCallPrice_BS(double S, double tau, double K, double sigma, double r);
double eropeanPutPrice_BS(double S, double tau, double K, double sigma, double r);


//Monte Carlo:
double gaussrand();
void Heston_MC(					double S0,
								double tau,
								double K,
								double r,
								double Y0,
								double kappa,
								double theta,
								double sigma,
								double rho);


//Heston's close-form formula:
complex<double> f_j(int j, double phi, Para para);
double integrand_phi(int j, double phi, Para para);
void gauleg(float x1, float x2, float x[], float w[], int n);
double integration_gauleg(int j, double(*integrand)(int, double), Para para);
double P_j(int j, Para para);
double EuroCall_Heston(Para para);
double EuroPut_Heston(Para para);


#endif