docker build -t zios_build:latest .
docker run -ti --rm -v "$PWD/../..:/src"  -v "$PWD/build/output:/deploy" zios_build:latest
docker rmi zios_build:latest