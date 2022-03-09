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

    # example of cmake args
    # config = 'Debug' if self.debug else 'Release'
    config = 'Release'
    import site
    install_dir = site.getsitepackages()[0] + '/h5geopy'
    cmake_args = [
        '-DCMAKE_INSTALL_PREFIX=' + install_dir,
        '-DCMAKE_BUILD_TYPE=' + config,
        '-DH5GEO_SUPERBUILD=ON',
        '-DH5GEO_BUILD_h5geopy=ON',
        '-DH5GEO_USE_THREADS=ON',
        '-DH5GEO_BUILD_SHARED_LIBS=ON',
        '-DH5GEO_USE_GDAL=OFF',
        '-DH5GEO_BUILD_TESTS=ON',
        '-DCOPY_H5GEOPY_RUNTIME_DEPS=OFF'
    ]

    # example of build args
    import multiprocessing
    build_args = [
        '--config', config,
        '--', '-j'+str(multiprocessing.cpu_count())
    ]

    # example of install args
    install_args = []

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


setuptools.setup(
    name='h5geopy',
    version='0.0.1',
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
    # include_package_data=True,   # important to copy MANIFEST.in files
    ext_modules=[CMakeExtension('.')],
    cmdclass={
      'build_ext': build_ext,
    }
)