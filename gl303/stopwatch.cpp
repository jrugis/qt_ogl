#include "stopwatch.h"

CStopwatch::CStopwatch(QString s)
{
  t = 0;
  id = s;
  message = id + "0";
}

void CStopwatch::start()
{
  timer.restart();
}

void CStopwatch::stop()
{
  t = timer.elapsed();
  message = id + QString::number(t);
}
