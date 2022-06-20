Build instructions
==================

Pre-requisites
--------------
* CMake
* compiler with C++17 support
* python (optional)

Dependencies
------------

* `HDF5 <https://www.hdfgroup.org/solutions/hdf5/>`_
* `units <https://github.com/LLNL/units>`_
* `h5gt <https://github.com/tierra-colada/h5gt>`_ (header only)
* `Eigen <https://gitlab.com/libeigen/eigen>`_ (header only)
* `magic_enum <https://github.com/Neargye/magic_enum>`_ (header only)
* `mio <https://github.com/tierra-colada/mio>`_ (header only)
* `TBB <https://github.com/oneapi-src/oneTBB>`_ (optional)
* `GDAL <https://github.com/OSGeo/gdal>`_ with `PROJ <https://github.com/OSGeo/PROJ>`_ support (optional)

Build using prebuilt dependencies
---------------------------------

.. code:: bash

   git clone https://github.com/tierra-colada/h5geo.git
   cd h5geo
   mkdir build
   cd build
   cmake ..
     # find package dirs
     -DEigen3_ROOT:PATH=/path/to/eigen 
     -Dmio_ROOT:PATH=/path/to/mio
     -DZLIB_ROOT:PATH=/path/to/zlib
     -DHDF5_ROOT:PATH=/path/to/hdf5
     -Dh5gt_ROOT:PATH=/path/to/h5gt 
     -Dmagic_enum_ROOT:PATH=/path/to/magic_enum 
     -Dunits_DIR:PATH=$/path/to/units/lib/cmake/units
     -DTBB_ROOT:PATH=/path/to/tbb
     -DGDAL_ROOT:PATH=/path/to/gdal
     -DPYTHON_EXECUTABLE=/path/to/python.exe 
     # Lib settings
     -DH5GEO_SUPERBUILD:BOOL=OFF
     -DH5GEO_USE_THREADS:BOOL=ON
     -DH5GEO_USE_GDAL:BOOL=ON
     -DH5GEO_BUILD_SHARED_LIBS:BOOL=ON
     -DH5GEO_BUILD_TESTS:BOOL=OFF
     -DH5GEO_BUILD_h5geopy:BOOL=ON
     # to 'import h5geopy' to python runtime deps should be resolved
     -DH5GEOPY_RESOLVE_RUNTIME_DEPS:BOOL=ON
     -DH5GEOPY_COPY_RUNTIME_DEPS:BOOL=OFF
     # path to runtimes (used at h5geopy installation step)
     -DHDF5_RUNTIME_DIRS:PATH=/path/to/hdf5/bin
     -DZLIB_RUNTIME_DIRS:PATH=/path/to/zlib/bin
     -DUNITS_RUNTIME_DIRS:PATH=/path/to/units/bin
     -DTBB_RUNTIME_DIRS:PATH=/path/to/tbb/bin
     -DGDAL_RUNTIME_DIRS:PATH=/path/to/tbb/bin
     -DH5GEO_RUNTIME_DIRS:PATH=/path/to/h5geo/bin
     # CMake settings
     -DCMAKE_BUILD_TYPE=Release
     -G "some generator"
   cmake --build . --config Release
   cmake --install . --prefix /path/to/h5geo-install

.. note::
   If you see that some of the dependencies were not resolved at installation step 
   then change ``<lib>_RUNTIME_DIRS``, rerun cmake & cmake install steps.

.. warning::
   ``H5GEOPY_COPY_RUNTIME_DEPS`` copies all the resolved runtimes to
   ``site-packages/h5geopy`` dir. There might be OS-specific runtimes that 
   nobody wants to copy. Thus this option is not recommended yet.

Build using superbuild
----------------------

.. code:: bash

   git clone https://github.com/tierra-colada/h5geo.git
   cd h5geo
   mkdir build
   cd build
   cmake ..
     -DCMAKE_INSTALL_PREFIX=/path/to/h5geo-install
     -DCMAKE_BUILD_TYPE=Release
     -DH5GEO_SUPERBUILD=ON
     -DH5GEO_BUILD_h5geopy=ON
     -DH5GEO_USE_THREADS=ON
     -DH5GEO_BUILD_SHARED_LIBS=ON
     -DH5GEO_USE_GDAL=ON
     -DH5GEO_BUILD_TESTS=OFF
     -DCOPY_H5GTPY_RUNTIME_DEPS=OFF
     -DRESOLVE_H5GTPY_RUNTIME_DEPS=ON
     -DH5GEOPY_COPY_RUNTIME_DEPS=OFF
     -DH5GEOPY_RESOLVE_RUNTIME_DEPS=ON
     -DPYTHON_EXECUTABLE=/path/to/python.exe 

.. note::
   ``h5gtpy`` and ``GDAL`` (``osgeo``) will be installed in ``site-packages`` as well.

Build using pip
---------------

.. code:: bash
   
   pip install git+https://github.com/tierra-colada/h5geo.git@<tag> --verbose

where ``<tag>`` is git-tag.

.. note::
   ``h5gtpy`` and ``GDAL`` (``osgeo``) will be installed in ``site-packages`` as well.

   ``h5geo`` dependencies will be installed in ``site-packages/h5geopy.<some_postfix>`` dir.

.. warning::
   No prebuilt wheels is supplied yet. ``pip install`` simply runs superbuild.
   Thus installation takes pretty much time. The ``verbose`` option helps you
   to see the build progress and catch errors if any.

Supported platforms
-------------------

Windows, Linux, MacOS

.. toctree::
   :maxdepth: 2
