// Except for any way in which it interferes with Cedrick Collomb's 2009
// copyright assertion in the article "Burg’s Method, Algorithm and Recursion":
//
// Copyright (C) 2012, 2013 Rhys Ulerich
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//

/** @file
 * Example program using \ref ar::burg_method and \ref ar::evaluate_models.
 */

 //#include <cmath>
 //#include <cstdio>
 //#include <iostream>
 //#include <iterator>
 //#include <vector>
 //#include <random>
 //#include <chrono>
#include <valarray>
#include <array>
#include <gtest/gtest.h>

#include "Basic/ARModel/ARModel.h"



using namespace std;

namespace WBSF
{
	TEST(ARModel, fit_predict)
	{
		using namespace std;

		vector<double> data = { 988,985.9,978.8,1000.3,995.2,977.1,976.2,988.9,983.8,996.9,996,991.8,999.4,998.9,1018.6,1009.8,990.3,987,990.2,996.7,990,984.9,996,1009.9,1004.9,1006.2,1002.3,991.6,993.1,995.8,986,1008.2,1010.3,1008.4,1009.4,1006.4,999.3,999.6,1009.2,1012.2,1005.9,1002,1007.2,994,991.8,999.6,995.3,987.8,988.6,986,978.2,979.9,983.9,972.9,971.5,977.7,993.8,997.7,1007.7,1003.6,998.7,998.9,993.8,997.6,992.5,994.4,987.4,983.9,978,991.1,997.4,997,1001.8,1013.1,1006.7,998.8,998,997.7,979.9,989.2,996.8,999.1,994,988.8,994.2,993.6,994.5,994.4,987.9,985.9,988.7,998.3,995 };


		CARModel AR;
		AR.fit(2, data, true);

		//printf(" %8s %8s %8s %8s %8s %8s %8s\n", "p0", "p1", "MCC", "BIC", "GIC", "AIC", "AICC");
		//for (size_t i = 0; i < AR.coeffs().size(); ++i)
		//	printf(" %8.4lg", AR.coeffs()[i]);
		//
		//
		//printf(" %8.4lf", AR.get_metrics<MCC>());
		//printf(" %8.4lf", AR.get_metrics<BIC>());
		//printf(" %8.4lf", AR.get_metrics<GIC<>>());
		//printf(" %8.4lf", AR.get_metrics<AIC>());
		//printf(" %8.4lf", AR.get_metrics<AICC>());
		//printf("\n");

		ASSERT_NEAR(AR.coeffs()[0], 0.7460, 0.0001);
		ASSERT_NEAR(AR.coeffs()[1], -0.1328, 0.0001);
		ASSERT_NEAR(AR.get_metrics<MCC>(), 4.0173, 0.0001);
		ASSERT_NEAR(AR.get_metrics<BIC>(), 4.0498, 0.0001);
		ASSERT_NEAR(AR.get_metrics<GIC<>>(), 4.0169, 0.0001);
		ASSERT_NEAR(AR.get_metrics<AIC>(), 3.9954, 0.0001);
		ASSERT_NEAR(AR.get_metrics<AICC>(), 3.9968, 0.0001);


		//const size_t NB_REPS = 1000;
		//std::array< std::valarray<double>, 2 > coeffs;
		//for (size_t i = 0; i < coeffs.size(); ++i)
		//	coeffs[i].resize(NB_REPS);
		//
		//
		//for (int ii = 0; ii < NB_REPS; ii++)
		//{
		//	size_t N2 = 10000;
		//	vector<double> data2 = AR.predict(N2, data);
		//
		//
		//	AR.fit(2, data2, true);
		//
		//	for (size_t i = 0; i < AR.coeffs().size(); ++i)
		//		coeffs[i][ii] = AR.coeffs()[i];
		//
		//}
		//
		//
		//
		//for (size_t i = 0; i < AR.coeffs().size(); ++i)
		//	printf(" %8.4lg  %8.4lg", coeffs[i].sum() / coeffs[i].size(), 1.1*(coeffs[i].max() - coeffs[i].min()) / 2);

		size_t N2 = 10000;
		vector<double> data2 = AR.predict(N2, data);
		AR.fit(2, data2, true);

		EXPECT_NEAR(AR.coeffs()[0], 0.8371, 0.2456);//Generate with 1000 reps
		EXPECT_NEAR(AR.coeffs()[1], -0.15, 0.2612);//Generate with 1000 reps




	}



}

