FROM debian:latest

RUN apt update
RUN apt install -y build-essential gdb cmake clang-format

USER root
WORKDIR /usr/src/bluefin/build/

CMD ["/bin/bash"]
