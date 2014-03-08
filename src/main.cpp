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

#include <QtGui/QApplication>
#include <QDir>
#include <QDebug>
#include "ServerMain.hpp"

extern const QString  serverVersion = "2.0";


int main(int argc, char *argv[])
{
    // QCoreApplication a(argc, argv);
    QApplication a(argc, argv, false);

    qDebug() << "Starting Arn Server ...";
    qDebug() << "CWD=" << QDir::currentPath();
    ServerMain sm;

#ifndef WIN32
    LinuxSignal::setupLinuxSignalHandlers();
#endif
    int  stat = a.exec();
    qDebug() << "Ended Arn Server ...";
    return stat;
}
