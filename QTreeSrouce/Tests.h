#ifndef TESTS_H_
#define TESTS_H_

#include "StdAfx.h"
#include "Path.h"


double MCHeston(Para& para, int numPath, int sigmaType = 1)
{
	if (sigmaType != 1)
	{
		try
		{
			throw 4;
		}
		catch (int e)
		{
			cout << "Sigma Type has to be 1 in MC Heston!" << endl;
		}
	}
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



#endif // !TESTS_H_