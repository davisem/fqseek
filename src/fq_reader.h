#ifndef FQ_READER_H
#define FQ_READER_H
#include <string>

//-----------------------------------------------------------------------------------
// FqReader allows for better IO performance when reading large files such as fastqs.
// It holds it's own internal data buffer, which is filled with a large chunk of the fastq
// in single call in hopes to reduce context switching. 
// The caller can just keep calling getNext() while checking the return value.
// True is returned if it could get next sequence. False is returned if it can't (reached end of file).
class FqReader
{
   public:
      bool reached_eof;

      FqReader(std::string &filename);
      FqReader(char *string, int size);
      virtual ~FqReader() 
      {
         delete[] buffer;
      }
      bool getNext(char *&seq, int &size);
      void printBuffer(int offset);

   private:
      char *buffer;
      int offset;
      int bytes_read;
      int lineNum;
      char *line;
      char *next;
      FILE *fp;
      void copyToFront(char *line);
      bool getNextBuffer();
      char *endOfBuffer();
};

#endif
