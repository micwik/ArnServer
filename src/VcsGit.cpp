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

#include "VcsGit.hpp"

GitProc::GitProc( QObject* parent) :
    QProcess( parent)
{
}


QByteArray GitProc::runGit( QStringList arguments, bool block)
{
        QStringList args;
        args << "--no-pager";
        args << arguments;
        setWorkingDirectory( _workingDir);
        start( _gitExePath, args);

        if(block)
                waitForFinished(-1);
        return readAllStandardOutput();
}


VcsGit::VcsGit( QObject* parent) :
    ArnVcs( parent)
{
    _gitProc = new GitProc( this);

    // connect( _gitProc, SIGNAL(notify(QString)), this, SIGNAL(notify(QString)));
}


void  VcsGit::commit( QString commitMsg, QStringList files, QString name, QString email)
{
    convertFileList( files, ArnLink::NameF::Relative);  // Enforce relative paths

    emit notify("Adding files");
    // emit progress(0,"Adding files");

    if (files.isEmpty()) {
        addFiles( QStringList() << "-A" << ".");  // Add all files
    }
    else {
        addFiles( files);
    }

    emit notify("Committing files");
    // emit progress(50,"Commiting files");

    QStringList  args;
    args << "commit";
    if (!name.isEmpty() && !email.isEmpty()) {
        args << "--author";
        QString  auth = name + " <" + email + "> ";
        args << auth;
    }
    if (!commitMsg.isEmpty()) {
        args << "-m";
        args << commitMsg;
    } else {
        return;
    }
    _gitProc->runGit( args);
    emit commitR();

    emit notify("Ready Commit");
    // emit progress(100);
}


void  VcsGit::tag( QString name, QString ref)
{
    QStringList  args;
    emit notify("Tagging");
    // emit progress(0,"Tagging");
    args << "tag";
    args << name;
    if (!ref.isEmpty())
        args << ref;
    _gitProc->runGit( args);
    emit tagR();

    emit notify("Ready");
    // emit progress(100);
}


void  VcsGit::files( QString ref)
{
    QStringList  args;

    emit notify("Gathering files");
    // emit progress(0,"Gathering files");
    args << "ls-tree" << "-r" << "--name-only";
    if (ref.isEmpty())
        args << "master";
    else
        args << ref;

    QByteArray  files = _gitProc->runGit( args);
    // emit progress(50);

    QStringList  replyFiles = QString::fromUtf8(files).split('\n', QString::SkipEmptyParts);
    convertFileList( replyFiles, ArnLink::NameF::EmptyOk);

    emit filesR( replyFiles);

    emit notify("Ready");
    // emit progress(100);
}


void  VcsGit::tags()
{
    QStringList  args;

    emit notify("Gathering tags");
    // emit progress(0);
    args << "tag";

    QByteArray tags = _gitProc->runGit( args);
    emit progress(50,"Gathering tags");

    QStringList  tagList = QString::fromUtf8( tags).split('\n', QString::SkipEmptyParts);
    int  tagN = tagList.size();
    for (int i = 0; i < (tagN / 2); ++i)  // Reverse tagList
        tagList.swap( i, tagN - 1 - i);
    emit tagsR( tagList);

    emit notify("Ready");
    // emit progress(100);
}


void  VcsGit::branches()
{
    QStringList  args;

    emit notify("Gathering branches");
    // emit progress(0,"Gathering branches");
    args << "branch";

    QByteArray branches = _gitProc->runGit( args);
    // emit progress(50);

    emit branchesR( QString::fromUtf8( branches).split('\n', QString::SkipEmptyParts));

    emit notify("Ready");
    // emit progress(100);
}


void  VcsGit::status()
{
    QStringList  args;

    emit notify("Gathering files status");
    // emit progress(0,"Gathering files status");
    args << "status";

    QByteArray  statusTxt = _gitProc->runGit( args);
    // emit progress(50);

    emit statusR( QString::fromUtf8( statusTxt));

    emit notify("Ready");
    // emit progress(100);
}


