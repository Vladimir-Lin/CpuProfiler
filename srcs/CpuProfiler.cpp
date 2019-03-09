#include "UUIDs.hpp"

#include <stdlib.h>
#include <string.h>

SUID randomUpperSuid (void)
{
  SUID v [ 8 ]               ;
  SUID s                     ;
  v [0]  = rand ( ) % 256    ;
  v [1]  = rand ( ) % 256    ;
  v [2]  = rand ( ) % 256    ;
  v [3]  = rand ( ) % 256    ;
  v [4]  = rand ( ) % 256    ;
  v [5]  = rand ( ) % 256    ;
  v [6]  = rand ( ) % 256    ;
  v [7]  = rand ( ) % 114    ;
  v [7] +=             14    ;
  s      = v [ 7 ] ; s <<= 8 ;
  s     |= v [ 6 ] ; s <<= 8 ;
  s     |= v [ 5 ] ; s <<= 8 ;
  s     |= v [ 4 ] ; s <<= 8 ;
  s     |= v [ 3 ] ; s <<= 8 ;
  s     |= v [ 2 ] ; s <<= 8 ;
  s     |= v [ 1 ] ; s <<= 8 ;
  s     |= v [ 0 ]           ;
  return s                   ;
}

SUID randomUpperSuid (SUID header)
{
  SUID uuid = randomUpperSuid ( )   ;
  SUID huid = header * 10           ;
  SUID divd = 1000000000000000000LL ;
  while ( header > 0 )              {
    uuid   /= 10                    ;
    divd   /= 10                    ;
    header /= 10                    ;
  }                                 ;
  huid *= divd                      ;
  uuid += huid                      ;
  return uuid                       ;
}

SUID randomLongSuid (void)
{
  SUID v [ 8 ]               ;
  SUID s                     ;
  v [0]  = rand ( ) % 256    ;
  v [1]  = rand ( ) % 256    ;
  v [2]  = rand ( ) % 256    ;
  v [3]  = rand ( ) % 256    ;
  v [4]  = rand ( ) % 256    ;
  v [5]  = rand ( ) % 256    ;
  v [6]  = rand ( ) % 256    ;
  v [7]  = rand ( ) % 114    ;
  v [7] +=            142    ;
  s      = v [ 7 ] ; s <<= 8 ;
  s     |= v [ 6 ] ; s <<= 8 ;
  s     |= v [ 5 ] ; s <<= 8 ;
  s     |= v [ 4 ] ; s <<= 8 ;
  s     |= v [ 3 ] ; s <<= 8 ;
  s     |= v [ 2 ] ; s <<= 8 ;
  s     |= v [ 1 ] ; s <<= 8 ;
  s     |= v [ 0 ]           ;
  return s                   ;
}

SUID randomLongSuid (SUID tailer)
{
  SUID uuid = randomLongSuid ( )       ;
  SUID V    = ( rand ( ) % 114 ) + 142 ;
  SUID T    = tailer                   ;
  SUID C    = 0                        ;
  while ( T > 0 )                      {
    T    /= 10                         ;
    uuid /= 10                         ;
    C++                                ;
  }                                    ;
  if ( C > 0 )                         {
    for (SUID i = 0 ; i < C ; i++ )    {
      uuid *= 10                       ;
    }                                  ;
  }                                    ;
  V    <<= 56                          ;
  uuid >>= 8                           ;
  uuid  += V                           ;
  uuid  += tailer                      ;
  return uuid                          ;
}

SUID randomSuid(SUID extension,bool upper)
{
  if   ( upper         )                                      {
    if ( extension > 0 ) return randomUpperSuid ( extension ) ;
                    else return randomUpperSuid (           ) ;
  } else                                                      {
    if ( extension > 0 ) return randomLongSuid  ( extension ) ;
                    else return randomLongSuid  (           ) ;
  }                                                           ;
}

TUID randomTuid(void)
{
  TUID v [ 8 ]               ;
  TUID s                     ;
  v [0]  = rand ( ) % 256    ;
  v [1]  = rand ( ) % 256    ;
  v [2]  = rand ( ) % 256    ;
  v [3]  = rand ( ) % 256    ;
  v [4]  = rand ( ) % 256    ;
  v [5]  = rand ( ) % 256    ;
  v [6]  = rand ( ) % 256    ;
  v [7]  = rand ( ) % 114    ;
  v [7] +=             14    ;
  s      = v [ 7 ] ; s <<= 8 ;
  s     |= v [ 6 ] ; s <<= 8 ;
  s     |= v [ 5 ] ; s <<= 8 ;
  s     |= v [ 4 ] ; s <<= 8 ;
  s     |= v [ 3 ] ; s <<= 8 ;
  s     |= v [ 2 ] ; s <<= 8 ;
  s     |= v [ 1 ] ; s <<= 8 ;
  s     |= v [ 0 ]           ;
  return s                   ;
}

TUID randomTuid(TUID header)
{
  TUID uuid = randomTuid ( )        ;
  TUID huid = header * 10           ;
  TUID divd = 1000000000000000000LL ;
  while ( header > 0 )              {
    uuid   /= 10                    ;
    divd   /= 10                    ;
    header /= 10                    ;
  }                                 ;
  huid *= divd                      ;
  uuid += huid                      ;
  return uuid                       ;
}

