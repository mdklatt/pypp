####
py++
####

.. |travis.png| image:: https://travis-ci.org/mdklatt/pypp.png?branch=master
   :alt: Travis CI build status
   :target: `travis`_
.. _travis: https://travis-ci.org/mdklatt/pypp
.. _py++: http://github.com/mdklatt/pypp


|travis.png|

Python functions you wish C++ had.


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
.. _Breathe: https://breathe.readthedocs.io/en/latest/

.. code-block:: console

    $ cd doc/sphinx
    $ make html
    
Doxygen documentation can be integrated into Sphinx using the* `Breathe`_
extension.


==========
Test Suite
==========
.. code-block:: console

    $ cd build/Debug
    $ test/test_pypp
