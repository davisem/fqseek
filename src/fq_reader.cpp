#include "fq_reader.h"
#include "util.h"
#include <iostream>

//-----------------------------------------------------------------------------------
// ctor

FqReader::FqReader(std::string &filename) : 
   lineNum(0), line(nullptr), fp(nullptr), offset(0), reached_eof(false)
{
   fp = fopen(filename.c_str(), "r");
   if (!fp)
      throw std::runtime_error("Unable to open " + filename);
   
   getNextBuffer();
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

      } while(next++ < endOfBuffer());//until we reach the end of buffer
      
      if (!reached_eof)
      {
         if (line)//the buffer ended, but we never reached the end of the line.
         {
            copyToFront(line);//the truncated line gets copied to the front of the next buffer.
         }
         
         getNextBuffer();
         continue;
      }
   
   } while(!reached_eof);
   
   return false;
}
//-----------------------------------------------------------------------------------
// Refills the buffer, adjusting for any parital lines in the previous buffer that
// were truncated before reached end of line.
inline char *FqReader::endOfBuffer()
{
   return buffer + offset + bytes_read;
}

void FqReader::getNextBuffer()
{
   bytes_read = fread(buffer + offset, sizeof(char), BUFFER_SIZE - offset, fp);
   reached_eof = offset + bytes_read < BUFFER_SIZE ? true : false;
   next = buffer;
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
      buffer[i] = *line;
      ++line;
   }
   line = buffer;
}