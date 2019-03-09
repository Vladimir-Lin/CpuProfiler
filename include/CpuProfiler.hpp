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

typedef struct     {
  int32_t CPU      ;
  int32_t Memory   ;
  int64_t Stamp    ;
} CpuTag           ;

typedef struct     {
  bool    Continue ;
  int64_t ustamp   ;
} MonitorDetails   ;

#pragma pack(pop)

class CpuProfiler : public Thread
{
  public:

    CPU::Usage       CPU            ;
    SharedMemory     SHM            ;
    CpuDetails     * details        ;
    MonitorDetails * monitor        ;
    ProcessDetails * process        ;
    CpuTag         * T1             ;
    CpuTag         * T2             ;
    CpuTag         * TAG [ 2 ]      ;
    int64_t          StartTime      ;
    int64_t          RecordingTime  ;
    int64_t          Totals   [ 2 ] ;
    int64_t          Overflow [ 2 ] ;
    int              TagId          ;
    int              MaxTags        ;
    std::string      TempDir        ;

    explicit     CpuProfiler (void) ;
    virtual     ~CpuProfiler (void) ;

    bool         Create      (int recordTime) ;

  protected:

    virtual void run         (int Type,CIOS::ThreadData * data) ;

    void         CpuLookup   (ThreadData * data) ;
    void         RecordCpu   (ThreadData * data) ;

    bool         Save        (int64_t total,CpuTag * cpu) ;

  private:

} ;

} ;

#endif
