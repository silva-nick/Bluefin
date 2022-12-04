FROM debian:latest

RUN apt update
RUN apt install -y build-essential gdb cmake clang-format lcov python libboost-all-dev

USER root
WORKDIR /usr/src/bluefin/

CMD ["/bin/bash"]
