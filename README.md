# ssl-fedit
Formation Editor For Small Size League

=============
Introduction
=============
SSL-Fedit is a formation editor for RoboCup football simulation
It is.

- RoboCup Soccer Simulator Homepage: http://wiki.robocup.org/Small_Size_League
- RoboCup Official Homepage: http://www.robocup.org/

- Parsian Homepage: http://parsianrobotics.aut.ac.ir/

=============
Request library
=============
SSL-fedit requires the following libraries:
 - Qt-4.3 or later
 - boost - 1.32 or later
 - librcsc - 4.0.x

Before you start installing ssl-fedit, install these libraries first
Please keep it.

==========
Installation
==========
Execute the following command inside the directory where you unpacked the package:

./configure
make
Su
make install

This procedure generates the necessary binaries and they go to '/usr/local' or below
It will be installed. '/usr/local/bin/fedit2' is the executable file.

If you do not have librcsc installed in your system's directory,
Installing librcsc with '--with-librcsc' option of configure
You must specify a path.

./configure --with-librcsc=/path/to/librcsc/prefix

In this case, you may also need to set the environment variable LD_LIBRARY_PATH.

=================
Installation settings
=================
Before building SSL-fedit, the configure script included in the package
The setting can be changed by the option of.

By default, SSL-fedit is installed in the following location.

/usr/local/bin executable file

/Usr/local/share data

In order to install fedit 2 to the default position, you need administrator privileges
I will. The installation location depends on '- prefix' option of configure script
You can change it. For details on the options that can be specified in configure, see 'configure
You can reference it by executing '--help'.

=============
uninstall
=============
Run 'make uninstall' on the package compiled directory
You can easily uninstall fedit 2 with. This operation is installed
Delete all files, but the directories created during installation
It will not be deleted.

=======
how to use
=======
To execute fedit2, execute the following command.

`Fedit2 '

However, in this case, the executable file exists in the directory contained in the environment variable PATH
You must be present.
