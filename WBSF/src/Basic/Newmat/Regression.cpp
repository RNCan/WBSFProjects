#include "Regression.h"

namespace NEWMAT
{

double DoRegression(const Matrix& X, const ColumnVector& Y, ColumnVector& result, ColumnVector& Fitted)
{

    int nobs = X.Nrows();
    int npred = X.Ncols();

   // make vector of 1s
   NEWMAT::ColumnVector Ones(nobs); Ones = 1.0;

   // calculate means (averages) of x1 and x2 [ .t() takes transpose]
   NEWMAT::RowVector M = Ones.t() * X / nobs;

   // and subtract means from x1 and x1
   NEWMAT::Matrix XC(nobs,npred);
   XC = X - Ones * M;

   // do the same to Y [use Sum to get sum of elements]
   NEWMAT::ColumnVector YC(nobs);
   Real m = NEWMAT::Sum(Y) / nobs;  YC = Y - Ones * m;

   // form sum of squares and product matrix
   //    [use << rather than = for copying Matrix into SymmetricMatrix]
   NEWMAT::SymmetricMatrix SSQ; SSQ << XC.t() * XC;

   // calculate estimate
   //    [bracket last two terms to force this multiplication first]
   //    [ .i() means inverse, but inverse is not explicity calculated]
   NEWMAT::ColumnVector A = SSQ.i() * (XC.t() * YC);

   
   // calculate estimate of constant term
   //    [AsScalar converts 1x1 matrix to Real]
   Real a = m - (M * A).AsScalar();
   result.ReSize( A.Nrows() + 1);
   result[0] = a ;//<< A.Store();
   for(int i=0; i<A.Nrows(); i++)
    result[i+1] = A[i];

   // Get variances of estimates from diagonal elements of inverse of SSQ
   //    [ we are taking inverse of SSQ - we need it for finding D ]
   NEWMAT::Matrix ISSQ = SSQ.i(); NEWMAT::DiagonalMatrix D; D << ISSQ;
   NEWMAT::ColumnVector V = D.AsColumn();
   NEWMAT::Real v = 1.0/nobs + (M * ISSQ * M.t()).AsScalar();
					    // for calc variance of const

   // Calculate fitted values and residuals
   int npred1 = npred+1;
   //NEWMAT::ColumnVector Fitted = X * A + a;
   Fitted = X * A + a;
   NEWMAT::ColumnVector Residual = Y - Fitted;
   NEWMAT::Real ResVar = Residual.SumSquare() / (nobs-npred1);

   Real R2 = 1 - (Residual.t()*Residual).AsScalar()/(YC.t()*YC).AsScalar();
   
   return R2;
}

}