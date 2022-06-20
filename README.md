# h5geo
[![][build-img]][build-status]
[![][docs-stable-img]][docs-stable-status]

**h5geo** provides API to work with geo-data that is based on [HDF5](https://www.hdfgroup.org/solutions/hdf5/) containers via [h5gt](https://travis-ci.com/tierra-colada/h5geo) header only library. For now **h5geo** provides API to work with:
* seismic data
* wells (deviations and logs)
* maps

**h5geo** also includes **h5geopy** project wich is a python wrapper. 

***h5geopy** uses [pybind11](https://github.com/pybind/pybind11) to make python bindings and there may be some perfomance penalty when copying matrices from C++ `Eigen3` matrices to `numpy` (or `std::vector` to `py::list`). I have not done any perfomance testing.*

***This project is aimed at geoscientist developers***

## Building h5geo with h5geopy support
**h5geo** depends on:
* [HDF5](https://github.com/HDFGroup/hdf5)
* [h5gt](https://github.com/tierra-colada/h5gt) (header only)
* [Eigen3](https://gitlab.com/libeigen/eigen) (header only)
* [magic-enum](https://github.com/Neargye/magic_enum) (header only)
* [units](https://github.com/LLNL/units)
* [mio](https://github.com/mandreyel/mio) (header only)
* optionally: [tbb](https://github.com/oneapi-src/oneTBB) (MSVC compiler includes it by default)
* optionally: [GDAL](https://github.com/OSGeo/gdal)
* optionally: [h5gtpy](https://github.com/tierra-colada/h5gt) (needed only when building with python support `-DH5GEO_BUILD_h5geopy=ON`)

You can use superbuild or build every dependency by yourself. Then do:
```cmake
mkdir build
cd build
cmake ..
  -DCMAKE_BUILD_TYPE=Release
  -DEigen3_ROOT:PATH=/path/to/eigen 
  -Dmagic_enum_ROOT:PATH=/path/to/magic_enum 
  -Dmio_ROOT:PATH=/path/to/mio
  -Dh5gt_ROOT:PATH=/path/to/h5gt 
  -DH5GEO_SUPERBUILD:BOOL=OFF
  -DH5GEO_USE_THREADS:BOOL=ON
  -DTBB_ROOT:PATH=/path/to/tbb
  -DH5GEO_BUILD_SHARED_LIBS:BOOL=ON
  -DH5GEO_BUILD_h5geopy:BOOL=ON
  -DHDF5_RUNTIME_DIR:PATH=/path/to/dir/containing/hdf5-runtime
  -DH5GEOPY_COPY_RUNTIME_DEPS:BOOL=ON
  -DPYTHON_EXECUTABLE=/path/to/python.exe 
  -DH5GEO_BUILD_TESTS:BOOL=OFF
  -DCMAKE_INSTALL_PREFIX:PATH=/install/prefix
  -G "some generator"
cmake --build . 
```
**NOTE:** instead of `<prefix>_ROOT` path (root directory of every package) you can pass `<prefix>_DIR` path containing some `<prefix>-config.cmake` file of every package.

**NOTE:** to run **h5geopy** python must know where HDF5-runtime, units-runtime, tbb-runtime and h5geo-runtime are located. You may choose whether to copy them to `site-packages/h5geopy` dir or add their dirs to `PATH` env. By default cmake try to search h5geo-runtime in `h5geo-install/lib` folder. Also **h5geopy** depends on **h5gtpy**. So you would need to have installed **h5gtpy**.

## Installation h5geo with h5geopy

`h5geo` installation is done the same way one usually installs cmake project:
```cmake
cmake --install . --prefix /some/path --config Release
```

Then you can find **h5geopy** in `site-packages` of your python env.

**NOTE:** cmake tries to find runtime dependencies at `install` step. If there are unresolved or conflict dependencies then you need to modify `PATH` env, rerun `cmake ..` and `cmake --install . --prefix /some/path --config Release`. **You don't have to rebuild the project!**

## Build & Install using `pip`
It is possible to build and install the library using the following command:
```
pip install git+https://github.com/tierra-colada/h5geo.git@v0.1.0 --verbose
```
This will automatically run **CMake Superbuild** to build build **h5geo v0.1.0** and its dependencies (without GDAL support) in the `site-pakages/h5geopy` dir. 

On modern PC this usually takes a couple of minutes.

**NOTE:** CMake and C++ compiler are needed.

## Usage
Importing is done via:
```python
from h5gtpy import h5gt
from h5geopy import h5geo
```

You can find tests in `site-packages/h5geopy`.
To run them:
```python
python -m unittest discover h5geopy.tests
```

Note: `h5geopy` (as well as `h5geo`) uses fortran memory order (columnwise). Thus you should allocate numpy with `numpy.asfortranarray` or `order='F'` as numpy uses `C` order by default.

## Integrating with VS Code
There is stub file `h5geopy/h5geo.pyi`. You need to set VS Code setting `"python.analysis.stubPath": "some/relative/path/to/stub-storage"`: where `stub-storage` is supposed to contain folder `h5geopy` with `h5geo.pyi` file.

I'm some kind of a uncomfortable to distribute stub file, so to be sure that stub file is updated you can generate your own stub file:
```shell
pip install pybind11-stubgen
pybind11-stubgen --no-setup-py h5geopy._h5geo
```

## Integrating with PyCharm
You don't need to provide stub file to PyCharm as it handles this by its own.

## Supported HDF5 versions
1.10, 1.12 (recommended)

## Supported platforms
Windows 10/11, Linux, OSX

[build-img]:https://img.shields.io/travis/com/tierra-colada/h5geo?style=plastic
[build-status]:https://app.travis-ci.com/tierra-colada/h5geo
[docs-stable-img]:https://img.shields.io/badge/docs-stable-blue.svg?style=plastic
[docs-stable-status]:https://h5geo.readthedocs.io/en/latest/?badge=latest
