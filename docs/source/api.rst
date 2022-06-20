API & Basic Usage
=================

Description
-----------

**h5geo** follows interface strategy.
That means API is exposed through implementation classes (suffixed with ``Impl``).
Also there is no public constructors.
To instantiate a class one needs to use factory function.

The most basic class is ``H5Base``.
It can't be instantiated explicitely.

The classes ``H5BaseContainer`` and ``H5BaseObject`` are direct successors
of ``H5Base``. 
The main difference between them is that ``H5BaseContainer`` is built
around ``h5gt::File`` while ``H5BaseObject`` explores ``h5gt::Group``.
Thus container (HDF5 file) may store many geo-objects (HDF5 objects).

Next in the hierarchy follows more specific classes: 
``H5SeisContainer/H5Seis``, ``H5MapContainer/H5Map``, ``H5WellContainer/H5Well``,
``H5DevCurve``, ``H5LogCurve``, ``H5BasePoints``, ``H5Points1`` etc.

.. note::
   A designated container may store only appropriated geo-objects.
   In that sense you can't store ``H5Map`` in ``H5SeisContaner`` for example.
   But as you can see there is no container for points. 
   That means you are free to create and store points in any container.

Usage
-----

containers
^^^^^^^^^^

To create a container one should use one the following factory functions:

.. code:: c++

   H5BaseContainer* createContainer(
      h5gt::File h5File, h5geo::ContainerType cntType, h5geo::CreationType createFlag);

   H5BaseContainer* createContainerByName(
      std::string& fileName, h5geo::ContainerType cntType, h5geo::CreationType createFlag);

Using ``dynamic_cast<>`` one is able to cast returned container to 
the one defined with ``h5geo::ContainerType`` argument.

.. note::

   There are helper functions to create specific containers:
   ``h5geo::createMapContainer``, ``h5geo::createMapContainerByName``
   that implicitely cast returned type to ``H5Map`` 
   (most of geo-objects have such functions).

And to open a container:

.. code:: c++

   H5BaseContainer* openContainer(
      h5gt::File h5File);

   H5BaseContainer* openContainerByName(
      const std::string& fileName);

.. note::

   As when creating object you may want to use helper functions like:
   ``h5geo::openMapContainer``, ``h5geo::openMapContainerByName``
   that implicitely cast returned type to ``H5Map`` 
   (most of geo-objects have such functions).

geo-objects
^^^^^^^^^^^

All geo-objects are represented by ``h5gt::Group`` with assotiated and nested 
DataSets, Groups and Attributes.

To create a geo-object one must have an instance of an appropriate container
(or parent geo-object: for example ``H5DevCurve`` and ``H5LogCurve`` have ``H5Well`` as parent).
Thus to create ``H5Well`` one must have instance of ``H5WellContainer`` and use 
one of the following method:

.. code:: c++

   H5Well* H5WellContainer::createWell(
      std::string& name,
      WellParam& p,
      h5geo::CreationType createFlag)

   H5Well* H5WellContainer::createWell(
      h5gt::Group group,
      WellParam& p,
      h5geo::CreationType createFlag)

``WellParam`` inherites ``BaseObjectParam`` and defines parameters like:
spatial reference, length/temporal/angular/data units, null value, uwi, kelly bushing and 
well head coordinates.

.. note::

   Most geo-objects may be created/opened with name or ``h5gt::Group`` object.

To open a geo-object one may use parent object instance:

.. code:: c++

   H5Well* H5WellContainer::openWell(
      const std::string& name);
   H5Well* H5WellContainer::openWell(
      h5gt::Group group);

.. note:: 

   There are helper functions to open them without having parent object:
   ``h5geo::openWell``

   Or more generally (use it in pair with ``dynamic_cast<>``):
   ``h5geo::openObject``,
   ``h5geo::openObjectByName``

pointers
^^^^^^^^

Currently only unique pointers are provided. 
They are named in the following manner: ``H5WellCnt_ptr`` and ``H5Well_ptr``.

The preffered way to create objects:

.. code:: c++

   #include <h5geo/h5wellcontainer.h>
   #include <h5geo/h5well.h>
   #include <iostream>


   int main(){
      std::string fileName = "path/to/container";
      H5WellCnt_ptr wellCnt(h5geo::createWellContainerByName(
         fileName, h5geo::CreationType::OPEN_OR_CREATE));
      
      if (!wellCnt){
         std::cout << "Unable to open or create well container" << std::endl;
         return 1;
      }

      std::string wellName = "myWell";
      WellParam wellParam;
      wellParam.headX = 444363;
      wellParam.headY = 7425880;
      wellParam.kb = 50.88;
      wellParam.uwi = "my_uwi";
      wellParam.lengthUnits = "meter";
      H5Well_ptr well(wellCnt->createWell(
         wellName, wellParam, h5geo::CreationType::OPEN_OR_CREATE));
      
      return 0;
   }

units & spatial reference
^^^^^^^^^^^^^^^^^^^^^^^^^

units
"""""

All geo objects have spatial reference and length/temporal/angular/data units.
Not all them may be used by geo-object but the idea is:
*a geo-object must match to the units*.

That means if for example ``H5Well`` has length units ``meter`` then
all length units must be given in meters (header coordinates as well as kelly bushing for example).
The same also concerns temporal, angular and data units.

Data units is units of Z-axis of ``H5Map`` object for example. 
Or units of ``H5Seis`` traces (maybe ``psi`` in case of marine seismic).

Every geo-object provides API to automatically convert units.
For example when writing data to ``H5Map`` one is free to specify
the data units that the data currently is in:
``bool H5Map::writeData(Eigen::Ref<Eigen::MatrixXd> M, "m")``

The data will be converted from ``m`` to ``H5Map::getDataUnits``.

And one needs to get data in some units:
``Eigen::MatrixXd H5Map::getData("cm")``

Then the conversion is done in reverse order: 
from ``H5Map::getDataUnits`` to ``cm``.

.. note:: 
   No conversion is done if no units were specified.

Sometimes it is impossible to predict what units are going to be used.
For example when working with seismic trace headers the API provides
two arguments: ``unitsFrom`` and ``unitsTo``.
The conversion is done in direct order: ``unitsFrom`` will be converted to ``unitsTo``.

One can check if the units are convertable through `the web-service <https://units.readthedocs.io/en/latest/_static/convert.html>`_.

spatial reference
"""""""""""""""""

Spatial reference is given from ``PROJ-install/share/proj/proj.db``.
In ``projected_crs`` table find ``auth_name`` and ``code`` columns.
Usually the spatial reference is shaped as: ``auth_name:code``.

An example: ``EPSG:32056``.

Basically `OGRSpatialReference::SetFromUserInput <https://gdal.org/doxygen/classOGRSpatialReference.html#aec3c6a49533fe457ddc763d699ff8796>`_
function is used to create spatial reference object.

If you work with many objects that belong to different spatial reference
you may want to set a spatial reference for the session and pass ``doCoordTransform`` 
as ``true`` when writing/getting the data.
Take a look at `h5sr_settings.h <https://github.com/tierra-colada/h5geo/blob/main/include/h5geo/h5sr_settings.h>`_.
