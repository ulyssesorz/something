#include <cassert>
#include <iostream>
#include <memory>
#include <vector>
using namespace std;

void* Memmove(void* dst, const void* src, size_t length)
{
   assert(dst != NULL && src != NULL);
   void* ret = dst;
   if(dst < src)
   {
      while(length--)
      {
         *(char*)dst = *(char*)src;
         dst = (char*)dst + 1;
         src = (char*)src + 1;
      }
   }
   else {
      dst = (char*)dst + length - 1;
      src = (char*)src + length - 1;
      while(length--)
      {
         *(char*)dst = *(char*)src;
         dst = (char*)dst - 1;
         src = (char*)src - 1;
      }
   }
   return ret;
}

int main()
{
   int length = 5;
   int* src = new int[length];
   for(int i = 0; i < length; ++i)
   {
      src[i] = i;
   }
   int* dst = new int[length];
   Memmove(dst, src, length);
   for(int i = 0; i < length; ++i)
   {
      cout << src[i] << " ";
   }
}