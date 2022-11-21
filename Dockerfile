FROM ubuntu:latest
RUN apt update -y && \
    apt install -y wget g++ make binutils cmake cppcheck clang-tidy python3-pip libc6-dbg cmake libgtest-dev lcov clang-tools vim
RUN apt install -y libboost-dev libboost-all-dev
RUN apt install -y clang-format
RUN apt install -y git curl
RUN pip install cpplint
RUN wget https://sourceware.org/pub/valgrind/valgrind-3.18.1.tar.bz2 && \
    tar xfv valgrind-3.18.1.tar.bz2 && \
    cd valgrind-3.18.1 && \
    ./autogen.sh && \
    ./configure && \
    make && \
    make install \
RUN