#ifndef UTIL_H
#define UTIL_H

#include <cstdint>

const uint8_t A_BASE = 0; //00
const uint8_t C_BASE = 1; //01
const uint8_t G_BASE = 2; //10
const uint8_t T_BASE = 3; //11
const uint32_t MASK  = 3; //0000 0011;
const uint16_t MASK_16 = 3;
const int MAX_SEQ_16 = 8;

//-----------------------------------------------------------------------------------
// Converts a char to twobit representation. Valid is set to false if a base not in 
// {A, a, T, t, C, c, G, g} is encountered. The caller is free to decide how to handle this
// situation.
inline uint8_t twoBit(char c, bool &valid)
{
   switch(c)
   {
      case 'A': 
      case 'a': return A_BASE;
      case 'T':
      case 't': return T_BASE;
      case 'C':
      case 'c': return C_BASE;
      case 'G':
      case 'g': return G_BASE;
      
      default:
         valid = false;
         return 4;
   }
}

//-----------------------------------------------------------------------------------
// Calculates the hamming distance between two, 2bit sequences.
inline int twoBitHamming(uint16_t a, uint16_t b)
{ 
   if (a == b)
      return 0;
   
   uint16_t x = a ^ b; //XOR identifies the mismatched bits.
   int distance = 0;
   //shift every two bits. If the 2bit value is > 0, the bases don't match.
   ( x        & MASK_16) > 0 ? distance++ : 0;
   ((x >> 2)  & MASK_16) > 0 ? distance++ : 0;
   ((x >> 4)  & MASK_16) > 0 ? distance++ : 0;
   ((x >> 6)  & MASK_16) > 0 ? distance++ : 0;
   ((x >> 8)  & MASK_16) > 0 ? distance++ : 0;
   ((x >> 10) & MASK_16) > 0 ? distance++ : 0;
   ((x >> 12) & MASK_16) > 0 ? distance++ : 0;
   ((x >> 14) & MASK_16) > 0 ? distance++ : 0;
   return distance;
}

bool squeeze(const char *seq, int nseq, uint16_t &squeezed);

#endif
