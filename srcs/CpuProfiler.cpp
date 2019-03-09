#include "CpuProfiler.hpp"

#include <Windows.h>

namespace CIOS
{

CpuProfiler:: CpuProfiler (void)
            : Thread      (    )
{
  details       = nullptr ;
  monitor       = nullptr ;
  process       = nullptr ;
  T1            = nullptr ;
  T2            = nullptr ;
  StartTime     = 0       ;
  RecordingTime = 0       ;
  TagId         = 0       ;
  MaxTags       = 0       ;
}

CpuProfiler::~CpuProfiler (void)
{
  TAG [ 0 ] = nullptr                ;
  TAG [ 1 ] = nullptr                ;
  if ( nullptr != T1 ) delete [ ] T1 ;
  if ( nullptr != T2 ) delete [ ] T2 ;
  T1  = nullptr                      ;
  T2  = nullptr                      ;
}

bool CpuProfiler::Create(int recordTime)
{
  int64_t total = sizeof(CIOS::CpuDetails) + sizeof(CIOS::ProcessDetails) ;
  if ( ! SHM . Create ( "CpuProfiler" , total ) ) return false            ;
  if ( ! SHM . isCreated  ( )                   ) return false            ;
  if ( ! SHM . isAttached ( )                   ) return false            ;
  /////////////////////////////////////////////////////////////////////////
  char * p = SHM . Memory ( )                                             ;
  details = (CpuDetails     *) ( p                            )           ;
  monitor = (MonitorDetails *) ( p + sizeof(CpuDetails) - 768 )           ;
  process = (ProcessDetails *) ( p + sizeof(CpuDetails)       )           ;
  /////////////////////////////////////////////////////////////////////////
  ::memset ( p , 0 , (size_t) total )                                     ;
  strcpy_s ( details -> Name        , 224 , "CpuProfiler" )               ;
  strcpy_s ( process -> application , 256 , "CpuProfiler" )               ;
  /////////////////////////////////////////////////////////////////////////
  Totals   [ 0 ] = 0                                                      ;
  Totals   [ 1 ] = 0                                                      ;
  Overflow [ 0 ] = 0                                                      ;
  Overflow [ 1 ] = 0                                                      ;
  TagId          = 0                                                      ;
  MaxTags        = recordTime * 2                                         ;
  T1             = new CpuTag [ MaxTags + 2000 ]                          ;
  T2             = new CpuTag [ MaxTags + 2000 ]                          ;
  TAG      [ 0 ] = T1                                                     ;
  TAG      [ 1 ] = T2                                                     ;
  RecordingTime  = recordTime * 1000000LL                                 ;
  /////////////////////////////////////////////////////////////////////////
  CPU . Tell     ( details )                                              ;
  CPU . Tell     ( process )                                              ;
  CPU . GetUsage ( true    )                                              ;
  /////////////////////////////////////////////////////////////////////////
  return true                                                             ;
}

void CpuProfiler::run(int T,CIOS::ThreadData * d)
{
  switch ( T )        {
    case 1001         :
      CpuLookup ( d ) ;
    break             ;
    case 1002         :
      RecordCpu ( d ) ;
    break             ;
  }                   ;
}

void CpuProfiler::CpuLookup(ThreadData * d)
{
  int32_t x                                                                  ;
  int64_t v                                                                  ;
  ////////////////////////////////////////////////////////////////////////////
  StartTime      = StarDate::ustamp ( )                                      ;
  Totals   [ 0 ] = 0                                                         ;
  Totals   [ 1 ] = 0                                                         ;
  Overflow [ 0 ] = 0                                                         ;
  Overflow [ 1 ] = 0                                                         ;
  TagId          = 0                                                         ;
  ////////////////////////////////////////////////////////////////////////////
  CpuTag * tag                                                               ;
  while ( IsContinue ( ) )                                                   {
    //////////////////////////////////////////////////////////////////////////
    msleep ( 500 )                                                           ;
    //////////////////////////////////////////////////////////////////////////
    CPU . Tell ( process )                                                   ;
    process -> machine = CPU  . GetUsage  ( true )                           ;
    monitor -> ustamp  = StarDate::ustamp (      )                           ;
    //////////////////////////////////////////////////////////////////////////
    v   = process -> total - process -> available                            ;
    x   = (int32_t) ( ( v * 10000 ) / process -> total )                     ;
    //////////////////////////////////////////////////////////////////////////
    tag  = & ( TAG [ TagId ] [ Totals [ TagId ] ] )                          ;
    tag -> CPU    = process -> machine                                       ;
    tag -> Memory = x                                                        ;
    tag -> Stamp  = monitor -> ustamp                                        ;
    //////////////////////////////////////////////////////////////////////////
    Totals [ TagId ] ++                                                      ;
    v    = monitor -> ustamp - StartTime                                     ;
    if ( ( v > RecordingTime ) || ( Totals [ TagId ] >= MaxTags ) )          {
      Overflow [ TagId ] = 1                                                 ;
      TagId              = ( TagId + 1 ) % 2                                 ;
      Totals   [ TagId ] = 0                                                 ;
      Overflow [ TagId ] = 0                                                 ;
      StartTime          = monitor -> ustamp                                 ;
            printf("Switch to %d\n",TagId) ;
    }                                                                        ;
    //////////////////////////////////////////////////////////////////////////
  }                                                                          ;
}

void CpuProfiler::RecordCpu(ThreadData * d)
{
  while ( IsContinue ( ) )                                                   {
    msleep ( 500 )                                                           ;
    if ( ( Overflow [ 0 ] > 0 ) || ( Overflow [ 1 ] > 0 ) )                  {
      int id = -1                                                            ;
      if ( ( id < 0 ) && ( Overflow [ 0 ] > 0 ) ) id = 0                     ;
      if ( ( id < 0 ) && ( Overflow [ 1 ] > 0 ) ) id = 1                     ;
      if ( id >= 0 )                                                         {
        //////////////////////////////////////////////////////////////////////
        if ( Save ( Totals[ id ] , TAG [ id ] ) )                            {
          Overflow [ id ] = 0                                                ;
          Totals   [ id ] = 0                                                ;
        }                                                                    ;
        //////////////////////////////////////////////////////////////////////
      }                                                                      ;
    }                                                                        ;
  }                                                                          ;
}

bool CpuProfiler::Save(int64_t total,CpuTag * cpu)
{
  if ( total <= 0 ) return false                                             ;
  ////////////////////////////////////////////////////////////////////////////
  int64_t   ttt = cpu [ 0 ] . Stamp                                          ;
  int64_t   mmm = ttt % 1000000LL                                            ;
  time_t    now = (time_t) ( ttt / 1000000LL )                               ;
  struct tm ts                                                               ;
  localtime_s ( &ts , &now )                                                 ;
  mmm /= 1000                                                                ;
  ////////////////////////////////////////////////////////////////////////////
  char filename [ 1024 ]                                                     ;
  char hour     [ 1024 ]                                                     ;
  char day      [ 1024 ]                                                     ;
  char month    [ 1024 ]                                                     ;
  char mm       [ 1024 ]                                                     ;
  char ss       [ 1024 ]                                                     ;
  char uu       [ 1024 ]                                                     ;
  char xx       [ 1024 ]                                                     ;
  char dirname  [ 1024 ]                                                     ;
  if ( ts . tm_sec  > 9 ) sprintf ( ss       ,  "%d" , ts . tm_sec     )     ;
                     else sprintf ( ss       , "0%d" , ts . tm_sec     )     ;
  if ( ts . tm_min  > 9 ) sprintf ( mm       ,  "%d" , ts . tm_min     )     ;
                     else sprintf ( mm       , "0%d" , ts . tm_min     )     ;
  if ( ts . tm_hour > 9 ) sprintf ( hour     ,  "%d" , ts . tm_hour    )     ;
                     else sprintf ( hour     , "0%d" , ts . tm_hour    )     ;
  if ( ts . tm_mday > 9 ) sprintf ( day      ,  "%d" , ts . tm_mday    )     ;
                     else sprintf ( day      , "0%d" , ts . tm_mday    )     ;
  if ( ts . tm_mon  > 8 ) sprintf ( month    ,  "%d" , ts . tm_mon + 1 )     ;
                     else sprintf ( month    , "0%d" , ts . tm_mon + 1 )     ;
  sprintf ( dirname  , "%d-%s-%s" , ts.tm_year+1900 , month , day      )     ;
  sprintf ( filename , "%s.txt"   , hour                               )     ;
  sprintf ( uu , "%lld" , mmm                                          )     ;
  strcpy  ( xx , uu                                                    )     ;
  while ( strlen ( xx ) < 3 )                                                {
    sprintf ( xx , "0%s" , uu )                                              ;
    strcpy  ( uu , xx                                                  )     ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  std::string dirpath = TempDir + std::string("\\") + std::string(dirname )  ;
  std::string fname   = dirpath + std::string(".txt")                        ;
  ////////////////////////////////////////////////////////////////////////////
  FILE * fp = fopen(fname.c_str(),"a+")                                      ;
  if ( nullptr != fp )                                                       {
    for (int i = 0 ; i < total ; i++ )                                       {
      ////////////////////////////////////////////////////////////////////////
      ttt = cpu [ i ] . Stamp                                                ;
      mmm = ttt % 1000000LL                                                  ;
      now = (time_t) ( ttt / 1000000LL )                                     ;
      localtime_s ( &ts , &now )                                             ;
      mmm /= 1000                                                            ;
      ////////////////////////////////////////////////////////////////////////
      sprintf ( uu , "%lld" , mmm                                          ) ;
      strcpy  ( xx , uu                                                    ) ;
      while ( strlen ( xx ) < 3 )                                            {
        sprintf ( xx , "0%s" , uu )                                          ;
        strcpy  ( uu , xx                                                  ) ;
      }                                                                      ;
      ////////////////////////////////////////////////////////////////////////
      if ( ts . tm_sec  > 9 ) sprintf ( ss       ,  "%d" , ts . tm_sec     ) ;
                         else sprintf ( ss       , "0%d" , ts . tm_sec     ) ;
      if ( ts . tm_min  > 9 ) sprintf ( mm       ,  "%d" , ts . tm_min     ) ;
                         else sprintf ( mm       , "0%d" , ts . tm_min     ) ;
      if ( ts . tm_hour > 9 ) sprintf ( hour     ,  "%d" , ts . tm_hour    ) ;
                         else sprintf ( hour     , "0%d" , ts . tm_hour    ) ;
      ////////////////////////////////////////////////////////////////////////
      fprintf                                                                (
        fp                                                                   ,
        "%d %d %lld %s:%s:%s.%s\r\n"                                         ,
        cpu [ i ] . CPU                                                      ,
        cpu [ i ] . Memory                                                   ,
        cpu [ i ] . Stamp                                                    ,
        hour                                                                 ,
        mm                                                                   ,
        ss                                                                   ,
        uu                                                                 ) ;
    }                                                                        ;
    fclose ( fp )                                                            ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  return true                                                                ;
}

}

static bool * Continue = nullptr;

void CpuSignal(int no)
{
  if ( nullptr != Continue ) {
    ( *Continue ) = false    ;
  }                          ;
  CIOS::StarDate::sleep(2)   ;
}

int main(int argc,char * argv [])
{
  ////////////////////////////////////////////////////////////////////////////
  char * CPUPROFILER = getenv ( "CPUPROFILER" )                              ;
  char * CPUINTERVAL = getenv ( "CPUINTERVAL" )                              ;
  char * TEMP        = getenv ( "TEMP"        )                              ;
  char * TMP         = getenv ( "TMP"         )                              ;
  ////////////////////////////////////////////////////////////////////////////
  std::string temp                                                           ;
  int         recordTime = 3600                                              ;
  if ( nullptr != TMP         ) temp       = TMP                             ;
  if ( nullptr != TEMP        ) temp       = TEMP                            ;
  if ( nullptr != CPUPROFILER ) temp       = CPUPROFILER                     ;
  if ( nullptr != CPUINTERVAL ) recordTime = atoi ( CPUINTERVAL )            ;
  ////////////////////////////////////////////////////////////////////////////
  int    argi        = 1                                                     ;
  while ( argi < argc )                                                      {
    if ( 0 == strcmp ( argv [ argi ] , "--interval" ) )                      {
      argi++                                                                 ;
      recordTime = atoi ( argv [ argi ] )                                    ;
      argi++                                                                 ;
    } else
    if ( 0 == strcmp ( argv [ argi ] , "--dir" ) )                           {
      argi++                                                                 ;
      temp = strdup  ( argv [ argi ] )                                       ;
      argi++                                                                 ;
    } else {                                                                 ;
      argi++                                                                 ;
    }                                                                        ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  signal ( SIGABRT , CpuSignal ) ; // Abnormal termination
  signal ( SIGINT  , CpuSignal ) ; // CTRL+C signal
  signal ( SIGTERM , CpuSignal ) ; // Termination request
  ////////////////////////////////////////////////////////////////////////////
  CIOS::CpuProfiler profiler                                                 ;
  profiler . TempDir = temp                                                  ;
  profiler . Create ( recordTime )                                           ;
  Continue = & ( profiler . monitor -> Continue ) ;
  *Continue = true ;
  profiler . Controller = Continue ;
  profiler . start  ( 1001 ) ;
  profiler . start  ( 1002 ) ;
  while ( *Continue ) {
    CIOS::StarDate::sleep(1) ;
  }
  ////////////////////////////////////////////////////////////////////////////
  return 0                                                                   ;
}
