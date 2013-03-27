    Copyright (C) 2010-2013 Michael Wiklund.
    All rights reserved.
    Contact: arnlib@wiklunden.se

# ArnServer - Active Registry Network Server.

This Qt and ArnLib based program implements a generic Arn Server.
When this neutral server is used as is, all custom Arn Data Objects are created in the
connecting clients as well as all custom logic.
The server then only works as a central connection point and provides some general
services like _Persistance_.


## Main features

* Based on QT, multiple platform and OS support.

* Arn Server central connection point. Neutral without custom objects or logic.

* Persistent storage of objects in SQLight and in files.

* Version Control System (VCS) using _git_ for objects stored in files.

* Automatic archiving of SQLight DB each 24 hour.

* Linux signal (HUP, TERM, INT) handling.
