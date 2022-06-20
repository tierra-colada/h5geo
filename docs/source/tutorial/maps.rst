Tutorial: Maps
==============

Map is an object that is represented by a matrix with 
coordinates of **upper-left** matrix corner (origin ``X0``, ``Y0``), 
coordinates of **upper-right** matrix corner (point1 ``X1``, ``Y1``) and 
coordinates of **lower-left** matrix corner (point2 ``X2``, ``Y2``).
Thus only single ``Z`` value corresponds for each ``XY`` point.

Create Map
----------

Define map parameters for column-major Eigen matrix of size ``[20, 10]``
whose Z-values represent time (``ms``) and
the coordinates are given in ``millimeters``.

.. code:: c++

   #include <iostream>
   #include <h5geo/h5mapcontainer.h>
   #include <h5geo/h5map.h>


   int main(){
      std::string fileName = "maps.h5";
      H5MapCnt_ptr cnt(h5geo::createMapContainerByName(
         fileName, h5geo::CreationType::OPEN_OR_CREATE));
      if (!cnt){
         std::cout << "Unable to open or create map container" << std::endl;
         return -1;
      }

      MapParam p;
      p.nX = 10;
      p.nY = 20;
      p.X0 = 0;
      p.Y0 = 0;
      p.X1 = 100;
      p.Y1 = 0;
      p.X2 = 0;
      p.Y2 = 100;
      p.lengthUnits = "millimeter";
      p.dataUnits = "ms";
      p.xChunkSize = 5;
      p.yChunkSize = 5;
      p.spatialReference = "EPSG:8139";

      std::string mapName = "myMap";
      H5Map_ptr map(cnt->createMap(
         mapName, p, h5geo::CreationType::OPEN_OR_CREATE));
      if (!map){
         std::cout << "Unable to open or create map" << std::endl;
         return -1;
      }
      
      return 0;
   }

Write/Read data
---------------

Let's suppose that we have data in ``sec`` and we want to write it.
To be sure that **h5geo** is able to convert ``sec`` to ``ms`` we can use 
`the web-service <https://units.readthedocs.io/en/latest/_static/convert.html>`_.

.. code:: c++

   Eigen::MatrixXd m = Eigen::MatrixXd::Random(p.nY, p.nX);
   if (!map->writeData(m, "sec")){
      std::cout << "Unable to write data" << std::endl;
      return -1;
   }

Now we want to read the data in ``sec``:

.. code:: c++

   Eigen::MatrixXd M = map->getData("sec");
   if (M.size() < 1){
      std::cout << "Unable to read data" << std::endl;
      return -1;
   }

Working with attribute map
--------------------------

Let's suppose the created time-domain map has velocity attribute 
i.e. we have somehow sliced volume of velocities and kept the data.
Lets generate such attribute map first:

.. code:: c++

   MapParam p_attrMap = p;
   p_attrMap.dataUnits = "feet/s"

   std::string attrMapName = "myAttrMap";
   H5Map_ptr attrMap(cnt->createMap(
      attrMapName, p_attrMap, h5geo::CreationType::OPEN_OR_CREATE));
   if (!attrMap){
      std::cout << "Unable to open or create attribute map" << std::endl;
      return -1;
   }

   Eigen::MatrixXd v = Eigen::MatrixXd::Random(p_attrMap.nY, p_attrMap.nX);
   if (!attrMap->writeData(v, "km/ms")){
      std::cout << "Unable to write data" << std::endl;
      return -1;
   }

To add attribute map:

.. code:: c++

   // addAttributeMap returns std::optional<h5gt::Group> of created map
   if (!map->addAttributeMap(attrMap, "velocity").has_value()){
      std::cout << "Unable to add attribute map" << std::endl;
   }

Then we can open the attribute and work with it as with usual map:

.. code:: c++

   H5Map_ptr velocityMap(map->openAttributeMap("velocity"));
   if (!velocityMap){
      std::cout << "Unable to open attribute map" < std::endl;
   }

Finally to remove attribute map we can call the following method:

.. code:: c++

   if (!map->removeAttributeMap("velocity")){
      std::cout << "Unable to remove attribute map" << std::endl;
   }

.. note:: 

   Attribute map is simply HDF5 soft link within ``H5Map`` object.