#include "fq_reader.h"
#include "util.h"
#include <iostream>
const int BUFFER_SIZE = 10000000;
//-----------------------------------------------------------------------------------
// ctor
FqReader::FqReader(std::string &filename) : 
   lineNum(0), line(nullptr), fp(nullptr), offset(0), buffer(nullptr)
{
   fp = fopen(filename.c_str(), "r");
   if (!fp)
      throw std::runtime_error("Unable to open " + filename);
   buffer = new char[BUFFER_SIZE];
   getNextBuffer();
}
//-----------------------------------------------------------------------------------
// alternate ctor. Fill the buffer from a string. The string must 
// be smaller than the buffer, so no dynamic buffering is supported.

FqReader::FqReader(char *string, int size) : 
   lineNum(0), line(nullptr), fp(nullptr), offset(0), buffer(nullptr)
{
   if (size > BUFFER_SIZE)
      throw std::runtime_error("String is larger than buffer");
   
   buffer = new char[BUFFER_SIZE];
   for (int i = 0; i < size; ++i)
      buffer[i] = string[i];
   
   bytes_read = size;
   reached_eof = true;
   next = buffer;
}

//-----------------------------------------------------------------------------------
// Retrieves the next sequence from from the internal buffer, 
// and reports the sequence size. Returns false when EOF is reached.
bool FqReader::getNext(char *&seq, int &size)
{
   do
   {
      do
      {
         if (*next == '\n')
         {
            if ((lineNum & MASK) == 0)
            {
               line = ++next;
            }
            else if ((lineNum & MASK) == 1)
            {
               *next = '\0'; //replace '\n' with null terminator
               seq = line;
               size = next - line;
               ++next;
               ++lineNum;
               line = nullptr;
               return true;
            }
            ++lineNum;
         }
      } while(++next < endOfBuffer());

      if (line)//the buffer ended, but we never reached the end of the line.
      {
         copyToFront(line);//the truncated line gets copied to the front of the next buffer.
      }
   } while(getNextBuffer());
   
   return false;
}

inline char *FqReader::endOfBuffer()
{
   return buffer + offset + bytes_read;
}

//-----------------------------------------------------------------------------------
// Refills the buffer, adjusting for any parital lines in the previous buffer that
// were truncated before reached end of line.
bool FqReader::getNextBuffer()
{
   if (!fp)
      return false;

   bytes_read = fread(buffer + offset, sizeof(char), BUFFER_SIZE - offset, fp);
   reached_eof = offset + bytes_read < BUFFER_SIZE ? true : false;
   next = buffer;
   return (bytes_read > 0);
}

//-----------------------------------------------------------------------------------
// This is called when a line has been split across buffers. The length of the 
// truncated line is calculated with pointer arithmetic, and coppied to the front
// of the next buffer (which is actually the same buffer. The front gets overwritten).
void FqReader::copyToFront(char *line)
{
   offset = next - line;
   for (int i = 0; i < offset; ++i)
   {
      buffer[i] = line[i];
   }
   line = buffer;
}

void FqReader::printBuffer(int offset)
{
   for (int i = offset; i < BUFFER_SIZE; ++i)
      std::cout << buffer[i];
}
