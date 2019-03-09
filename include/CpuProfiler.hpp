/****************************************************************************\
 *
 *                         CPU Profiler Toolsets
 *
\****************************************************************************/

#ifndef CPU_PROFILER_HPP
#define CPU_PROFILER_HPP

#include "UUIDs.hpp"
#include "stardate.hpp"
#include "parallel.hpp"

#include <signal.h>

namespace CIOS
{

#pragma pack(push,1)

typedef struct   {
  bool Continue  ;
} MonitorDetails ;

#pragma pack(pop)

class CpuProfiler : public Thread
{
  public:

    CPU::Usage       cpu     ;
    SharedMemory     SHM     ;
    CpuDetails     * details ;
    MonitorDetails * monitor ;
    ProcessDetails * process ;

    explicit     CpuProfiler (void) ;
    virtual     ~CpuProfiler (void) ;

    bool         Create      (void) ;

  protected:

    virtual void run         (int Type,CIOS::ThreadData * data) ;

    void         CpuLookup   (ThreadData * data) ;

  private:

} ;

} ;

#endif