SUID suidFromHex(const char * s,size_t len)
{
  if ( nullptr == s   ) return 0                                  ;
  if ( 0       == len ) len = ::strlen ( s )                      ;
  if ( 0       == len ) return 0                                  ;
  SUID v = 0                                                      ;
  for (size_t i = 0 ; i < len ; i++ )                             {
    char c = s [ i ]                                              ;
    v <<= 4                                                       ;
    if ( ( c >= '0' ) && ( c <= '9' ) ) v |= uint32_t ( c - '0' ) ; else
    if ( ( c >= 'A' ) && ( c <= 'F' ) ) v |= uint32_t ( c - 'A' ) ; else
    if ( ( c >= 'a' ) && ( c <= 'f' ) ) v |= uint32_t ( c - 'a' ) ;
  }                                                               ;
  return v                                                        ;
}

TUID tuidFromHex(const char * s,size_t len)
{
  if ( nullptr == s   ) return 0                                  ;
  if ( 0       == len ) len = ::strlen ( s )                      ;
  if ( 0       == len ) return 0                                  ;
  TUID v = 0                                                      ;
  for (size_t i = 0 ; i < len ; i++ )                             {
    char c = s [ i ]                                              ;
    v <<= 4                                                       ;
    if ( ( c >= '0' ) && ( c <= '9' ) ) v |= uint32_t ( c - '0' ) ; else
    if ( ( c >= 'A' ) && ( c <= 'F' ) ) v |= uint32_t ( c - 'A' ) ; else
    if ( ( c >= 'a' ) && ( c <= 'f' ) ) v |= uint32_t ( c - 'a' ) ;
  }                                                               ;
  return v                                                        ;
}

void * suidArray(size_t size)
{
  return (void *) ::malloc ( sizeof(SUID) * size ) ;
}

void * suidResize(void * block,size_t size)
{
  return (void *) ::realloc ( block , sizeof(SUID) * size ) ;
}

void suidFree (void * block)
{
  if ( nullptr == block ) return ;
  ::free ( block )               ;
}

int suidCompare(const void * a, const void * b)
{
  SUID c = * (SUID *) a   ;
  SUID d = * (SUID *) b   ;
  if ( c == d ) return  0 ;
  if ( c  < d ) return -1 ;
                return  1 ;
}

void suidSort(void * block,size_t size)
{
  if ( size < 2 ) return                                ;
  ::qsort ( block , size , sizeof(SUID) , suidCompare ) ;
}

int tuidCompare(const void * a, const void * b)
{
  TUID c = * (TUID *) a   ;
  TUID d = * (TUID *) b   ;
  if ( c == d ) return  0 ;
  if ( c  < d ) return -1 ;
                return  1 ;
}

void tuidSort(void * block,size_t size)
{
  if ( size < 2 ) return                                ;
  ::qsort ( block , size , sizeof(TUID) , tuidCompare ) ;
}

size_t suidUnique(void * unique,const void * block,size_t size)
{
  ////////////////////////////////////////
  if ( 0       >= size   ) return 0      ;
  if ( nullptr == unique ) return 0      ;
  if ( nullptr == block  ) return 0      ;
  ////////////////////////////////////////
  const SUID * A = (const SUID *) block  ;
        SUID * B = (      SUID *) unique ;
  size_t       n = 0                     ;
  size_t       p = 1                     ;
  ////////////////////////////////////////
  B [ 0 ] = A [ 0 ]                      ;
  for (int i = 1 ; i < size ; i++,n++)   {
    if ( A [ n ] != A [ i ] )            {
      B [ p ] = A [ i ]                  ;
      p++                                ;
    }                                    ;
  }                                      ;
  ////////////////////////////////////////
  return p                               ;
}

size_t tuidUnique(void * unique,const void * block,size_t size)
{
  ////////////////////////////////////////
  if ( 0       >= size   ) return 0      ;
  if ( nullptr == unique ) return 0      ;
  if ( nullptr == block  ) return 0      ;
  ////////////////////////////////////////
  const TUID * A = (const TUID *) block  ;
        TUID * B = (      TUID *) unique ;
  size_t       n = 0                     ;
  size_t       p = 1                     ;
  ////////////////////////////////////////
  B [ 0 ] = A [ 0 ]                      ;
  for (int i = 1 ; i < size ; i++,n++)   {
    if ( A [ n ] != A [ i ] )            {
      B [ p ] = A [ i ]                  ;
      p++                                ;
    }                                    ;
  }                                      ;
  ////////////////////////////////////////
  return p                               ;
}

void suidReverse(void * reverse,const void * block,size_t size)
{
  ////////////////////////////////////////////
  if ( 0       >= size    ) return           ;
  if ( nullptr == reverse ) return           ;
  if ( nullptr == block   ) return           ;
  ////////////////////////////////////////////
  const SUID * A = (const SUID *) block      ;
        SUID * B = (      SUID *) reverse    ;
  size_t       n = size - 1                  ;
  ////////////////////////////////////////////
  for (size_t i = 0 ; i < size ; i++ , n-- ) {
    B [ n ] = A [ i ]                        ;
  }                                          ;
  ////////////////////////////////////////////
}

void tuidReverse(void * reverse,const void * block,size_t size)
{
  ////////////////////////////////////////////
  if ( 0       >= size    ) return           ;
  if ( nullptr == reverse ) return           ;
  if ( nullptr == block   ) return           ;
  ////////////////////////////////////////////
  const SUID * A = (const SUID *) block      ;
        SUID * B = (      SUID *) reverse    ;
  size_t       n = size - 1                  ;
  ////////////////////////////////////////////
  for (size_t i = 0 ; i < size ; i++ , n-- ) {
    B [ n ] = A [ i ]                        ;
  }                                          ;
  ////////////////////////////////////////////
}
