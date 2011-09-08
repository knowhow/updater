/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2010 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "xversion.h"

#include <QStringList>
#include <QObject>
#include <QRegExp>

#define DEBUG false

XVersion::XVersion()
  : _major(-1), _minor(-1), _point(-1), _stage(UNKNOWN), _substage(-1)
{
}

XVersion::XVersion(const QString &pString)
  : _major(-1), _minor(-1), _point(-1), _stage(UNKNOWN), _substage(-1)
{
  setVersion(pString);
}

XVersion::XVersion(const XVersion &pVersion)
{
  _major = pVersion._major;
  _minor = pVersion._minor;
  _point = pVersion._point;
  _stage = pVersion._stage;
  _substage = pVersion._substage;
}

XVersion::~XVersion()
{
}

bool XVersion::isValid() const
{
  if (_major == -1 || _stage == UNKNOWN)
    return false;
  else
    return true;
}

void XVersion::setVersion(const QString &pString)
{
  QStringList suffixes;
  suffixes << "wip" << "alpha" << "beta" << "rc";
  QRegExp relregex("^(\\d+)\\.(\\d+)(\\.\\d+)?(?:(" +
                   suffixes.join("|") +
                   ")(\\d*)?)?$");

  if (relregex.indexIn(pString.toLower()) == -1)
    return;

  if (relregex.cap(1).isEmpty())
    return;
  _major = relregex.cap(1).toInt();

  if (relregex.cap(2).isEmpty())
    return;
  _minor = relregex.cap(2).toInt();

  if (! relregex.cap(3).isEmpty())
    _point = relregex.cap(3).mid(1).toInt();

  if (relregex.cap(4).isEmpty())
  {
    _stage    = FINAL;
    _substage = 0;
  }
  else switch (suffixes.indexOf(relregex.cap(4)))
  {
    case 0:  _stage = WIP;     _substage = 0; break;
    case 1:  _stage = ALPHA;   _substage = 0; break;
    case 2:  _stage = BETA;    _substage = 0; break;
    case 3:  _stage = RC;      _substage = 0; break;
    default: _stage = UNKNOWN;                break;
  }

  if (! relregex.cap(5).isEmpty())
    _substage = relregex.cap(5).toInt();
}

int XVersion::majorNumber(bool &ok) const
{
  ok = (_major >= 0);
  return _major;
}

int XVersion::minorNumber(bool &ok) const
{
  ok = (_minor >= 0);
  return _minor;
}

int XVersion::pointNumber(bool &ok) const
{
  if (_point >= 0)
  {
    ok = true;
    return _point;
  }
  else
  {
    ok = false;
    return 0;
  }
}

XVersion::ReleaseStage XVersion::stage(bool &ok) const
{
  ok = (_stage != UNKNOWN);
  return _stage;
}

int XVersion::substageNumber(bool &ok) const
{
  ok = (_substage >= 0);
  return _substage;
}

QString XVersion::toString() const
{
  QString result = QObject::tr("invalid");

  if (_major >= 0)
  {
    result = QString::number(_major);
    if (_minor >= 0)
    {
      result += "." + QString::number(_minor);
      if (_point >= 0)
      {
        result += "." + QString::number(_point);
      }
    }
    switch (_stage)
    {
      case WIP:   result += "wip";    break;
      case ALPHA: result += "alpha";  break;
      case BETA:  result += "beta";   break;
      case RC:    result += "rc";     break;
      case FINAL: break;
      case UNKNOWN:
      default:    result = QObject::tr("invalid");
    }

    if (_substage > 0 && (_stage == WIP ||
                          _stage == ALPHA ||
                          _stage == BETA ||
                          _stage == RC))
      result += QString::number(_substage);
  }

  return result;
}

// NOTE:
// all of the comparisons return false if either version is obviously invalid

bool XVersion::operator==(XVersion other)
{
  bool ok;
  if (! isValid() || ! other.isValid())
    return false;
  else
    return (_major          == other._major &&
            _minor          == other._minor &&
            pointNumber(ok) == other.pointNumber(ok) &&     // treat uninit as 0
            _stage          == other._stage  &&
            _substage       == other._substage);
}

/* can't simple say if (major > other.major && minor > other.minor ...
   because v2.3 > v1.4 but 3 < 4 and so would fail here: ^^^^^^^^^^^^^^^^^^
 */
bool XVersion::operator>(XVersion other)
{
  if      (! isValid() || !other.isValid())     return false;

  else if (_major > other._major)               return true;
  else if (_major < other._major)               return false;

  else if (_minor > other._minor)               return true;
  else if (_minor < other._minor)               return false;

  else if (_point > other._point)               return true;
  else if (_point < other._point)               return false;

  else if (_stage > other._stage)               return true;
  else if (_stage < other._stage)               return false;

  else return (_substage > other._substage);
}

bool XVersion::operator>=(XVersion other)
{
  if (! isValid() || ! other.isValid())
    return false;
  else
    return (*this == other || *this > other);
}

bool XVersion::operator< (XVersion other)
{
  if (! isValid() || ! other.isValid())
    return false;
  else
    return !(*this == other || *this > other);
}

bool XVersion::operator<=(XVersion other)
{
  if (! isValid() || ! other.isValid())
    return false;
  else
    return (*this == other || *this < other);
}

bool XVersion::operator!=(XVersion other)
{
  if (! isValid() || ! other.isValid())
    return false;
  else
    return ! (*this == other);
}
