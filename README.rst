####
PyPP
####

========
Overview
========
|ci-badge|

C++ implementation of useful modules from the Python standard library.

In some cases, the Python API is modified to be more compatible with idiomatic
C++. For example, an iterable range is typically an object in Python but a
pair of iterators in C++.

Unlike Python, C++ does not have built-in support for Unicode, and none is
implied here beyond the ASCII subset.

Limitations
===========

At least C++11 and CMake v3.8 are required.

These modules are currently limited to POSIX platforms (including MacOS):
* ``os``
* ``path``
* ``tempfile``

========
Building
========

Library
=======
.. code-block:: console

    $ mkdir -p build/Debug && cd build/Debug
    $ cmake -DCMAKE_BUILD_TYPE=Debug ../
    $ cmake --build .
    

Documentation (Doxygen)
=======================
.. code-block:: console

    $ cd doc/doxygen
    $ doxygen Doxyfile
 
    
Documentation (Sphinx)
======================

.. code-block:: console

    $ cd doc/sphinx
    $ make html
    
Doxygen documentation can be integrated into Sphinx using the `Breathe`_
extension.


==========
Test Suite
==========

.. code-block:: console

    $ cd build/Debug
    $ test/test_pypp


.. |ci-badge| image:: https://github.com/mdklatt/pypp/actions/workflows/build.yml/badge.svg
   :alt: GitHub CI status
   :target: `github-ci`_
.. _github-ci: https://github.com/mdklatt/pypp/actions/workflows/build.yml
.. _Breathe: https://breathe.readthedocs.io/en/latest/
