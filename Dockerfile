FROM debian:latest

RUN apt update
RUN apt install -y g++
RUN apt install -y git vim gdb

CMD ["/bin/bash"]
