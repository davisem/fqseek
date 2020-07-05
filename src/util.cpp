#include <iostream>
#include "util.h"

//-----------------------------------------------------------------------------------
// Compresses a sequence into a uint16_t. Returns false if an invalid base is
// encountered. 

bool squeeze(const char *seq, int nseq, uint16_t &squeezed)
{
   if(nseq > MAX_SEQ_16)
      throw std::runtime_error("Sequence must be smaller than " + 
                               std::to_string(MAX_SEQ_16) + " bps");
   
   bool valid = true;
   int shift = 14;
   int i = 0;
   
   do
   {
      squeezed |= (twoBit(seq[i++], valid) << shift);
      shift-=2;
   }
   while(i < nseq);
   
   return valid;
}