docker build -t zios_build:latest .
docker run -ti --rm -v "%CD%\..\..:/src"  -v "%CD%\build\output:/deploy"  zios_build:latest
docker rmi zios_build:latest