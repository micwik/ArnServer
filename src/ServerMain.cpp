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

#include "ServerMain.hpp"
#include "qgetopt.h"
#include <ArnInc/Arn.hpp>
#include <QDir>
#include <QFile>
#include <QCoreApplication>
#include <QStringList>
#include <QDebug>

extern const QString  serverVersion;


ServerMain::ServerMain( QObject* parent) :
    QObject( parent)
{
    QDir  dataDir   = QDir::current();  // Default
    QDir  configDir = QDir::current();
    configDir.cd("config");
    QGetOpt gopt;

    QGetOpt::Option  optConfigDir( 'c', "configdir", 0, 1);
    gopt.addOption( optConfigDir);

    QGetOpt::Option  optDataDir( 'd', "datadir", 0, 1);
    gopt.addOption( optDataDir);

    QGetOpt::Option  optZeroConfGroups( 'g', "zeroconf-groups", 1);
    gopt.addOption( optZeroConfGroups);

    QGetOpt::Option  optZeroConfService( 's', "zeroconf-service", 1, 1);
    gopt.addOption( optZeroConfService);

    QGetOpt::Option  optLoginFreeNet( 'f', "login-freenets", 1);
    gopt.addOption( optLoginFreeNet);

    QGetOpt::Option  optRemoteOff( 'r', "remote-off");
    gopt.addOption( optRemoteOff);

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

    if (gopt.update( optConfigDir).isUsed()) {
        qDebug() << "ConfigDir: used=" << optConfigDir.isUsed() << " value=" << optConfigDir.value();
        configDir.setPath( optConfigDir.value());
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

    QStringList  noLoginNets;
    if (gopt.update( optLoginFreeNet).isUsed()) {
        qDebug() << "LoginFreeNet: used=" << optLoginFreeNet.isUsed() << " values=" << optLoginFreeNet.values();
        noLoginNets = optLoginFreeNet.values();
    }

    bool  useRemote = true;  // Default
    if (gopt.update( optRemoteOff).isUsed()) {
        qDebug() << "RemoteOff: Remote server feature not used";
        useRemote = false;
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
    _server->setDemandLogin( true);
    _server->setNoLoginNets( noLoginNets);
    qDebug() << "Config path=" << configDir.absolutePath();
    setupConfig( configDir);
    _server->start();

    //// Setuo discover remote advertise
    _discoverRemote = new ArnDiscoverRemote( this);
    _discoverRemote->setDefaultService("Arn Server");
    _discoverRemote->setInitialServiceTimeout(0);  // Don't expect initial delayed service name
    _discoverRemote->setService( zeroConfService);
    _discoverRemote->setGroups( noLoginNets);
    _discoverRemote->addCustomProperty("ServerVers", serverVersion);
    _discoverRemote->startUseServer( _server);

    //// Setuo server remote
    if (useRemote) {
        _serverRemote = new ArnServerRemote( this);
        _serverRemote->startUseServer( _server);
    }
    else {
        _serverRemote = 0;
    }
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
    if (_persist) {
        _persist->flush();
        qDebug() << "Pending persistent values flushed (saved)";
    }

    if (_discoverRemote)
        delete _discoverRemote;  // Must be deleted while still in the main eventloop

    QCoreApplication::quit();
}


void  ServerMain::doAboutToQuit()
{
    qDebug() << "About to Quit !!!";
}


void  ServerMain::setupConfig( const QDir& configDir)
{
    QFile  file( configDir.absoluteFilePath("arnserver.secrets"));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    while (!file.atEnd()) {
        QByteArray line = file.readLine().trimmed();
        if (line.startsWith("#"))  continue;

        QStringList  userPwAllow = QString::fromUtf8( line.constData(), line.size()).split(":");
        if (userPwAllow.size() != 3)  continue;

        Arn::Allow  allow;
        QStringList  allowList = userPwAllow.at(2).split(",");
        foreach (QString allowStr, allowList) {
            allowStr = allowStr.toLower().trimmed();
            if (allowStr == "read")    allow.set( allow.Read);
            if (allowStr == "write")   allow.set( allow.Write);
            if (allowStr == "create")  allow.set( allow.Create);
            if (allowStr == "delete")  allow.set( allow.Delete);
            if (allowStr == "mode")    allow.set( allow.ModeChange);
            if (allowStr == "all")     allow.set( allow.All);
        }

        // qDebug() << "Server add access: user=" << userPwAllow.at(0) << " pass=" << userPwAllow.at(1)
        //          << " allow=" << allow.toInt();
        _server->addAccess( userPwAllow.at(0), userPwAllow.at(1), allow);
    }
}


void  ServerMain::errorLog( QString errText)
{
    qDebug() << "MW-Err:" << errText;
}
