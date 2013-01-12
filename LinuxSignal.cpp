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

#include "LinuxSignal.hpp"
#include <QSocketNotifier>
#include <QDebug>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>


int  LinuxSignal::sighupFd[2]  = {0, 0};
int  LinuxSignal::sigtermFd[2] = {0, 0};
int  LinuxSignal::sigintFd[2]  = {0, 0};


LinuxSignal::LinuxSignal( QObject* parent) :
    QObject( parent)
{
    if (::socketpair( AF_UNIX, SOCK_STREAM, 0, sighupFd))
        qFatal("Couldn't create HUP socketpair");

    if (::socketpair( AF_UNIX, SOCK_STREAM, 0, sigtermFd))
        qFatal("Couldn't create TERM socketpair");

    if (::socketpair( AF_UNIX, SOCK_STREAM, 0, sigintFd))
        qFatal("Couldn't create INT socketpair");

    snHup = new QSocketNotifier( sighupFd[1], QSocketNotifier::Read, this);
    connect( snHup, SIGNAL(activated(int)), this, SLOT(doSigHup()));
    snTerm = new QSocketNotifier( sigtermFd[1], QSocketNotifier::Read, this);
    connect( snTerm, SIGNAL(activated(int)), this, SLOT(doSigTerm()));
    snInt = new QSocketNotifier( sigintFd[1], QSocketNotifier::Read, this);
    connect( snInt, SIGNAL(activated(int)), this, SLOT(doSigInt()));
}


void  LinuxSignal::hupSignalHandler(int)
{
    qDebug() << "Linux-sig Hup";
    char a = 1;
    ::write( sighupFd[0], &a, sizeof(a));
}


void  LinuxSignal::termSignalHandler(int)
{
    qDebug() << "Linux-sig Term";
    char a = 1;
    ::write( sigtermFd[0], &a, sizeof(a));
}


void  LinuxSignal::intSignalHandler(int)
{
    qDebug() << "Linux-sig Int";
    char a = 1;
    ::write( sigintFd[0], &a, sizeof(a));
}


void  LinuxSignal::doSigHup()
{
    snHup->setEnabled(false);
    char tmp;
    ::read( sighupFd[1], &tmp, sizeof(tmp));

    emit sigHup();

    snHup->setEnabled(true);
}


void  LinuxSignal::doSigTerm()
{
    snTerm->setEnabled(false);
    char tmp;
    ::read( sigtermFd[1], &tmp, sizeof(tmp));

    emit sigTerm();

    snTerm->setEnabled(true);
}


void  LinuxSignal::doSigInt()
{
    snInt->setEnabled(false);
    char tmp;
    ::read( sigintFd[1], &tmp, sizeof(tmp));

    emit sigInt();

    snInt->setEnabled(true);
}


int  LinuxSignal::setupLinuxSignalHandlers()
{
    struct sigaction saHup;
    struct sigaction saTerm;
    struct sigaction saInt;

    saHup.sa_handler = LinuxSignal::hupSignalHandler;
    sigemptyset(&saHup.sa_mask);
    saHup.sa_flags = 0;
    saHup.sa_flags |= SA_RESTART;

    if (sigaction(SIGHUP, &saHup, 0) > 0)
       return 1;

    saTerm.sa_handler = LinuxSignal::termSignalHandler;
    sigemptyset(&saTerm.sa_mask);
    saTerm.sa_flags |= SA_RESTART;

    if (sigaction(SIGTERM, &saTerm, 0) > 0)
       return 2;

    saInt.sa_handler = LinuxSignal::intSignalHandler;
    sigemptyset(&saInt.sa_mask);
    saInt.sa_flags |= SA_RESTART;

    if (sigaction(SIGINT, &saInt, 0) > 0)
       return 3;

    return 0;
}

