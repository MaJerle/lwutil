.. _getting_started:

Getting started
===============

Getting started may be the most challenging part of every new library.
This guide is describing how to start with the library quickly and effectively

.. _download_library:

Download library
^^^^^^^^^^^^^^^^

Library is primarily hosted on `Github <https://github.com/MaJerle/lwutil>`_.

You can get it by:

* Downloading latest release from `releases area <https://github.com/MaJerle/lwutil/releases>`_ on Github
* Cloning ``main`` branch for latest stable version
* Cloning ``develop`` branch for latest development

Download from releases
**********************

All releases are available on Github `releases area <https://github.com/MaJerle/lwutil/releases>`_.

Clone from Github
*****************

First-time clone
""""""""""""""""

This is used when you do not have yet local copy on your machine.

* Make sure ``git`` is installed.
* Open console and navigate to path in the system to clone repository to. Use command ``cd your_path``
* Clone repository with one of available options below

  * Run ``git clone --recurse-submodules https://github.com/MaJerle/lwutil`` command to clone entire repository, including submodules
  * Run ``git clone --recurse-submodules --branch develop https://github.com/MaJerle/lwutil`` to clone `development` branch, including submodules
  * Run ``git clone --recurse-submodules --branch main https://github.com/MaJerle/lwutil`` to clone `latest stable` branch, including submodules

* Navigate to ``examples`` directory and run favourite example

Update cloned to latest version
"""""""""""""""""""""""""""""""

* Open console and navigate to path in the system where your repository is located. Use command ``cd your_path``
* Run ``git pull origin main`` command to get latest changes on ``main`` branch
* Run ``git pull origin develop`` command to get latest changes on ``develop`` branch
* Run ``git submodule update --init --remote`` to update submodules to latest version

.. note::
  This is preferred option to use when you want to evaluate library and run prepared examples.
  Repository consists of multiple submodules which can be automatically downloaded when cloning and pulling changes from root repository.

Add library to project
^^^^^^^^^^^^^^^^^^^^^^

At this point it is assumed that you have successfully download library, either with ``git clone`` command or with manual download from the library releases page.
Next step is to add the library to the project, by means of source files to compiler inputs and header files in search path.

*CMake* is the main supported build system. Package comes with the ``CMakeLists.txt`` and ``library.cmake`` files, both located in the ``lwutil`` directory:

* ``library.cmake``: It is a fully configured set of variables and with library definition. User can include this file to the project file with ``include(path/to/library.cmake)`` and then manually use the variables provided by the file, such as list of source files, include paths or necessary compiler definitions. It is up to the user to properly use the this file on its own.
* ``CMakeLists.txt``: It is a wrapper-only file and includes ``library.cmake`` file. It is used for when user wants to include the library to the main project by simply calling ``add_subdirectory`` **CMake** command, followed by ``target_link_libraries`` to add library as an external library.

.. tip::
    Open ``library.cmake`` and analyze the provided information. Among variables, you can also find list of all possible exposed libraries for the user.

If you do not use the *CMake*, you can do the following:

* Copy ``lwutil`` folder to your project, it contains library files
* Add ``lwutil/src/include`` folder to `include path` of your toolchain. This is where `C/C++` compiler can find the files during compilation process. Usually using ``-I`` flag
* Add source files from ``lwutil/src/`` folder to toolchain build. These files are built by `C/C++` compiler
* Build the project
