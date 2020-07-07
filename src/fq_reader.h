#ifndef FQ_READER_H
#define FQ_READER_H
#include <string>

const int BUFFER_SIZE = 1 << 20;
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
      virtual ~FqReader() { }
      bool getNext(char *&seq, int &size);

   private:
      char buffer[BUFFER_SIZE];
      int offset;
      int bytes_read;
      int lineNum;
      char *line;
      char *next;
      FILE *fp;
      void copyToFront(char *line);
      void getNextBuffer();
};

#endif