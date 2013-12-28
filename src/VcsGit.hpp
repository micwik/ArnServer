// Copyright (C) 2010-2013 Michael Wiklund.
// All rights reserved.
// Contact: arnlib@wiklunden.se
//
// This file is part of the ArnServer - Active Registry Network Server.
// Parts of ArnServer depend on Qt 4 and/or other libraries that have their own
// licenses. ArnServer is independent of these licenses; however, use of these other
// libraries is subject to their respective license agreements.
//
// GNU Lesser General Public License Usage
// This file may be used under the terms of the GNU Lesser General Public
// License version 2.1 as published by the Free Software Foundation and
// appearing in the file LICENSE.LGPL included in the packaging of this file.
// In addition, as a special exception, you may use the rights described
// in the Nokia Qt LGPL Exception version 1.1, included in the file
// LGPL_EXCEPTION.txt in this package.
//
// GNU General Public License Usage
// Alternatively, this file may be used under the terms of the GNU General
// Public License version 3.0 as published by the Free Software Foundation
// and appearing in the file LICENSE.GPL included in the packaging of this file.
//
// Other Usage
// Alternatively, this file may be used in accordance with the terms and
// conditions contained in a signed written agreement between you and Michael Wiklund.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//

//  Git setup
//  git init
//  git config --global user.name "Arn Magnusson"
//  git config --global user.email "arn.magnusson@arnnas.se"
//

#ifndef VCSGIT_HPP
#define VCSGIT_HPP

#include <ArnInc/ArnPersist.hpp>
#include <ArnInc/ArnDefs.hpp>
#include <QProcess>


class GitProc : public QProcess
{
    Q_OBJECT
    QString  _gitExePath;
    QString  _workingDir;

public:
    GitProc( QObject* parent);

    void  setGitExePath( const QString& path)  {_gitExePath = path;}
    QString  gitExePath()  {return _gitExePath;}
    void  setWorkingDir( const QString& dir)  {_workingDir = dir;}
    QString  workingDir()  {return _workingDir;}

    QByteArray runGit( QStringList arguments, bool block = true);
};


class VcsGit : public ArnVcs
{
    Q_OBJECT
    GitProc*  _gitProc;

public:
    explicit  VcsGit(QObject* parent = 0);
    
    void  setGitExePath( const QString& path)  {_gitProc->setGitExePath( path);}
    void  setWorkingDir( const QString& dir)   {_gitProc->setWorkingDir( dir);}

signals:

public slots:
    virtual void  log( int numLog);
    virtual void  files( QString ref);
    virtual void  commit( QString commitMsg, QStringList files, QString name, QString email);
    virtual void  tag( QString name, QString ref);
    virtual void  diff( QString ref, QStringList files);
    virtual void  logRecord( QString ref);
    virtual void  tags();
    virtual void  branches();
    virtual void  status();
    virtual void  userSettings();
    virtual void  setUserSettings( QString name, QString email);
    virtual void  checkout( QString ref, QStringList files);
    //void getHelp(QString command);
    //void getCommands();

private slots:

private:
    void  addFiles( QStringList relFiles);
    void  convertFileList( QStringList& files, Arn::NameF nameF);
};

#endif // VCSGIT_HPP
