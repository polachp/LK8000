/*
   LK8000 Tactical Flight Computer -  WWW.LK8000.IT
   Released under GNU/GPL License v.2
   See CREDITS.TXT file for authors and copyrights

   $Id: $
*/

#ifndef __TESTCONTEST_H__
#define __TESTCONTEST_H__

#include "Trace.h"
#include "TimeStamp.h"
#include <fstream>
#include <vector>

class CReplayLogger;

class CTestContest {
  
  class CKMLWrapper {
    mutable std::ofstream _stream;
    
  public:
    CKMLWrapper(const std::string &path);
    ~CKMLWrapper();
    
    void Dump(const CTrace &trace) const;
  };
  
  typedef std::vector<CTimeStamp> CTimeStampArray;
  
  static const unsigned TRACE_SIZE = 20;
  static const unsigned TIME_ANALYSIS_STEP = 500;
  
  const unsigned _handicap;
  const std::string _igcFile;
  CReplayLogger &_replay;
  CKMLWrapper _kml;
  CTrace _trace;
  CTimeStampArray _timeArray;

  static void GPSHandler(void *user, double time, double latitude, double longitude, double altitude);
  
public:
  CTestContest(unsigned handicap, const std::string &igcFile);
  ~CTestContest();
  void Run();
};


#endif /* __TESTCONTEST_H__ */
