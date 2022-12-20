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
#RUN wget https://github.com/emweb/wt/archive/4.8.2.tar.gz && \
#    tar xfv 4.8.2.tar.gz && \
#    cd wt-4.8.2 && \
#    mkdir build && \
#    cd build && \
#    cmake .. && \
#    make && \
#    make install && \
#    ln -s /wt-4.8.2/build/libwt.so /usr/lib #&& \
    #LD_LIBRARY_PATH=/usr/local/lib/ && \
    #LD_LIBRARY_PATH=$LD_LIBRARY_PATH && \
    #export LD_LIBRARY_PATH
