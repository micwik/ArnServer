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

#include "ServerMain.hpp"
#include "qgetopt.h"
#include <QDir>
#include <QCoreApplication>
#include <QDebug>


ServerMain::ServerMain( QObject* parent) :
    QObject( parent)
{
    QDir dataDir = QDir::currentPath();  // Default
    QGetOpt gopt;

    QGetOpt::Option  optDataDir( 'd', "datadir",  1, 1);
    gopt.addOption( optDataDir);
    gopt.parse();
    if (!gopt.check()) {
        const QGetOpt::Error*  optErr = gopt.lastError();
        qCritical() << QString( QCoreApplication::arguments().at(0) + ": " + optErr->errorMessage()
                       + " in --" + optErr->option()->longOption()
                       + " or -" + optErr->option()->shortOption()
                       ).toAscii().data();
        qCritical() << "Usage options:";
        foreach( QGetOpt::Option opt, gopt.options()) {
            qCritical() << QString("--" + opt.longOption() + "  -" + opt.shortOption()).toAscii().data();
        }
        exit(1);
    }

    if (gopt.update( optDataDir).isUsed()) {
        qDebug() << "DataDir: used=" << optDataDir.isUsed() << " values=" << optDataDir.values();
        dataDir.setPath( optDataDir.value());
    }

    /// Error log from Arn system
    Arn::setConsoleError( false);
    connect( &Arn::getInstance(), SIGNAL(errorLogSig(QString,uint,void*)),
             this, SLOT(errorLog(QString)));

    _linuxSignal = new LinuxSignal( this);
    QCoreApplication*  app = QCoreApplication::instance();
    connect( _linuxSignal, SIGNAL(sigInt()),  app, SLOT(quit()));
    connect( _linuxSignal, SIGNAL(sigTerm()), app, SLOT(quit()));
    connect( app, SIGNAL(aboutToQuit()), this, SLOT(doAboutToQuit()));

    _server = new ArnServer( ArnServer::Type::NetSync, this);
    _server->start();

    _git = new VcsGit( this);
    _git->setGitExePath("/usr/bin/git");
    // _git->setUserSettings("Arn Magnusson", "arn.magnusson@arnnas.se");
    _git->setWorkingDir(dataDir.absoluteFilePath("persist"));
    qDebug() << "Persist filePath=" << dataDir.absoluteFilePath("persist");

    _persist = new ArnPersist( this);
    qDebug() << "Persist dbPath=" << dataDir.absoluteFilePath("persist.db");
    _persist->setupDataBase( dataDir.absoluteFilePath("persist.db"));
    _persist->setArchiveDir( dataDir.absoluteFilePath("archive"));
    _persist->setPersistDir( dataDir.absoluteFilePath("persist"));
    _persist->setMountPoint("/");
    _persist->setVcs( _git);

    _archiveTimer.start(24 * 3600 * 1000);
    connect( &_archiveTimer, SIGNAL(timeout()), _persist, SLOT(doArchive()));
}


void  ServerMain::doAboutToQuit()
{
    qDebug() << "About to Quit !!!";
}


void  ServerMain::errorLog( QString errText)
{
    qDebug() << "MW-Err:" << errText;
}
