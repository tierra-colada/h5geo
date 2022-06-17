About
=====

C++17 and python API to work with geo-data (seismic, wells, maps, other in process) 
based on HDF5. Aimed at geoscientists and developers.

General
-------

In geoscience there are different types of data and many file formats for storing these data. 
Also the data belong to some spatial reference system. 
In the same time the developper or scientist prefer not to use many libraries when working with data (i.e. reading, converting spatial reference system and units).
All we want is convenient API, fast execution and availability in python.
That's what **h5geo** is aimed at.

Being written in C++ it uses `pybind11 <https://github.com/pybind/pybind11>`_ to make python bindings,
`HDF5 <https://www.hdfgroup.org/solutions/hdf5/>`_ for storing data, 
`GDAL <https://gdal.org/>`_ for spatial reference conversion,
`units <https://github.com/LLNL/units>`_ for units conversion
and `Eigen <https://gitlab.com/libeigen/eigen>`_ as container for storing numerics in memory.

For now h5geo provides API to work with:

* seismic
* wells

  * deviations
  * logs

* maps
* points

Originally the project was started as part of a free and open source geophysical platform 
`Colada <https://github.com/tierra-colada/Colada>`_.

License
-------

`MIT <https://github.com/tierra-colada/h5geo/blob/main/LICENSE>`_.

.. toctree::
   :maxdepth: 2
