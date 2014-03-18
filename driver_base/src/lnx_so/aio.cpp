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
// File:        Aio.cpp
// Module:      UserLib.lib
// Environment: User Mode
// Reference:   Microsoft WDK
// Description: Ai&Ao functions
// ----------------------------------------------------------------------------
#include <assert.h>
#include <biouserbase.h>


//*************************************************************************
// Name: VrgFindItemByGain
// Description:
//       Helper function to retrieve a value range information entry 
//    according with the specified gain code.
// Parameters: 
//  [in]gain   -- the gain code used to search.
//  [in]entries-- the value range info table to be searched.
//  [in]count  -- the count of the value range info table. 
//   
// return value: 
//    a const pointer to the AI value range info entry or NULL if not found.
//************************************************************************* 
PCVRG_INFO VrgFindItemByGain(
   IN uint32      gain,
   IN PCVRG_INFO entries,
   IN uint32      count)
{
   for (uint32 i = 0; i < count; ++i)
   {
      if (entries[i].Gain == gain)
      {
         return &entries[i];
      }
   }
   return NULL;
}
//*************************************************************************
// Name: VrgFindItemByVrgType
// Description:
//       Helper function to retrieve a value range information entry 
//    according with the specified value range type.
// Parameters: 
//  [in]vrgType-- the value range type used to search.
//  [in]entries-- the value range info table to be searched.
//  [in]count  -- the count of the value range info table. 
//   
// return value: 
//    a const pointer to the AI value range info entry or NULL if not found.
//************************************************************************* 
PCVRG_INFO VrgFindItemByVrgType(
   IN uint32      vrgType,
   IN PCVRG_INFO entries,
   IN uint32      count)
{
   for (uint32 i = 0; i < count; ++i)
   {
      if (entries[i].VrgType == vrgType)
      {
         return &entries[i];
      }
   }
   return NULL;
}

//*************************************************************************
// Name: ScaleListFindEntry
// Description:
//       Helper function to retrieve a value range scaling information entry 
//    according with the specified Value range Type.
// Parameters: 
//  [in]listHead  -- pointer to the list of scale info
//  [in]tag -- the Value to compare.
//  [in]byGain -- the tag type, true for using Gain code, false for using vrgType
// Return value: 
//    a pointer to the value range scaling info entry or NULL if not found.
//************************************************************************* 
PSCALING_ENTRY ScaleListFindEntry(
   IN PLIST_ENTRY listHead, 
   IN uint32       tag,
   IN bool         byGain)
{
   PLIST_ENTRY curr;
   PLIST_ENTRY next;
   FOR_EACH_LIST_ENTRY(curr, next, listHead)
   {
      SCALING_ENTRY *entry = container_of(curr, SCALING_ENTRY, ListEntry);
      if ((byGain && entry->Gain == tag) || (!byGain && entry->VrgType == tag))
      {
         return entry;
      }
   }
   return NULL;
}
//*************************************************************************
// Name: SizeOfMapFuncPieces
// Description:
//       Helper function to calculate the size of the specified polynomial pieces. 
// 
// Parameters: 
//  [in]pieces -- mapping function polynomial piece array
//  [in]count -- count of polynomial array.
// Return value: 
//    the size in bytes of the array.
//    A Zero indicates an error. Generally this due to a invalid
// count was passed to the function.
//************************************************************************* 
uint32 SizeOfMapFuncPieces(
   IN BioMapFuncPiece const * pieces, 
   IN uint32                   count)
{
   uint32 size = 0;
   while (count--)
   {
      size += pieces->size;
      pieces = (BioMapFuncPiece const *)((uint8*)pieces + pieces->size);
   }
   return size;
}

//*************************************************************************
// Name: PieceCountOfMapFuncPieces
// Description:
//       Helper function to calculate the item count of the specified polynomial pieces . 
// 
// Parameters: 
//  [in]pieces -- mapping function polynomial piece array
//  [in]tableSize -- the size of polynomial array.
// Return value: 
//    the item count of the array.
//    A Zero indicates an error. Generally this due to a invalid
// table size was passed to the function.
//************************************************************************* 
uint32 PieceCountOfMapFuncPieces(
   IN BioMapFuncPiece const * pieces, 
   IN uint32                   tableSize)
{
   uint32 count = 0;
   while(tableSize)
   {
      ++count;
      tableSize -= pieces->size;
      pieces = (BioMapFuncPiece const *)((uint8*)pieces + pieces->size);
   }
   return count;
}

