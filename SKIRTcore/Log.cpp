/*//////////////////////////////////////////////////////////////////
////       SKIRT -- an advanced radiative transfer code         ////
////       © Astronomical Observatory, Ghent University         ////
//////////////////////////////////////////////////////////////////*/

#include "QDateTime"
#include "Log.hpp"

////////////////////////////////////////////////////////////////////

Log::Log()
    : _lowestLevel(Info), _link(0)
{
}

////////////////////////////////////////////////////////////////////

void Log::setLowestLevel(Log::Level level)
{
    _lowestLevel = level;
}

////////////////////////////////////////////////////////////////////

Log::Level Log::lowestLevel() const
{
    return _lowestLevel;
}

////////////////////////////////////////////////////////////////////

void Log::setLinkedLog(Log* log)
{
    if (_link) delete _link;
    _link = log;
    if (_link) _link->setParent(this);
}

////////////////////////////////////////////////////////////////////

Log* Log::linkedLog() const
{
    return _link;
}

////////////////////////////////////////////////////////////////////

void Log::info(QString message)
{
    if (_link) _link->info(message);
    if (Info >= _lowestLevel) output(timestamp() + "   " + message, Info);
}

////////////////////////////////////////////////////////////////////

void Log::warning(QString message)
{
    if (_link) _link->warning(message);
    if (Warning >= _lowestLevel) output(timestamp() + " ! " + message, Warning);
}

////////////////////////////////////////////////////////////////////

void Log::success(QString message)
{
    if (_link) _link->success(message);
    if (Success >= _lowestLevel) output(timestamp() + " - " + message, Success);
}

////////////////////////////////////////////////////////////////////

void Log::error(QString message)
{
    if (_link) _link->error(message);
    if (Error >= _lowestLevel) output(timestamp() + " * *** Error: " + message, Error);
}

////////////////////////////////////////////////////////////////////

QString Log::timestamp()
{
    return QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss.zzz");
}

////////////////////////////////////////////////////////////////////
