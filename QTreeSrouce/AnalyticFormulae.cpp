/*
 * AnalyticFormulae.cpp
 *
 *  Created on: Jun 24, 2014
 *      Author: luis
 */
#include "StdAfx.h"
#include "AnalyticFormulae.h"

using namespace std;

//BlackScholes
double normalDistribution(double x)
{
  if(x<-10.)return 0.;
  if(x>10.)return 1.;
  // number of steps
  int N=1000;
  // range of integration
  double a=0,b=x;
  // local variables
  double s,h,sum=0.;
  // initialize the variables
  h=(b-a)/N;
  // add in the first few terms
  sum = sum + exp(-a*a/2.) + 4.*exp(-(a+h)*(a+h)/2.);
  // and the last one
  sum = sum + exp(-b*b/2.);
  // loop over terms 2 up to N-1
  for(int i=1;i<N/2;i++)
  {
    s = a + 2*i*h;
    sum = sum + 2.*exp(-s*s/2.);
    s = s + h;
    sum = sum + 4.*exp(-s*s/2.);
  }
  // complete the integral
  sum = 0.5 + h*sum/3./sqrt(8.*atan(1.));
  // return result
  return sum;
}

double eropeanCallPrice_BS(double S, double tau, double K, double sigma, double r)
{
	double d1,d2;
	d1=(log(S/K)+(r+sigma*sigma/2.0)*tau)/(sigma*sqrt(tau));
	d2=d1-sigma*sqrt(tau);

	return S*normalDistribution(d1)-K*exp(-r*tau)*normalDistribution(d2);
}

double eropeanPutPrice_BS(double S, double tau, double K, double sigma, double r)
{
	double d1,d2;
	d1=(log(S/K)+(r+sigma*sigma/2.0)*tau)/(sigma*sqrt(tau));
	d2=d1-sigma*sqrt(tau);

	return K*exp(-r*tau)*normalDistribution(-d2)
			-S*normalDistribution(-d1);
}

double gaussrand()
{
	static double V1, V2, S;
	static int phase = 0;
	double X;

	if ( phase == 0 )
	{
		do {
			double U1 = (double)rand() / RAND_MAX;
			double U2 = (double)rand() / RAND_MAX;

			V1 = 2 * U1 - 1;
			V2 = 2 * U2 - 1;
			S = V1 * V1 + V2 * V2;
		} while(S >= 1 || S == 0);

		X = V1 * sqrt(-2 * log(S) / S);
	} else
		X = V2 * sqrt(-2 * log(S) / S);

	phase = 1 - phase;
	return X;
}

void Heston_MC(double S0,
								double tau,
								double K,
								double r,
								double Y0,
								double kappa,
								double theta,
								double sigma,
								double rho){

	//total number of paths
	int M=NUMPATHS;

	//total number of loops needed
	int n=M/4;

	//total steps with daily data
	int N=STEPS;

	//daily-data step length
	double Delta_t=tau/N;
	double sqrt_Delta_t=sqrt(Delta_t);

	//option price
	vector <double> result;
	result.clear();
//	double ave_C=0.0;
//	double ave_CC=0.0;


	for (int i=1;i<=n;++i){

		//starting points:
		double S_temp1=S0,S_temp2=S0,S_temp3=S0,S_temp4=S0;
		double Y_temp1=Y0,Y_temp2=Y0,Y_temp3=Y0,Y_temp4=Y0;

		for(int l=1;l<=N;++l){

			double e1=gaussrand();
			double e2=gaussrand();

		//stochastic increment:
			S_temp1=S_temp1+r*S_temp1*Delta_t+sqrt(Y_temp1)*S_temp1*sqrt_Delta_t*e1;
			Y_temp1=Y_temp1+kappa*(theta-Y_temp1)*Delta_t+sigma*sqrt(Y_temp1)
				*(rho*sqrt_Delta_t*e1+sqrt(1-rho*rho)*sqrt_Delta_t*e2);

			S_temp2=S_temp2+r*S_temp2*Delta_t+sqrt(Y_temp2)*S_temp2*sqrt_Delta_t*(-e1);
			Y_temp2=Y_temp2+kappa*(theta-Y_temp2)*Delta_t+sigma*sqrt(Y_temp2)
				*(rho*sqrt_Delta_t*(-e1)+sqrt(1-rho*rho)*sqrt_Delta_t*(-e2));

			S_temp3=S_temp3+r*S_temp3*Delta_t+sqrt(Y_temp1)*S_temp3*sqrt_Delta_t*(-e1);
			Y_temp3=Y_temp3+kappa*(theta-Y_temp3)*Delta_t+sigma*sqrt(Y_temp3)
				*(rho*sqrt_Delta_t*(-e1)+sqrt(1-rho*rho)*sqrt_Delta_t*e2);

			S_temp4=S_temp4+r*S_temp4*Delta_t+sqrt(Y_temp4)*S_temp4*sqrt_Delta_t*e1;
			Y_temp4=Y_temp4+kappa*(theta-Y_temp4)*Delta_t+sigma*sqrt(Y_temp4)
				*(rho*sqrt_Delta_t*e1+sqrt(1-rho*rho)*sqrt_Delta_t*(-e2));

		}

		double Call_temp1=exp(-r*tau)*max(S_temp1-K,0.0); result.push_back(Call_temp1);
		double Call_temp2=exp(-r*tau)*max(S_temp2-K,0.0); result.push_back(Call_temp2);
		double Call_temp3=exp(-r*tau)*max(S_temp3-K,0.0); result.push_back(Call_temp3);
		double Call_temp4=exp(-r*tau)*max(S_temp4-K,0.0); result.push_back(Call_temp4);


//		ave_C 	=	(ave_C*(i-1)*4+Call_temp1+Call_temp2+Call_temp3+Call_temp4)/(i*4);
//		ave_CC 	= 	(ave_CC*(i-1)*4 +Call_temp1*Call_temp1
//									+Call_temp2*Call_temp2
//									+Call_temp3*Call_temp3
//									+Call_temp4*Call_temp1)/(i*4);
	}

	double mean = 0.0, stddev;
	for (int i = 0; i < M; i++)
	{
		mean += result[i];
	}
	mean /= M;

	double variance = 0.0;
	for (int i = 0; i < M; i++)
	{
		variance += (result[i] - mean) * (result[i] - mean);
	}
	variance /= (M - 1);
	stddev = sqrt(variance);
	double stderrr = stddev/sqrt(double(M));

	cout << "Number of Paths: " << M << endl;
	cout << "Mean: " << mean << endl;
	cout << "Std err: " << stderrr << endl;


//	double error=1.96*sqrt((ave_CC-callPrice*callPrice)/(M-1));

//	cout<<"call price: "<<ave_C<<endl
//		<<"Std: "<< sqrt((M*ave_CC-M*ave_C*ave_C)/(M-1)) <<endl;
//		<<"ave_C="<<callPrice<<endl
//		<<"ave_CC="<<ave_CC<<endl;
}