//*************************************************************************
// Name: AiBuildDefaultMapFunc
// Description:
//       Helper function to build map function of specified value range.
//
//*************************************************************************
void AiBuildDefaultMapFunc(
   IN VRG_INFO const * vrg,
   IN uint32         resInBit,
   OUT SCALING_ENTRY *entry)
{
   assert(vrg && entry);
   entry->VrgType           = vrg->VrgType;
   entry->Gain              = vrg->Gain;
   entry->PieceCount        = 1;

   entry->Pieces->size      = sizeof(BioMapFuncPiece);
   entry->Pieces->degree    = 1;
   entry->Pieces->upperLimit= double(1ULL << resInBit);
   entry->Pieces->coef[0]   = vrg->RangeLow;
   entry->Pieces->coef[1]   = (vrg->RangeHigh - vrg->RangeLow) / double(1ULL << resInBit);
}

//*************************************************************************
// Name: AoBuildVoltageDefaultMapFunc
// Description:
//       Helper function to build map function of specified value range
// (built-in / external reference, voltage).
//
//*************************************************************************
void AoBuildVoltageDefaultMapFunc(
   IN VRG_INFO const *vrg,
   IN uint32         resInBit,
   IN uint32         extRefAntiPolar, /*ignored if not external reference*/
   IN double         extRefVoltage,   /*ignored if not external reference*/
   OUT SCALING_ENTRY *entry)
{
   assert(vrg && entry);

   double rangeLow;
   double rangeHigh;

   // the sign of coef[1], default 1 for internal reference voltage, for external reference sign = antiPolarSign * externalRefVoltage
   // If the AO external reference voltage is anti-polar,
   // for Unipolar output: the output range is (0, extV) @ -extV voltage, and (-extV,0) @ extV voltage
   // for Bipolar output: the output range is (-extV,extV) @ extV(binary 0 ==> extV), (-extV,extV) @ -extV(binary 0 ==> -extV)
   double sign = 1.0;
   double antiPolarSign = extRefAntiPolar ? -1.0 : 1.0;
   switch (vrg->VrgType)
   {
   case V_ExternalRefBipolar:
      rangeLow  = -abs(extRefVoltage);
      rangeHigh = abs(extRefVoltage);
      sign      = extRefVoltage * antiPolarSign > 0 ? 1 : -1;
      break;
   case V_ExternalRefUnipolar:
      if (extRefAntiPolar) {
         if (extRefVoltage < 0) {
            rangeLow = 0;
            rangeHigh= abs(extRefVoltage);
         } else {
            rangeLow = -1 * extRefVoltage;
            rangeHigh= 0;
         }
      } else {
         rangeLow = __min(0, extRefVoltage);
         rangeHigh= __max(0, extRefVoltage);
      }
      sign = (extRefVoltage * antiPolarSign > 0) ? 1 : -1;
      break;
   default:
      rangeLow = vrg->RangeLow;
      rangeHigh= vrg->RangeHigh;
      break;
   }

   entry->VrgType    = vrg->VrgType;
   entry->Gain       = vrg->Gain;
   entry->PieceCount = 2;

   /*/////////////////////////////////////////////////////////////////////////
   // Bipolar
   // Positive reference voltage
   //                 Y  (P2)   (P2')
   //                 | /----------
   //                 |/
   //                 /
   //                /|
   //               / |
   //              /  |
   // ------------/---0--------------> Voltage
   //           (P1)
   //
   // Negative reference voltage
   //         (P2) \  Y
   //               \ |
   //                \|
   //                 \
   //                 |\
   //                 | \
   //                 |  \
   // ----------------0---\-----------> Voltage
   //                    (P1)
   // map function piece 1: 0-0
   // map function piece 2: P1-P2
   //////////////////////////////////////////////////////////////////////////

   //////////////////////////////////////////////////////////////////////////
   // Unipolar
   // Positive reference voltage
   //                Binary
   //                 Y    (P2)   (P2')
   //                 |    /----------
   //                 |   /
   //                 |  /
   //                 | /
   //                 |/
   // ----------------0--------------> Voltage
   //                (P1)
   //
   // Negative reference voltage
   //                Binary
   //   (P1')   (P1)    Y   (P2)   (P2')
   //     -------\    |    /-------
   //             \   |   /
   //              \  |  /
   //               \ | /
   //                \|/
   // ----------------0--------------> Voltage
   // Case1: ExtRef is not AntiPolar
   //    RefVolt<0: 0-P1: antiPolarSign * RefVolt < 0
   //      map function piece 1: P1'-P1
   //      map function piece 2: P1-0
   //    RefVolt>0: 0-P2: antiPolarSign * RefVolt > 0
   //      map function piece 1: 0-0
   //      map function piece 2: 0-P2
   //
   // Case2: ExtRef is AntiPolar
   //    RefVolt<0: 0-P2: antiPolarSign * RefVolt > 0
   //      map function piece 1: 0-0
   //      map function piece 2: 0-P2
   //    RefVolt>0: 0-P1: antiPolarSign * RefVolt < 0
   //      map function piece 1: P1'-P1
   //      map function piece 2: P1-0
   //////////////////////////////////////////////////////////////////////////*/

   double const lsb = ANALOG_LSB_BITS(rangeLow, rangeHigh, resInBit);
   double const half_lsb = lsb / 2;

   PBioMapFuncPiece piece;

   // map function piece 1:
   piece            = &entry->Pieces[0];
   piece->size      = sizeof(BioMapFuncPiece);
   piece->degree    = 1;
   piece->upperLimit= rangeLow - sign * half_lsb;
   piece->coef[0]   = (sign < 0) ? ((1 << resInBit) - 1): 0;
   piece->coef[1]   = 0;

   // map function piece 2:
   piece            = &entry->Pieces[1];
   piece->size      = sizeof(BioMapFuncPiece);
   piece->degree    = 1;
   piece->upperLimit= rangeHigh - sign * half_lsb;
   piece->coef[0]   = (vrg->VrgType == V_ExternalRefUnipolar) ? 0 : -(rangeLow / lsb);
   piece->coef[1]   = sign * 1 / lsb;
}

