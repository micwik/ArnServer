// Copyright (C) 2010-2016 Michael Wiklund.
// All rights reserved.
// Contact: arnlib@wiklunden.se
//
// This file is part of the ArnLib - Active Registry Network.
// Parts of ArnLib depend on Qt and/or other libraries that have their own
// licenses. Usage of these other libraries is subject to their respective
// license agreements.
//
// GNU General Public License Usage
// This file may be used under the terms of the GNU General Public
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

#include <QDir>
#include <QDebug>
#if QT_VERSION >= 0x050000
#  include <QtGui/QGuiApplication>
#else
#  include <QtGui/QApplication>
#endif
#include "ServerMain.hpp"

extern const QString  serverVersion = "3.0";


int main(int argc, char *argv[])
{
#if QT_VERSION >= 0x050000
    QGuiApplication a(argc, argv);
#else
    QApplication a(argc, argv, false);
#endif

    qDebug() << "Starting Arn Server ver: " + serverVersion;
    qDebug() << "CWD=" << QDir::currentPath();
    ServerMain sm;

#ifndef WIN32
    LinuxSignal::setupLinuxSignalHandlers();
#endif
    int  stat = a.exec();
    qDebug() << "Ended Arn Server ...";
    return stat;
}
