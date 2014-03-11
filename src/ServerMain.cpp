// Copyright (C) 2010-2014 Michael Wiklund.
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

extern const QString  serverVersion;


ServerMain::ServerMain( QObject* parent) :
    QObject( parent)
{
    QDir dataDir = QDir::currentPath();  // Default
    QGetOpt gopt;

    QGetOpt::Option  optDataDir( 'd', "datadir", 0, 1);
    gopt.addOption( optDataDir);

    QGetOpt::Option  optZeroConfGroups( 'g', "zeroconf-groups", 1);
    gopt.addOption( optZeroConfGroups);

    QGetOpt::Option  optZeroConfService( 's', "zeroconf-service", 1, 1);
    gopt.addOption( optZeroConfService);

    gopt.parse();
    if (!gopt.check()) {
        const QGetOpt::Error*  optErr = gopt.lastError();
        qCritical() << QString( QCoreApplication::arguments().at(0) + ": " + optErr->errorMessage()
                       + " in --" + optErr->option()->longOption()
                       + " or -" + optErr->option()->shortOption()
                       ).toLatin1().data();
        qCritical() << "Usage options:";
        foreach( QGetOpt::Option opt, gopt.options()) {
            qCritical() << QString("--" + opt.longOption() + "  -" + opt.shortOption()).toLatin1().data();
        }
        exit(1);
    }

    bool  usePersist = true;  // Default
    if (gopt.update( optDataDir).isUsed()) {
        qDebug() << "DataDir: used=" << optDataDir.isUsed() << " value=" << optDataDir.value();
        usePersist = !optDataDir.value().isEmpty();
        dataDir.setPath( optDataDir.value());
    }

    QStringList  zeroConfGroups;
    if (gopt.update( optZeroConfGroups).isUsed()) {
        qDebug() << "ZeroConfGroups: used=" << optZeroConfGroups.isUsed() << " values=" << optZeroConfGroups.values();
        zeroConfGroups = optZeroConfGroups.values();
    }

    QString  zeroConfService;
    if (gopt.update( optZeroConfService).isUsed()) {
        qDebug() << "ZeroConfService: used=" << optZeroConfService.isUsed() << " values=" << optZeroConfService.values();
        zeroConfService = optZeroConfService.value();
    }

    //// Error log from Arn system
    ArnM::setConsoleError( false);
    connect( &ArnM::instance(), SIGNAL(errorLogSig(QString,uint,void*)),
             this, SLOT(errorLog(QString)));

    //// Handle abort signals
    QCoreApplication*  app = QCoreApplication::instance();
#ifndef WIN32
    _linuxSignal = new LinuxSignal( this);
    connect( _linuxSignal, SIGNAL(sigInt()),  this, SLOT(doShutDown()));
    connect( _linuxSignal, SIGNAL(sigTerm()), this, SLOT(doShutDown()));
#else
    _linuxSignal = 0;
#endif
    connect( app, SIGNAL(aboutToQuit()), this, SLOT(doAboutToQuit()));

    _server = new ArnServer( ArnServer::Type::NetSync, this);
    _server->start();

    //// Setuo discover remote advertise
    _discoverRemote = new ArnDiscoverRemote( this);
    _discoverRemote->setDefaultService("Arn Server");
    _discoverRemote->setInitialServiceTimeout(0);  // Don't expect initial delayed service name
    _discoverRemote->setService( zeroConfService);
    _discoverRemote->setGroups( zeroConfGroups);
    _discoverRemote->addCustomProperty("ServerVers", serverVersion);
    _discoverRemote->startUseServer( _server);

    //// Setup persistent
    if (usePersist) {
        qDebug() << "Persist filePath=" << dataDir.absoluteFilePath("persist");
        qDebug() << "Persist dbPath=" << dataDir.absoluteFilePath("persist.db");

        _persist = new ArnPersist( this);
        _persist->setupDataBase( dataDir.absoluteFilePath("persist.db"));
        _persist->setArchiveDir( dataDir.absoluteFilePath("archive"));
        _persist->setPersistDir( dataDir.absoluteFilePath("persist"));
        _persist->setMountPoint("/");
    }
    else
        _persist = 0;

    //// Setup version control system
#ifndef WIN32
    _git = new VcsGit( this);
    _git->setGitExePath("/usr/bin/git");
    // _git->setUserSettings("Arn Magnusson", "arn.magnusson@arnnas.se");
    _git->setWorkingDir(dataDir.absoluteFilePath("persist"));
#else
    _git = 0;
#endif

    if (usePersist) {
        _persist->setVcs( _git);
        _archiveTimer.start(24 * 3600 * 1000);
        connect( &_archiveTimer, SIGNAL(timeout()), _persist, SLOT(doArchive()));
    }
}


ServerMain::~ServerMain()
{
}


void ServerMain::doShutDown()
{
    if (_discoverRemote)  delete _discoverRemote;  // Must be deleted while still in the main eventloop

    QCoreApplication::quit();
}


void  ServerMain::doAboutToQuit()
{
    qDebug() << "About to Quit !!!";
}


void  ServerMain::errorLog( QString errText)
{
    qDebug() << "MW-Err:" << errText;
}
