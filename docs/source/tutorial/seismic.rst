Seismic
=======

Seis is an object that is composed of several datasets and groups: 
traces are separated from trace headers.
Trace headers are kept on ``double`` format while trace data is ``float``.
Seis is designed to provide high perfomance while keeping simple API.

Create Seis
-----------

Seismic parameters includes: survey type (2D or 3D), 
data type (stack or prestack), number of traces, number of samples etc:

.. code:: c++

   #include <iostream>
   #include <h5geo/h5seiscontainer.h>
   #include <h5geo/h5seis.h>
   #include <h5geo/h5horizon.h>


   int main(){
      std::string fileName = "seis.h5";
      H5SeisCnt_ptr cnt(h5geo::createSeisContainerByName(
         fileName, h5geo::CreationType::OPEN_OR_CREATE));
      if (!cnt){
         std::cout << "Unable to open or create seis container" << std::endl;
         return -1;
      }

      H5SeisParam p;
      p.domain = h5geo::Domain::OWT;
      p.lengthUnits = "millimeter";
      p.temporalUnits = "millisecond";
      p.angularUnits = "degree";
      p.dataUnits = "psi";
      p.dataType = h5geo::SeisDataType::PRESTACK;
      p.surveyType = h5geo::SurveyType::TWO_D;
      p.nTrc = 30;
      p.nSamp = 10;
      p.srd = 20;
      p.spatialReference = "EPSG:8139";

      std::string seisName = "mySeis";
      H5Seis_ptr seis(cnt->createSeis(
         seisName, p, h5geo::CreationType::OPEN_OR_CREATE));
      if (!seis){
         std::cout << "Unable to open or create seis" << std::endl;
         return -1;
      }
      
      return 0;
   }

Write/Read data
---------------

By analogy with ``SEGY`` format ``H5Seis`` contains:
text header, binary header, traces headers, trace data.

Write/Read text header
^^^^^^^^^^^^^^^^^^^^^^

.. code:: c++

   std::vector<std::string> txtHdr;
   for (size_t i = 0; i < 40; i++)
      txtHdr.push_back("Bart Simpson");
   if (!seis->writeTextHeader(txtHdr)){
      std::cout << "Unable to write text header" << std::endl;
      return -1;
   }
   // another way is to write C-array: 'char txtHdr_c [40][80];'

.. note:: 

   Text header is a dataset of size ``[40, 80]``. 
   Thus everything outside of this range will be lost.

To read text header:

.. code:: c++

   std::vector<std::string> txtHdr_out =
      seis->getTextHeader();
   if (txtHdr_out.size() < 1){
      std::cout << "Unable to read text header" << std::endl;
      return -1;
   }

Write/Read binary header
^^^^^^^^^^^^^^^^^^^^^^^^

The simplest way to write binary header is:

.. code:: c++

   // convert 'seconds' to the temporal units of seis object
   if (!seis->writeBinHeader("SAMP_RATE", 0.002, "sec", seis->getTemporalUnits())){
      std::cout << "Unable to write samp rate" << std::endl;
      return -1;
   }

and to get it back:

.. code:: c++

   double sampRate = seis->getBinHeader("SAMP_RATE", seis->getTemporalUnits(), "ms");
   if (isnan(sampRate))
      std::cout << "Unable to get samp rate" << std::endl;
      return -1;
   }

.. note:: 

   List of binary header names is available through ``getBinHeaderNames`` 
   function declared in `h5core_util.h <https://github.com/tierra-colada/h5geo/blob/main/include/h5geo/private/h5core_util.h>`_.
   Header names are consistent to those used in ``SEGY`` viewer `SeiSee <https://mail.dmng.ru/freeware/>`_

Write/Read trace headers
^^^^^^^^^^^^^^^^^^^^^^^^

There are many functions to do this. Here is one of them:

