Wells
=====

Well is an object that is responsible for well head coordinates,
kelly bushing and manages deviations and log curves.

Create Well
-----------

We define well head coordinates, kelly bushing and uwi:

.. code:: c++

   #include <iostream>
   #include <h5geo/h5wellcontainer.h>
   #include <h5geo/h5well.h>
   #include <h5geo/h5devcurve.h>
   #include <h5geo/h5logcurve.h>


   int main(){
      std::string fileName = "wells.h5";
      H5WellCnt_ptr cnt(h5geo::createWellContainerByName(
         fileName, h5geo::CreationType::OPEN_OR_CREATE));
      if (!cnt){
         std::cout << "Unable to open or create well container" << std::endl;
         return -1;
      }

      H5WellParam p;
      p.headX = 444363;
      p.headY = 7425880;
      p.kb = 50.88;
      p.uwi = "my_uwi";
      p.lengthUnits = "meter";

      std::string wellName = "myWell";
      H5Well_ptr well(cnt->createWell(
         wellName, p, h5geo::CreationType::OPEN_OR_CREATE));
      if (!well){
         std::cout << "Unable to open or create well" << std::endl;
         return -1;
      }
      
      return 0;
   }


Open well by uwi
----------------

The straight forward way to open  well is by its name.
Another way is by uwi (slower but useful):

.. code:: c++

   H5Well_ptr wellByUwi(cnt->openWellByUWI("my_uwi"));
   if (!wellByUwi || !well->isEqual(wellByUwi.get())){
      std::cout << "Unable to open well by UWI" << std::endl;
      return -1;
   }

Create Dev Curve
----------------

Dev curve is responsible for trajectory storing and transformations
using minimum curvature method implemented in `h5deviation.h5 <https://github.com/tierra-colada/h5geo/blob/main/include/h5geo/private/h5deviation.h>`_.

.. code:: c++

   H5DevCurveParam p_devCurve;
   p_devCurve.lengthUnits = "meter";
   p_devCurve.temporalUnits = "millisecond";
   p_devCurve.angularUnits = "degree";

   std::string devCurveName = "myDevCurve";
   H5DevCurve_ptr devCurve(
      well->createDevCurve(
         devCurveName, p_devCurve, h5geo::CreationType::OPEN_OR_CREATE));
   if (!devCurve){
      std::cout << "Unable to open or create dev curve" << std::endl;
      return -1;
   }

Write/Read Dev Curve
--------------------

``H5DevCurve`` stores the following curves: ``MD,AZIM,INCL,DX,DY,TVD,OWT``.
All other curves are calculated based on these curves.
That is done to prevent calculation errors i.e.
everytime ``MD_AZIM_INCL`` is transformed to ``X_Y_TVD`` an error is accumulating.
The same concerns when doing that in backward order: ``X_Y_TVD`` to ``MD_AZIM_INCL``.

.. code:: c++
   
   Eigen::VectorXd dx(3), dy(3), tvd(3);
   dx << 0, 3, 5;
   dy << 0, 0.3, 0.5;
   tvd << 0, 1, 2;
   if (!devCurve->writeDX(dx, "m") || 
       !devCurve->writeDY(dy, "m") || 
       !devCurve->writeTVD(tvd, "m")){
      std::cout << "Unable to write DX, DY, TVD" << std::endl;
      return -1;
   }

   // update is needed to calculate MD, AZIM, INCL based on DX, DY, TVD
   devCurve->updateMdAzimIncl();

To get back data:

.. code:: c++

   Eigen::VectorXd tvdss_out = devCurve->getCurve("TVDSS", "km");
   if (tvdss_out.size() < 1){
      std::cout << "Unable to get TVDSS" << std::endl;
      return -1;
   }

``H5Well`` also provides API to work with active deviation.
To set current deviation to be active:

.. code:: c++

   if (!well->setActiveDevCurve(devCurve.get())){
      std::cout << "Unable to set active dev curve" << std::endl;
      return -1;
   }

   // or simply: devCurve->setActive();

To check is dev curve is active use ``H5DevCurve::isActive()``.

.. note:: 

   Active dev curve is simply soft link to the real dev curve within HDF5 file.

Create Log Curve
----------------

Log curve is represented by ``[N, 2]`` matrix.
The first column is ``MD`` and the second is ``VAL`` (value).

.. code:: c++

   H5LogCurveParam p_logCurve;
   p_logCurve.lengthUnits = "meter";
   p_logCurve.dataUnits = "kg/m2";

   std::string logCurveName = "myLogCurve";
   H5LogCurve_ptr logCurve(
      well->createLogCurve(
         logCurveName, p_logCurve, h5geo::CreationType::OPEN_OR_CREATE));
   if (!logCurve){
      std::cout << "Unable to open or create log curve" << std::endl;
      return -1;
   }

Write/Read Log Curve
--------------------

Write ``MD`` and ``VAL``:

.. code:: c++
   
   Eigen::VectorXd md(3), vals(3);
   md << 0, 3, 5;
   vals << 500, 700, 800;
   if (!logCurve->writeCurve(h5geo::LogDataType::MD, md) || 
       !logCurve->writeCurve(h5geo::LogDataType::VAL, vals)){
      std::cout << "Unable to write MD and VALS" << std::endl;
      return -1;
   }

And to read data:

.. code:: c++

   Eigen::VectorXd md_out = logCurve->getCurve(h5geo::LogDataType::MD, "cm");
   if (md_out.size() < 1){
      std::cout << "Unable to get MD" << std::endl;
      return -1;
   }

   Eigen::VectorXd vals_out = logCurve->getCurve("VAL", "g/cm2");
   if (vals_out.size() < 1){
      std::cout << "Unable to get VAL" << std::endl;
      return -1;
   }