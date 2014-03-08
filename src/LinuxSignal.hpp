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

#ifndef LINUXSIGNAL_HPP
#define LINUXSIGNAL_HPP

#include <QObject>
#include <signal.h>

class QSocketNotifier;


class LinuxSignal : public QObject
{
    Q_OBJECT
public:
    explicit  LinuxSignal( QObject* parent = 0);

    static int  setupLinuxSignalHandlers();

    // Unix signal handlers.
    static void  hupSignalHandler( int unused);
    static void  termSignalHandler( int unused);
    static void  intSignalHandler( int unused);

signals:
    void  sigHup();
    void  sigTerm();
    void  sigInt();

public slots:

private slots:
    // Qt signal handlers.
    void  doSigHup();
    void  doSigTerm();
    void  doSigInt();

private:
    static int  sighupFd[2];
    static int  sigtermFd[2];
    static int  sigintFd[2];

    QSocketNotifier* snHup;
    QSocketNotifier* snTerm;
    QSocketNotifier* snInt;
};

#endif // LINUXSIGNAL_HPP
