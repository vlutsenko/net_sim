FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt update && apt install -y \
    git cmake build-essential \
    libpcre2-dev libssh-dev libssl-dev \
    libsystemd-dev \
    libcurl4-openssl-dev pkg-config tar \
    python3 python3-pip nano \
    ca-certificates && \
    apt clean && rm -rf /var/lib/apt/lists/*

WORKDIR /opt

RUN echo "root:root" | chpasswd

RUN git clone https://github.com/CESNET/libyang.git && \
    cd libyang && mkdir build && cd build && \
    cmake .. && make -j$(nproc) && make install && \
    cd ../.. && rm -rf libyang


RUN git clone https://github.com/CESNET/libnetconf2.git && \
    cd libnetconf2 && mkdir build && cd build && \
    cmake .. && make -j$(nproc) && make install && \
    cd ../.. && rm -rf libnetconf2


RUN git clone https://github.com/sysrepo/sysrepo.git && \
    cd sysrepo && mkdir build && cd build && \
    cmake .. && make -j$(nproc) && make install && \
    cd ../.. && rm -rf sysrepo
    
RUN ldconfig

RUN git clone https://github.com/CESNET/netopeer2.git && \
    cd netopeer2 && mkdir build && cd build && \
    cmake .. && make -j$(nproc) && make install && \
    cd ../.. && rm -rf netopeer2
    
COPY modules/ /opt/modules/
COPY fc_startup_config/ /opt/fc_startup_config/
COPY install_yang_models.py /opt/install_yang_models.py
COPY oper_provider.py /opt/oper_provider.py
COPY oper_provider.c /opt/oper_provider.c
COPY entrypoint.sh /opt/entrypoint.sh

RUN chmod +x /opt/entrypoint.sh
RUN pip3 install sysrepo libyang
RUN python3 /opt/install_yang_models.py
RUN chmod 644 /opt/sysrepo/build/repository/data/*
RUN chmod 644 /opt/sysrepo/build/repository/yang/*

# CMD ["./entrypoint.sh"]
CMD ["netopeer2-server", "-d"]
# ENTRYPOINT ["/opt/entrypoint.sh"]
# CMD []

