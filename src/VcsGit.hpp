// Copyright (C) 2010-2016 Michael Wiklund.
// All rights reserved.
// Contact: arnlib@wiklunden.se
//
// This file is part of the ArnLib - Active Registry Network.
// Parts of ArnLib depend on Qt and/or other libraries that have their own
// licenses. Usage of these other libraries is subject to their respective
// license agreements.
//
// The MIT License (MIT) Usage
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this file to deal in its contained Software without restriction,
// including without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to permit
// persons to whom the Software is furnished to do so, subject to the
// following conditions:
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software in this file.
//
// GNU General Public License Usage
// Alternatively, this file may be used under the terms of the GNU General Public
// License version 3.0 as published by the Free Software Foundation and appearing
// in the file LICENSE_GPL.txt included in the packaging of this file.
//
// Other Usage
// Alternatively, this file may be used in accordance with the terms and conditions
// contained in a signed written agreement between you and Michael Wiklund.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
// PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.
//

//  Git setup
//  git init
//  git config --global user.name "Arn Magnusson"
//  git config --global user.email "arn.magnusson@arnnas.se"
//

#ifndef VCSGIT_HPP
#define VCSGIT_HPP

#include <ArnInc/ArnPersist.hpp>
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