.. code:: c++

   Eigen::MatrixXd cdp(3);
   cdp << 1, 2, 3;
   // write starting from 5th trace
   if (!seis->writeTraceHeader("CDP", cdp, 5)){
      std::cout << "Unable to write CDP trace header from 5th trace" << std::endl;
      return -1;
   }

and to get it back:

.. code:: c++

   // get 'cdp' trace header from 3 traces starting from 5th trace
   Eigen::MatrixXd cdp_out = seis->getTraceHeader("CDP", 5, 3);
   if (cdp_out.size() < 1){
      std::cout << "Unable to get CDP trace header">> std::endl;
      return -1;
   }

   // update trace header limits is needed when trace headers are written
   if (!seis->updateTraceHeaderLimits()){
      std::cout << "Unable to update trace header limits" << std::endl;
      return -1;
   }

.. note:: 

   List of trace header names is available through ``getTraceHeaderNames`` 
   function declared in `h5core_util.h <https://github.com/tierra-colada/h5geo/blob/main/include/h5geo/private/h5core_util.h>`_.
   Header names are consistent to those used in ``SEGY`` viewer `SeiSee <https://mail.dmng.ru/freeware/>`_

.. warning:: 

   Call ``updateTraceHeaderLimits`` everytime when trace header ``min/max`` values changed.

Write/Read trace data
^^^^^^^^^^^^^^^^^^^^^

Once again there are many functions to do this, here are some:

.. code:: c++

   Eigen::MatrixXd traces(p.nSamp, 3);
   Eigen::MatrixXf traces = Eigen::MatrixXf::Random(
      seis->getNSamp(), seis->getNTrc());
   // write starting from zero's trace
   if (!seis->writeTrace(traces, 0)){
      std::cout << "Unable to write traces" << std::endl;
      return -1;
   }

Get traces back:

.. code:: c++

   // from 3rd trace, 10 traces, from 2nd sample, 5 samples
   traces_out = seis->getTrace(3, 10, 2, 5);
   if (traces_out.size() < 1){
      std::cout << "Unable to get traces">> std::endl;
      return -1;
   }

.. note:: 

   ``write/get`` trace headers and trace data have pretty wide opportunities
   including trace selection and working with sorted data.
   Take a look at `seis.h <https://github.com/tierra-colada/h5geo/blob/main/include/h5geo/h5seis.h>`_ 
   to see all them.

Sorting
-------

The idea behind sorting is to prepare sorting by primary keys (PKey).
To accelerate the IO process the user need to add PKey sorting first
``addPKeySort`` and then use ``getSortedData`` function to retrieve
the data. 
No need to manually resort data, **h5geo** only keeps indexes
and unique values of prepared sortings. 
In theory this should make work with big data pretty effective.

For example there is widely used sorting ``CDP-OFFSET`` 
(``OFFSET`` is called ``DSREG`` in **h5geo**).
Add Pkey ``CDP`` and then you are free to retrieve any ``CDP-...`` sorted data.

.. code:: c++

   if (!seis->addPKeySort("CDP")){
      std::cout << "Unable to add CDP PKey" << std::endl;
      return -1;
   }

   // then you are allowed to use convenient 'getSortedData' function
   Eigen::MatrixXf trace_out;
   Eigen::MatrixXd trc_header_out;
   // from CDP 1 to 2, from DSREG 0 to 500
   // 'trc_ind' - contains indexes of selected traces
   Eigen::VectorX<size_t> trc_ind = seis->getSortedData(
      trace_out, trc_header_out, {"CDP", "DSREG"}, {1, 0}, {2, 500});

.. note:: 

   Use ``updatePKeySort`` when data was mixed.

   Sorting uses parallelization over the threads.

.. warning:: 

   Sorting idea is effetive only if the chosen PKey has many repeating values.

Calculating XY boundary around the survey
--------------------------------------