//*************************************************************************
// Name: AoBuildCurrentDefaultMapFunc
// Description:
//       Helper function to build map function of specified value range
// (current type, e.g. 0~20ma or 4~20ma).
//
//*************************************************************************
void AoBuildCurrentDefaultMapFunc(
   IN VRG_INFO const *vrg,
   IN uint32         resInBit,
   IN uint32         offset,
   OUT SCALING_ENTRY *entry)
{
   assert(vrg && entry);

   double lsb = ANALOG_LSB_SPAN(vrg->RangeLow, vrg->RangeHigh, (1 << resInBit) - offset);
   double halfLsb = lsb / 2;

   entry->VrgType    = vrg->VrgType;
   entry->Gain       = vrg->Gain;
   entry->PieceCount = 2;

   PBioMapFuncPiece piece;

   // map function piece 1:
   piece            = &entry->Pieces[0];
   piece->size      = sizeof(BioMapFuncPiece);
   piece->degree    = 1;
   piece->upperLimit= vrg->RangeLow - halfLsb;
   piece->coef[0]   = offset;
   piece->coef[1]   = 0;

   // map function piece 2:
   // bin_value = (bin_max - bin_offset) * (cur_value - cur_min) / (cur_max - cur_min) + bin_offset
   //           = (cur_value - cur_min) * ((bin_max - bin_offset) / (cur_max - cur_min)) + bin_offset
   // where '(bin_max - bin_offset) / (cur_max - cur_min)' is just the derivative of lsb, so
   // bin_value = (cur_value - cur_min) * 1 / lsb + bin_offset
   //           = cur_value * (1 / lsb) - cur_min * (1 / lsb) + bin_offset
   // so: coef[0] = bin_offset - cur_min * (1 / lsb)
   //     coef[1] = (1 / lsb)
   // Good Luck!
   piece            = &entry->Pieces[1];
   piece->size      = sizeof(BioMapFuncPiece);
   piece->degree    = 1;
   piece->upperLimit= vrg->RangeHigh - halfLsb;
   piece->coef[0]   = -( vrg->RangeLow / lsb ) + offset;
   piece->coef[1]   = 1/lsb;
}

