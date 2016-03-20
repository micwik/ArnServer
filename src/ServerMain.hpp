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

#ifndef SERVERMAIN_HPP
#define SERVERMAIN_HPP

#include "LinuxSignal.hpp"
#include "VcsGit.hpp"
#include <ArnInc/Arn.hpp>
#include <ArnInc/ArnDiscoverRemote.hpp>
#include <ArnInc/ArnPersist.hpp>
#include <ArnInc/ArnServer.hpp>
#include <ArnInc/ArnServerRemote.hpp>
#include <QTimer>
#include <QObject>

class QDir;


class ServerMain : public QObject
{
Q_OBJECT
public:
    explicit ServerMain( QObject* parent = 0);
    ~ServerMain();

signals:

public slots:

private slots:
    void  errorLog( QString errText);
    void  doShutDown();
    void  doAboutToQuit();

private:
    void  setupConfig( const QDir& configDir);

    ArnDiscoverRemote*  _discoverRemote;
    ArnServerRemote*  _serverRemote;
    // ArnDiscoverAdvertise*  _discoverRemote;
    ArnServer*  _server;
    ArnPersist*  _persist;
    VcsGit*  _git;
    LinuxSignal*  _linuxSignal;
    QTimer  _archiveTimer;
};

#endif // SERVERMAIN_HPP
