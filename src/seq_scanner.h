#include <string>

#include "util.h"

const int WINDOW_SIZE = 17;

class SeqScaner
{
   public:
      int num_valid_windows = 0;
      
      SeqScaner(const std::string &seq);

      virtual ~SeqScaner() 
      {
         delete[] a_dist;
         delete[] b_dist;
      }
      
      int scan(const char *seq, int seq_size, int max_distance);
      
   private:       
      
      uint8_t *a_dist;
      uint8_t *b_dist;
      uint8_t var;
      
      int getDistance(uint64_t window);
      void shiftWindow(uint64_t &window, int &n_valid, char c);
      bool varIsValid(uint64_t window);
      void buildDistances(uint16_t a_seq, uint16_t b_seq);
};