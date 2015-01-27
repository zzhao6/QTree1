#ifndef STDAFX_H_
#define STDAFX_H_

#include <iostream>
#include <cstdio>
#include <list>
#include <vector>
#define M_PI       3.14159265358979323846
#include <cmath>
#include <fstream>
#include <cassert>
#include <ctime>
#include <cstdlib>
#include <complex>

#include "para.h"
#include <stdio.h>
using namespace std;

#define NCHILDREN 4

// ========= input parameters ====================
#define PROB 1.0/6.0f
#define INTEREST 0.1f
#define NCHILDREN 4	// every node has 4 successors
#define MATURITY 1
#define STEPS MATURITY*252
#define PATH 1
#define POOLSIZE PATH*STEPS

#define	STARTPRICE 100
#define STRIKE 90

// tmp param
#define SIGMA 0.4f
#define SIGMA1 0.099
#define SIGMA2 0.101

// parameters of Uniform

#define	MAX 0.5
#define MIN 0.3

// parameters of Heston
#define KAPPA 11.35
#define THETA 0.022
#define VOLOFVOL 0.618
#define VOL0 0.04

#define LAMBDA 0
#define RHO 0

//quadrature parameters
#define EPS 3.0e-11
#define QUASTEPS 1000
#define LOWERBOUND 0
#define UPPERBOUND 100

//MonteCarlo parameters
#define NUMPATHS 100


// ================================================

#define max(a, b) (a>b) ? a : b
#define min(a, b) (a>b) ? b : a

#endif