There is a convenient function to calculate XY boundary around survey.
For 3D and 2D prestack data it uses convex hull algorithm.
For 2D stack data it simply shows  ``CDP`` coordinates of traces.

.. code:: c++

   if (!seis->updateBoundary()){
      std::cout << "Unable to update boundary" << std::endl;
      return -1;
   }

   Eigen::MatrixXd boundary = seis->calcBoundary();
   if (!boundary.size() < 1){
      std::cout << "Unable to calculate boundary" << std::endl;
      return -1;
   }

   std::string horizonName = "boundary";
   H5HorizonParam p_hrz;
   p_hrz.components["X"] = 0;
   p_hrz.components["Y"] = 1;
   p_hrz.nPoints = 10;
   p_hrz.pointsChunkSize = 10;
   p_hrz.domain = h5geo::Domain::TWT;
   p_hrz.lengthUnits = p.lengthUnits;
   p_hrz.spatialReference = p.spatialReference;

   H5Horizon_ptr hrz(
      seis->createHorizon(
         horizonName, p_hrz, h5geo::CreationType::CREATE_OR_OVERWRITE));
  ASSERT_TRUE(hrz != nullptr);
   if (!hrz){
      std::cout << "Unable to create horizon (boundary)" << std::endl;
      return -1;
   }

   if (!hrz->writeComponent("X", boundary.col(0))){
      std::cout << "Unable to write X to boundary" << std::endl;
      return -1;
   }

   if (!hrz->writeComponent("Y", boundary.col(1))){
      std::cout << "Unable to write Y to boundary" << std::endl;
      return -1;
   }

To get calculated values:

.. code:: c++

   Eigen::VectorXd X = hrz->getComponent("X");
   if (X.size() < 1){
      std::cout << "Unable to get X from boundary" << std::endl;
      return -1;
   }

   Eigen::VectorXd Y = hrz->getComponent("Y");
   if (Y.size() < 1){
      std::cout << "Unable to get Y from boundary" << std::endl;
      return -1;
   }

Read SEGY
---------

Reading ``SEGY`` is pretty simple:

.. code:: c++

   if (!seis->readSEGYTextHeader("file.sgy")){
      std::cout << "Unable to read segy text header" << std::endl;
      return -1;
   }
   if (!seis->readSEGYBinHeader("file.sgy")){
      std::cout << "Unable to read segy binary header" << std::endl;
      return -1;
   }
   // SEGY files will be concatenated
   if (!seis->readSEGYTraces({"file1.sgy", "file2.sgy", "file3.sgy"})){
      std::cout << "Unable to read segy binary header" << std::endl;
      return -1;
   }

.. note:: 

   To read ``SEGY`` files **h5geo** uses memory-mapping technique and
   parallelization over the threads (OpenMP library). Thus it should work 
   pretty fast but there is a limitation with memory-mapping:
   the ``SEGY`` files should be on the PC's hard drive. 
   See more on `wiki <https://en.wikipedia.org/wiki/Memory-mapped_file#Drawbacks>`_.

Map SEGY
--------

The user may want not to spend time on reading ``SEGY`` file  but simply map it.
In **h5geo** you are allowed to do this at ``H5Seis`` creation time:

.. code:: c++

   H5SeisParam p_mapped = p;
   p_mapped.mapSEGY = true;
   p_mapped.segyFiles = {"file1.sgy", "file2.sgy", "file3.sgy"};

   std::string mappedSeisName = "seisMapped";
   H5Seis_ptr seisMapped(cnt->createSeis(
      mappedSeisName, p_mapped, h5geo::CreationType::OPEN_OR_CREATE));
   if (!seisMapped){
      std::cout << "Unable to open or create mapped seis" << std::endl;
      return -1;
   }

Then you are free to use it as with regular seis object but with some limitations:

* probably it is impossible to resize file
* data loss when writing to trace headers and binary header (``double`` is casted to ``int`` and ``short``)
* only ``SEGY ieee-32`` format are supported