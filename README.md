    Copyright (C) 2010-2016 Michael Wiklund.
    All rights reserved.
    Contact: arnlib@wiklunden.se

# ArnServer - Active Registry Network Server.

This Qt and ArnLib based program implements a generic Arn Server.
When this opague server is used _as is_, all custom Arn Data Objects are created in the
connecting clients as well as all custom logic.
The server then only works as a central connection point and provides some general
services like _Persistance_.
<Br><Br>


## Installation and usage

Read [Install.md](Install.md) how to build, install and use.
This ArnServer depends on ArnLib 3.0 or later.
<Br><Br>


## Main features

* Based on QT, multiple platform and OS support.

* Arn Server central connection point. Opague without custom objects or logic.

* Persistent storage of objects in SQLight and in files.

* Version Control System (VCS) using _git_ for objects stored in files.

* Automatic archiving of SQLight DB each 24 hour.

* Linux signal (HUP, TERM, INT) handling.

* ZeroConf advertising of the server.

* Remote server, with sessions at "/Local/Sys/Server/Sessions/".

* Login system, with access keys in <configdir>/arnserver.secrets.

* Support nets not needing login.
