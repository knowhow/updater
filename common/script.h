/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2010 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef __SCRIPT_H__
#define __SCRIPT_H__

#include <QString>
#include <QStringList>

class QDomDocument;
class QDomElement;

#define TR(a) QObject::tr(a)

class Script
{
  public:
    enum OnError {
      Default = 0,
      Stop,
      Prompt,
      Ignore
    };

    Script(const QString & name = QString::null, OnError onError = Default,
           const QString & comment = QString::null);
    Script(const QDomElement &, QStringList &msg, QList<bool> &fatal);

    virtual ~Script();

    virtual QDomElement createElement(QDomDocument &);

    virtual bool isValid() const { return !_name.isEmpty(); }

    virtual QString filename() const;

    virtual QString name() const { return _name; }
    virtual void setName(const QString & name) { _name = name; }

    virtual OnError onError() const { return _onError; }
    virtual void setOnError(OnError onError) { _onError = onError; }

    virtual QString comment() const { return _comment; }
    virtual void setComment(const QString & comment) { _comment = comment; }

    virtual int writeToDB(const QByteArray &data, const QString annotation, QString &errMsg);

    static QString onErrorToName(OnError);
    static OnError nameToOnError(const QString &);
    static QStringList onErrorList(bool includeDefault = TRUE);

  protected:
    QString _name;
    QString _comment;
    OnError _onError;
    static QString _sqlerrtxt;
};

#endif

