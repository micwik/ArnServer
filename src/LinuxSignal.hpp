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
