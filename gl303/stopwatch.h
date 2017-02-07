#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <QString>
#include <QTime>

class CStopwatch
{
public:
  CStopwatch(QString s);

  void start();
  void stop();
  QString message;

private:
  size_t t;
  QString id;
  QTime timer;
};

#endif
