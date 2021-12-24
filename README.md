## Bluefin :fish:

#### A computer language

#### Building with Docker

**`Run docker container`**

```bash
docker compose up -d --build
docker exec -ti os bash
```

_Then to build._
```bash
# Configure the build
CC=gcc CXX=g++ cmake ..
cmake --build build --target bluefin
```

**`Debug`**
```bash
cmake -S . -B build/Debug -D CMAKE_BUILD_TYPE=Debug
cmake --build build/Debug --target test
```
