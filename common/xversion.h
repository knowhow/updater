/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2010 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef __XVERSION_H__
#define __XVERSION_H__

#include <QString>

class XVersion
{
  public:
    enum ReleaseStage { WIP, ALPHA, BETA, RC, FINAL, UNKNOWN };

    XVersion();
    XVersion(const QString &);
    XVersion(const XVersion &);

    ~XVersion();

    bool         isValid()                const;
    int          majorNumber(bool &ok)    const;
    int          minorNumber(bool &ok)    const;
    int          pointNumber(bool &ok)    const;
    void         setVersion(const QString &);
    ReleaseStage stage(bool &ok)          const;
    int          substageNumber(bool &ok) const;
    QString      toString()               const;

    bool operator==(XVersion);
    bool operator> (XVersion);
    bool operator>=(XVersion);
    bool operator< (XVersion);
    bool operator<=(XVersion);
    bool operator!=(XVersion);

  protected:
    int          _major;
    int          _minor;
    int          _point;
    ReleaseStage _stage;
    int          _substage;
};

#endif
