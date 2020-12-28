####
pypp
####

========
Overview
========

|travis.png|

C++ implementation of useful modules from the Python standard library.

The overarching design principle here is to emulate Python functionality as
closely as possible, which occasionally results in some compromises to the
ideal C++ approach to the problem. Also, user friendliness is prioritized over
performance.

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


.. |travis.png| image:: https://travis-ci.org/mdklatt/pypp.png?branch=main
   :alt: Travis CI build status
   :target: `travis`_
.. _travis: https://travis-ci.org/mdklatt/pypp
.. _Breathe: https://breathe.readthedocs.io/en/latest/
