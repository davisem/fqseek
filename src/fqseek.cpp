#include <fstream>
#include <iostream>
#include <cctype>
#include <limits>
#include <string>

#include "seq_scanner.h"
#include "util.h"

std::string input_file = "";
std::string input_seq = "";
int max_distance = 0;
const int MAX_KMER_SIZE = 32;
  

//-----------------------------------------------------------------------------------
void showUsage(const char *progname)
{
   std::cerr << "\nUsage: " << progname << " fastq [OPTION...] > outfile\n\n"
   << "REQUIRED:\n"
   << " fastq              Fastq input file\n\n"
   << "OPTIONS:\n"
   << " -seq=str           The sequence pattern to match. The sequence should consist of "
   << "the variant base of interest\n"
   << "                    flanked by 8 bases of context sequence on each side.\n"
   << " -n=N               Report all sequencing with substitution edit distance less than or equal to N.\n"
   << '\n';
}

//-----------------------------------------------------------------------------------
//Parses the input args. Returns false if an invalid arg is encountered.
bool parseArgs(int argc, const char *argv[])
{
   if (argc < 4)
      return false;
   
   for (int i = 1; i < argc; ++i)
   {
      std::string arg = argv[i];
      int arglen = arg.length();
      if (arglen == 0)
         continue;
      
      if (i == 1 && arg[0] != '-')
         input_file = arg;
      else if (arg.substr(1,4) == "seq=")
      {
         input_seq = arg.substr(5);
         if (input_seq.size() != WINDOW_SIZE)
         {
            throw std::runtime_error("Input seq must be exactly " + 
                                     std::to_string(WINDOW_SIZE) + 
                                     " bases: " + arg);
         }
      }
      else if (arg.substr(1,2) == "n="  && isdigit(arg[3]))
         max_distance = std::stoi(arg.substr(3));
      else
      {
         std::cerr << "Invalid arg:" << arg << '\n';
         return false;
      }
   }
   return true;
}

//-----------------------------------------------------------------------------------
// Reads the next line into the buffer, and returns when the lineNum % 4
// This is assumed to be the sequence.
bool getNextSeq(std::ifstream &fastq, int &lineNum, std::string &buffer)
{
   
   while(std::getline(fastq, buffer, '\n'))
   {
      if ((lineNum++ & MASK) == 2)
      {
         if (buffer.size() > MAX_KMER_SIZE)
            return true;
      }
   }
   return false; //reached EOF
}

//-----------------------------------------------------------------------------------
// Writes the found sequences to stdout with some annotations.
void writeSeq(std::string &seq, int idx)
{
   int start_win = idx - WINDOW_SIZE;
   std::cout << std::string(start_win, ' ') << '|'
             << std::string(7, ' ') << '*'
             << std::string(7, ' ') << '|' << '\n' 
             << seq << '\n';
}

//-----------------------------------------------------------------------------------
// Writes summary information to stdout
void report(int max_distance, int numCandidates, int numFound)
{
   std::cout << "Queried " << numCandidates
      << " candidate substrings..." << '\n'
      << "Found " << numFound << " matches with edit distance <= " 
      << max_distance << '\n';
}

int main(int argc, char const *argv[])
{
   try
   {
      if (!parseArgs(argc, argv))
      {
         showUsage(argv[0]);
         return 1;
      }
      
      std::ifstream fastq_fh;
      fastq_fh.open(input_file);
      if (!fastq_fh.is_open())
         throw std::runtime_error("Unable to open " + input_file);
      
      SeqScaner scanner(input_seq);

      int linenum = 1;
      int found = -1;
      int numFound = 0;
      std::string seq;

      while(getNextSeq(fastq_fh, linenum, seq))
      {
         found = scanner.scan(seq.c_str(), seq.size(), max_distance);
         if (found > 0)
         {
            writeSeq(seq, found);
            numFound++;
         }
      }
      report(max_distance, scanner.num_valid_windows, numFound);
   }

   catch(const std::exception& e)
   {
      std::cerr << e.what() << '\n';
   }
   

   return 0;
}
