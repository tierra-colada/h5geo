call "venv\Scripts\activate.bat"

cd build-MSVC2019-Release

cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
cmake --install . --prefix ../install --config Release

cd ..

pybind11-stubgen --no-setup-py h5geopy._h5geo

if exist stubs\h5gtpy\_h5gt-stubs\h5geo.pyi (
	if exist stubs\h5gtpy\_h5gt-stubs\__init__.pyi (
		del "stubs\h5gtpy\_h5gt-stubs\h5geo.pyi"
		rename stubs\h5gtpy\_h5gt-stubs\__init__.pyi h5geo.pyi
	) else (
		echo ERROR: can't find "stubs\h5gtpy\_h5gt-stubs\__init__.pyi" file
	)
) else (
	if exist stubs\h5gtpy\_h5gt-stubs\__init__.pyi (
		rename stubs\h5gtpy\_h5gt-stubs\__init__.pyi h5geo.pyi
	) else (
		echo ERROR: can't find "stubs\h5gtpy\_h5gt-stubs\__init__.pyi" file
	)
)
