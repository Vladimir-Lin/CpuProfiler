#include "CpuProfiler.hpp"

namespace CIOS
{

CpuProfiler:: CpuProfiler (void)
            : Thread      (    )
{
  details = nullptr ;
  process = nullptr ;
}

CpuProfiler::~CpuProfiler (void)
{
}

bool CpuProfiler::Create(void)
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
  cpu . Tell ( details )                                                  ;
  cpu . Tell ( process )                                                  ;
  /////////////////////////////////////////////////////////////////////////
  return true                                                             ;
}

void CpuProfiler::run(int T,CIOS::ThreadData * d)
{
  switch ( T )        {
    case 1001         :
      CpuLookup ( d ) ;
    break             ;
  }                   ;
}

void CpuProfiler::CpuLookup(ThreadData * d)
{
  while ( IsContinue ( ) )                                                   {
    cpu . Tell ( process )                                                   ;
    process -> machine   = cpu . GetUsage  ( false )                         ;
    process -> process   = cpu . GetUsage  ( true  )                         ;
    process -> timestamp = cpu . Timestamp (       )                         ;
        printf ( "%d %d\n" ,
                 process -> machine   ,
                 process -> process ) ;
    sleep ( 1 )                                                              ;
  }                                                                          ;
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
  signal ( SIGABRT , CpuSignal ) ; // Abnormal termination
  signal ( SIGINT  , CpuSignal ) ; // CTRL+C signal
  signal ( SIGTERM , CpuSignal ) ; // Termination request
  ////////////////////////////////////////////////////////////////////////////
  CIOS::CpuProfiler profiler ;
  profiler . Create ( ) ;
  Continue = & ( profiler . monitor -> Continue ) ;
  *Continue = true ;
  profiler . Controller = Continue ;
  profiler . start  ( 1001 ) ;
  while ( *Continue ) {
    CIOS::StarDate::sleep(1) ;
  }
  ////////////////////////////////////////////////////////////////////////////
  return 0                                                                   ;
}