complex<double> f_j(int j, double phi, Para para){
	double S0 = para.StartPrice;
//	double K=STRIKE;
	double r=para.Interest;
	double tau = para.Maturity;

	double lambda=para.Lambda;
	double kappa=para.Kappa;
	double theta=para.Theta;
	double sigma=para.VolOfVol;
	double rho=para.Rho;
	double V0=para.Vol0;

	complex<double> i(0,1);

	double a=kappa*theta;
	double b;
	double u;
	switch (j){
	case 1:
		u=.5;
		b=kappa + lambda - rho*sigma;
		break;
	case 2:
		u=-0.5;
		b=kappa+lambda;
	}

	complex<double> d=sqrt((rho*sigma*phi*i-b)*(rho*sigma*phi*i-b)
						-sigma*sigma*(2.0*u*phi*i-phi*phi));
	complex<double> g=(b-rho*sigma*phi*i+d)/(b-rho*sigma*phi*i-d);

	complex<double> C= r*phi*i*tau + (a)/(sigma*sigma)
					*( (b-rho*sigma*phi*i+d)*tau -2.0*log((1.0-g*exp(d*tau))/(1.0-g)) );

	complex<double> D=((b-rho*sigma*phi*i+d)/(sigma*sigma))*((1.0-exp(d*tau))/(1.0-g*exp(d*tau)));

	return exp(C+D*V0+i*phi*log(S0));
}




double integrand_phi(int j,double phi, Para para){
	complex<double> result(0,0);
	complex<double> i(0,1);
	result = exp(-i*phi*log(para.Strike))*f_j(j, phi, para)/(i*phi);


	return result.real();
}

double integration_gauleg(int j, double(*integrand)(int, double, Para), Para para)
{

	double lowerbound = LOWERBOUND;
	double upperbound = UPPERBOUND;

	double result = 0.0;

	int n = QUASTEPS;
	float x[QUASTEPS + 1];
	float w[QUASTEPS + 1];

	gauleg(lowerbound, upperbound, x, w, n);

	for (int i = 1; i <= n; i++){
		result += w[i] * integrand_phi(j, x[i], para);
	}

	return result;
}


double P_j(int j, Para para){



	double result=0.0;
//M_PI
	result += 0.5;

	result += 1.0/ M_PI *integration_gauleg(j,integrand_phi, para);


	return result;
}



void gauleg(float x1, float x2, float x[], float w[], int n){
	int m,j,i;
	double z1,z,xm,xl,pp,p3,p2,p1;

	m=(n+1)/2;
	xm=0.5*(x2+x1);
	xl=0.5*(x2-x1);
	for (i=1;i<=m;i++){
		z=cos(3.141592654*(i-0.25)/(n+0.5));

		do {
			p1=1.0;
			p2=0.0;
			for (j=1;j<=n;j++){
				p3=p2;
				p2=p1;
				p1=((2.0*j-1.0)*z*p2-(j-1.0)*p3)/j;
			}
			pp=n*(z*p1-p2)/(z*z-1.0);
			z1=z;
			z=z1-p1/pp;
		} while (fabs(z-z1)>EPS);
		x[i]=xm-xl*z;
		x[n+1-i]=xm+xl*z;
		w[i]=2.0*xl/((1.0-z*z)*pp*pp);
		w[n+1-i] =w[i] ;
	}
}


double EuroCall_Heston(Para para){

	return para.StartPrice*P_j(1, para)-para.Strike*exp(-para.Interest*para.Maturity)*P_j(2, para);
}

double EuroPut_Heston(Para para){
	double call = EuroCall_Heston(para);
	return call - para.StartPrice + para.Strike*exp(-para.Interest*para.Maturity);
}