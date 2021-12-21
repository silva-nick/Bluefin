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
CC=gcc CXX=g++ cmake ..
cmake --build build --target bluefin
```


**`Build test`**

```bash
mkdir build_test
cd build_test
cmake --build build --target test
```

_Then to run:_
```bash
...
```
