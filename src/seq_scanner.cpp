#include <climits>
#include <stdexcept>
#include "seq_scanner.h"

const int A_SEQ_SHIFT = 18;
const int VAR_SHIFT   = 16;
const uint64_t B_SEQ_MASK  = USHRT_MAX;

//       1N    4N     3N  + V   4N       4N
// 00000000 00000000 00000000 00000000 00000000
// 00000000 00000000 00000000 11111111 11111111  B_SEQ_MASK(mask for extracting the lowest 8 bases)
// 00000000 00000000 00000011 00000000 00000000  VAR_SHIFT (Shift the variant to the least significant 2 bits)

//-----------------------------------------------------------------------------------
// Ctor
SeqScaner::SeqScaner(const std::string &query_seq)
{
   uint16_t a_seq = 0;
   uint16_t b_seq = 0;

   bool valid = true;
   var = twoBit(query_seq[8], valid);
   
   std::string a = query_seq.substr(0, 8);
   std::string b = query_seq.substr(9);

   if (!squeeze(a.c_str(), 8, a_seq)||
       !squeeze(b.c_str(), 8, b_seq)||
       !valid)
      throw std::runtime_error("Invalid bases detected in " + query_seq);
   
   buildDistances(a_seq, b_seq);
}

//-----------------------------------------------------------------------------------
// Scans for the query sequence in the reference by left shifting the reference 
// window and adding the next 2 bits. If the query is found, the position
// of the last added base is returned. If no query is found, -1 is returned.
int SeqScaner::scan(const char *ref_seq, int seq_size, int max_distance)
{
   int idx = 0;
   uint64_t window = 0;
   int n_valid  = 0;
   int distance = 0;
   
   do
   {  
      shiftWindow(window, n_valid, ref_seq[idx++]);

      if (n_valid >= WINDOW_SIZE && varIsValid(window))
      {  
         num_valid_windows++;
         if(getDistance(window) <= max_distance)
         {
            return idx;
         }
      }

   } while (idx < seq_size);
   
   return -1;
}

//-----------------------------------------------------------------------------------
// Builds internal data structures for edit distances.
void SeqScaner::buildDistances(uint16_t a_seq, uint16_t b_seq)
{
   a_dist = new uint8_t[USHRT_MAX + 1];
   b_dist = new uint8_t[USHRT_MAX + 1];

   for(int i = 0; i <= USHRT_MAX; ++i)
   {
      a_dist[i] = twoBitHamming(a_seq, i);
      b_dist[i] = twoBitHamming(b_seq, i);
   }
}

//-----------------------------------------------------------------------------------
// Queries for edit distance and returns the distance to the caller.
inline int SeqScaner::getDistance(uint64_t window)
{
   return a_dist[(window >> A_SEQ_SHIFT) & B_SEQ_MASK] + b_dist[window & B_SEQ_MASK];
}

//-----------------------------------------------------------------------------------
// Shifts the current window, incorporating the next base.
inline void SeqScaner::shiftWindow(uint64_t &window, int &n_valid, char c)
{
   bool valid = true;
   window <<= 2;
   window |= twoBit(c, valid);
   (valid) ? n_valid++ : n_valid = 0;
}

//-----------------------------------------------------------------------------------
// Checks if the base at the variant position matches the expected variant.
inline bool SeqScaner::varIsValid(uint64_t window)
{
   return (((window >> VAR_SHIFT) & MASK) == var);
}