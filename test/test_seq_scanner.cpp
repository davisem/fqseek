#define CATCH_CONFIG_MAIN

#include "seq_scanner.h"
#include "catch.hpp"

const int REF_SIZE = 38;
void mutate(std::string &seq, int pos)
{
   char base = seq[pos];
   switch(base)
   {
      case 'A':
         seq[pos] = 'T'; return;
      case 'C':
         seq[pos] = 'G'; return;
      case 'T':
         seq[pos] = 'A'; return;
      case 'G':
         seq[pos] = 'C'; return;
      default:
         throw std::runtime_error("Invalid base " + std::to_string(base));
   }
}

SCENARIO("A query sequence can be found in a reference sequence")
{
   std::string ref = "ATTCCGGTCTACCGCGGCGGTCACACAGTAAACCCTAC";
   
   GIVEN("A substring at the beginning of the sequence")
   {     
      SeqScaner finder(ref.substr(0, WINDOW_SIZE));
      auto j = GENERATE(range(0, WINDOW_SIZE));
      
      AND_GIVEN("A mismatch at any position in the substring")
      {
         mutate(ref, j);
         
         WHEN("Mismatch tolerance is zero")
         {
            REQUIRE(finder.scan(ref.c_str(), REF_SIZE, 0) == -1);
         }
         WHEN("Mismatch tolerance is one")
         {
            if (j != 8)
            {
               REQUIRE(finder.scan(ref.c_str(), REF_SIZE, 1) == WINDOW_SIZE);
            }
         }
      }
   }
   GIVEN("A substring at the end of the sequence")
   {     
      std::string query = ref.substr(REF_SIZE - WINDOW_SIZE, WINDOW_SIZE);
      SeqScaner finder(query);
      auto j = GENERATE(range(REF_SIZE - WINDOW_SIZE, REF_SIZE));
      
      AND_GIVEN("A mismatch at any position in the substring")
      {
         mutate(ref, j);
         
         WHEN("Mismatch tolerance is zero")
         {
            REQUIRE(finder.scan(ref.c_str(), REF_SIZE, 0) == -1);
         }
         WHEN("Mismatch tolerance is one")
         {
            if (j != REF_SIZE - 8 - 1)
            {
               REQUIRE(finder.scan(ref.c_str(), REF_SIZE, 1) == REF_SIZE);
            }
         }
      }
   }
}