import setuptools

# read the contents of your README file
import os
import pathlib
this_directory = pathlib.Path(__file__).parent
long_description = (this_directory / "README.md").read_text()

from setuptools.command.build_ext import build_ext as build_ext_orig


class CMakeExtension(setuptools.Extension):

  def __init__(self, name):
    # don't invoke the original build_ext for this special extension
    super().__init__(name, sources=[])


class build_ext(build_ext_orig):

  def run(self):
    for ext in self.extensions:
      self.build_cmake(ext)
    super().run()

  def build_cmake(self, ext):
    cwd = pathlib.Path().absolute()

    # these dirs will be created in build_py, so if you don't have
    # any python sources to bundle, the dirs will be missing
    build_temp = pathlib.Path(self.build_temp)
    build_temp.mkdir(parents=True, exist_ok=True)
    extdir = pathlib.Path(self.get_ext_fullpath(ext.name))
    extdir.mkdir(parents=True, exist_ok=True)
    install_dir = str(extdir.absolute())

    # cmake args
    # config = 'Debug' if self.debug else 'Release'
    config = 'Release'

    import sys
    python_exe = sys.executable
    cmake_args = [
      '-DCMAKE_INSTALL_PREFIX=' + install_dir,
      '-DCMAKE_BUILD_TYPE=' + config,
      '-DH5GEO_SUPERBUILD=ON',
      '-DH5GEO_BUILD_h5geopy=ON',
      '-DH5GEO_USE_THREADS=ON',
      '-DH5GEO_BUILD_SHARED_LIBS=ON',
      '-DH5GEO_USE_GDAL=ON',
      '-DH5GEO_BUILD_TESTS=OFF',
      '-DCOPY_H5GTPY_RUNTIME_DEPS=OFF',
      '-DRESOLVE_H5GTPY_RUNTIME_DEPS=ON',
      '-DCOPY_H5GEOPY_RUNTIME_DEPS=OFF',
      '-DRESOLVE_H5GEOPY_RUNTIME_DEPS=ON',
      '-DPYTHON_EXECUTABLE=' + python_exe
    ]

    # build args
    import multiprocessing
    nthreads = 4 if multiprocessing.cpu_count() > 4 else 1
    build_args = [
      '--config', config,
      '-j'+str(nthreads)
    ]

    # install args
    install_args = [
      '--config', config
    ]

    os.chdir(str(build_temp))
    self.spawn(['cmake', str(cwd)] + cmake_args)
    if not self.dry_run:
      self.spawn(['cmake', '--build', '.'] + build_args)
      self.spawn(['cmake', '--install', '.'] + install_args)
    # Troubleshooting: if fail on line above then delete all possible 
    # temporary CMake files including "CMakeCache.txt" in top level dir.
    os.chdir(str(cwd))


classifiers = [
  'Development Status :: 5 - Production/Stable',
  'Intended Audience :: Developers',
  'Intended Audience :: Science/Research',
  'License :: OSI Approved :: MIT',
  'Operating System :: OS Independent',
  'Programming Language :: Python :: 3',
  'Programming Language :: C',
  'Programming Language :: C++',
  'Topic :: Scientific/Engineering',
  'Topic :: Software Development :: Libraries :: Python Modules'
]

# When installing locally on Windows an error may arise:
# CUSTOMBUILD : error : unable to create file autotest/gdrivers... 2: Filename too long [...GDAL.vcxproj]
# To solve this try (https://stackoverflow.com/questions/22575662/filename-too-long-in-git-for-windows): 
# git config --system core.longpaths true
setuptools.setup(
  name='h5geopy',
  version='0.2.0',
  packages=setuptools.find_packages(),
  url='https://github.com/tierra-colada/h5geo',
  license='MIT',
  author='kerim khemrev',
  author_email='tierracolada@gmail.com',
  description='h5geo: API to work with geo-data (seismic, wells, maps, other in process) based on HDF5 and originally written in C++',
  long_description=long_description,
  long_description_content_type='text/markdown',
  classifiers=classifiers,
  install_requires=[
    'numpy',
  ],
  keywords='python cpp hdf5 seismic surfaces wells geo-data',
  python_requires='>=3',
  ext_modules=[CMakeExtension('h5geopy')],  # name of 'ext.name' dir where to build
  cmdclass={
    'build_ext': build_ext,
  }
)