void  VcsGit::checkout( QString ref, QStringList files)
{
    convertFileList( files, ArnLink::NameF::Relative);  // Enforce relative paths

    QStringList  args;

    emit notify("Checking out files");
    // emit progress(0,"Checking out files");
    args << "checkout";
    if (ref.isEmpty())
        args << "master";
    else
        args << ref;
    if (files.isEmpty())
        args << ".";
    else
        args << files;

    QByteArray  status= _gitProc->runGit(args);
    // emit progress(50);

    emit checkoutR();

    emit notify("Ready");
    // emit progress(100);
}


void   VcsGit::log( int numLog)
{
    QStringList  args;
    QString  argLogN;

    emit notify("Running git log");
    // emit progress(0,"Running git log");
    argLogN.sprintf("-%i", numLog);
    args << "log";
    args << argLogN;
    args << "--oneline";
    QByteArray  array = _gitProc->runGit( args);
    QStringList  logs = QString::fromUtf8( array).split('\n', QString::SkipEmptyParts);
    QStringList  refs;
    QStringList  msgs;
    foreach (QString log, logs) {
        int  s = log.indexOf(' ');
        refs << log.left(s);
        msgs << log.mid(s + 1);
    }


    emit logR( refs, msgs);

    emit notify("Ready git log");
    // emit progress(100, "Ready git log");
}


void  VcsGit::diff( QString ref, QStringList files)
{
    convertFileList( files, ArnLink::NameF::Relative);  // Enforce relative paths

    QStringList  args;

    emit notify("Getting diffs");
    // emit progress(0,"Getting diffs");
    args << "diff";
    if (ref.isEmpty())
        args << "master";
    else
        args << ref;
    if (files.isEmpty())
        args << ".";
    else
        args << files;

    QByteArray  diffTxt = _gitProc->runGit(args);

    emit diffR( QString::fromUtf8( diffTxt.constData()));

    emit notify("Ready diffs");
    // emit progress(100);
}


void  VcsGit::logRecord( QString ref)
{
    QStringList  args;

    emit notify("Getting log record");
    // emit progress(0,"Getting log record");
    args << "log";
    args << "-1";
    if (ref.isEmpty())
        args << "master";
    else
        args << ref;

    QByteArray  logRecTxt = _gitProc->runGit( args);

    emit logRecordR( QString::fromUtf8( logRecTxt.constData()));

    emit notify("Ready log record");
    // emit progress(100);
}


void  VcsGit::userSettings()
{
    QStringList  args;
    QStringList  args2;

    emit notify("Getting user settings");
    // emit progress(0,"Getting user settings");
    args << "config" << "--global" << "--get" << "user.name";
    QByteArray  name = _gitProc->runGit( args);

    args2 << "config" << "--global" << "--get" << "user.email";
    QByteArray  email = _gitProc->runGit( args2);

    emit userSettingsR(QString::fromUtf8( name), QString::fromUtf8( email));

    emit notify("Ready");
    // emit progress(100);
}


void  VcsGit::setUserSettings( QString name, QString email)
{
    QStringList  args;
    QStringList  args2;

    emit notify("Setting user settings");
    // emit progress(0,"Setting user settings");
    args << "config" << "--global" << "user.name" << name;
    _gitProc->runGit( args);

    args2 << "config" << "--global"  << "user.email" << email;
    _gitProc->runGit( args2);
    emit notify("Ready");
    // emit progress(100);
}


void  VcsGit::addFiles( QStringList relFiles)
{
    QStringList  args;
    args << "add";
    args << relFiles;
    _gitProc->runGit( args);
}


void  VcsGit::convertFileList( QStringList &files, ArnLink::NameF nameF)
{
    for (int i = 0; i < files.size(); ++i) {
        files[i] = ArnM::convertPath( files.at(i), nameF);
    }
}
