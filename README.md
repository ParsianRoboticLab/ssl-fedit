# ssl-fedit
Formation Editor For Small Size League.

(this project is a fork of [fedit2](http://rctools.osdn.jp/pukiwiki/index.php?fedit2) that developed by Hidehisa AKIYAMA under GNU GPL for Robocup Soccer Simulation 2D)

=============
Introduction
=============
SSL-Fedit is a formation editor for RoboCup Small size League
It is.

- RoboCup Small Size League Wiki: http://wiki.robocup.org/Small_Size_League

- RoboCup Official Homepage: http://www.robocup.org/

- Parsian Homepage: http://parsianrobotics.aut.ac.ir/

=============
Request library
=============
SSL-fedit requires the following libraries:
- [Qt4 Development Libraries](https://www.qt.io) version 4.8+
 
- [Boost development libraries](http://www.boost.org/) (needed by VarTypes)
 
- [librcsc - 4.0.x](http://rctools.osdn.jp/pukiwiki/index.php?librcsc)

Before you start installing ssl-fedit, install these libraries first
Please keep it.

==========
Installation
==========
Execute the following command inside the directory where you unpacked the package:

```bash
./configure
make
su
make install
```

This procedure generates the necessary binaries and they go to `/usr/local` or below
It will be installed. `/usr/local/bin/fedit2` is the executable file.

If you do not have librcsc installed in your system's directory,
Installing librcsc with `--with-librcsc` option of configure
You must specify a path.

```bash
./configure --with-librcsc=/path/to/librcsc/prefix
```

In this case, you may also need to set the environment variable LD_LIBRARY_PATH.

=================
Installation settings
=================
Before building SSL-fedit, the configure script included in the package
The setting can be changed by the option of.

By default, SSL-fedit is installed in the following location.

`/usr/local/bin executable file`

`/usr/local/share data`

In order to install ssl-fedit to the default position, you need administrator privileges
I will. The installation location depends on `-prefix` option of configure script
You can change it. For details on the options that can be specified in configure, see `configure`
You can reference it by executing `--help`.

=============
uninstall
=============
Run `make uninstall` on the package compiled directory
You can easily uninstall ssl-fedit with. This operation is installed
Delete all files, but the directories created during installation
It will not be deleted.

=======
how to use
=======
To execute ssl-fedit, execute the following command.

`fedit2`

However, in this case, the executable file exists in the directory contained in the environment variable PATH
You must be present.
