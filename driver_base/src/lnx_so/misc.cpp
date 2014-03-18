/*******************************************************************************
              Copyright (c) 1983-2009 Advantech Co., Ltd.
********************************************************************************
THIS IS AN UNPUBLISHED WORK CONTAINING CONFIDENTIAL AND PROPRIETARY INFORMATION
WHICH IS THE PROPERTY OF ADVANTECH CORP., ANY DISCLOSURE, USE, OR REPRODUCTION,
WITHOUT WRITTEN AUTHORIZATION FROM ADVANTECH CORP., IS STRICTLY PROHIBITED. 

================================================================================
REVISION HISTORY
--------------------------------------------------------------------------------
$Log: $
--------------------------------------------------------------------------------
$NoKeywords:  $
*/

// ----------------------------------------------------------------------------
// File:        BioUserLib.cpp
// Module:      UserLib.lib
// Environment: User Mode
// Reference:   Microsoft WDK
// Description: common functions which are independent with any special 'function'
// ----------------------------------------------------------------------------
#include <float.h>
#include <math.h>
#include <biouserbase.h>

//*************************************************************************
// add by jidong to fix up the code from ABJ.
// The code from ABJ has a minor bug that for the user frequency which can be
// gracefully divided into two pacer divisor, the codes may create two somehow
// strange value which has a minor different with the actual value.
//*************************************************************************
bool CalcDivisorUseFactorization(
   uint32    divisor,
   uint16 & div1,
   uint16 & div2 )
{
   uint32 sqrtVal = (uint32)sqrt( (double)divisor );
   uint32 factor1;
   uint32 factor2;
   for( factor1 = sqrtVal; factor1 > 1; --factor1 )
   {
      if ( divisor % factor1 == 0 )
      {
         factor2 = divisor / factor1;
         if ( factor1 < 0x10000 && factor2 < 0x10000 )
         {
            div1 = (uint16)factor1;
            div2 = (uint16)factor2;
            return true;
         }
      }
   }
   return false;
}
//************************************************************************* 
// copied from ABJ PCI1747/MIC3747 DLL Driver: GetPacerRate
//************************************************************************* 
static
void CalcDivisorUseMinimalError(
   IN  double   divisor, 
   OUT uint16 & div1,
   OUT uint16 & div2 )
{
   uint32  divisor1, divisor2;
   divisor1 = (uint32)(divisor/65535.0 + 0.5);
   if (divisor1 > 1L)
   {
      divisor1 = __min(65535, (divisor1 + 1));
      divisor2 = __min(65535, (uint32)(divisor/ divisor1));
   } else {
      divisor1 = 1; // set to 1 to avoid "divided by zero" when calculate divisor2

      double  derror = 1.0 ;
      double  dtemp;
      for (int i = 2; i <= (int)(sqrt(divisor)+0.5); ++i )
      {
         divisor2 = (uint32)(divisor/i +0.5);
         if (divisor2*i == (uint32)(divisor+0.5))
         {
            divisor1 = i;
            break;
         }
         dtemp = fabs((double)divisor2*i-divisor)/divisor;
         if ( derror > dtemp )
         {
            divisor1 = i;
            derror = dtemp;
         }
      }
      divisor2 = (uint32)(divisor/divisor1 +0.5);
   }

   div1 = (uint16)divisor1;
   div2 = (uint16)divisor2;
}
//*************************************************************************
// Name: UserFreqToCounterDivider
// Description:
//       Calculate the two 16-bit counter divider value for the user 
// specified frequency and the base clock frequency.
// Parameters: 
//  [in]baseFreq -- base clock frequency of the counter.
//  [in]userFreq -- the frequency the user wanted.
//  [out]div1, div2 -- the calculation result. 
//   Approximately, UserFreq = BaseClkFreq / ( div1 * div2 )
// return value: 
//    <None>
//************************************************************************* 
void UserFreqToCounterDivider(
   IN  uint32  baseFreq,
   IN  double  userFreq,
   OUT uint16  *div1,
   OUT uint16  *div2 )
{
   double divisor = ((double)baseFreq) / userFreq;
   if ( divisor < 2 ) // if baseFreq < userFreq, divisor < 1, the calculated div1 or div2 maybe 0 
   {
      *div1 = 1;
      *div2 = 2;
      return;
   }

   //
   // The best way is using factorization to calculate the divisor because
   // it has no error.
   uint32  divInt  = (uint32)divisor;
   if ( divisor - divInt < FLT_EPSILON && CalcDivisorUseFactorization( divInt, *div1, *div2 ) )
   {
      return;
   }

   //
   // If we can't calculate the divisor using factorization, we had to use
   // the 'minimal error' method
   CalcDivisorUseMinimalError( divisor, *div1, *div2 );
}
