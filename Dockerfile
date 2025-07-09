# === Basis-Image mit Build-Tools ===
FROM ubuntu:22.04 AS base

RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y \
    build-essential cmake git wget pkg-config xz-utils \
    libtool automake autoconf unzip ca-certificates && \
    rm -rf /var/lib/apt/lists/*

# === Stage: Miyoo Mini Toolchain (ARMv7) ===
FROM base AS miyoo

RUN wget -q https://github.com/MiyooMini/miyoo-toolchain/releases/download/v1.0/gcc-arm-8.2-2018.08-x86_64-arm-linux-gnueabihf.tar.gz -O /tmp/miyoo-toolchain.tar.gz && \
    mkdir -p /opt/miyoo && \
    tar -xzf /tmp/miyoo-toolchain.tar.gz -C /opt/miyoo --strip-components=1 && \
    rm /tmp/miyoo-toolchain.tar.gz

ENV PATH="/opt/miyoo/bin:$PATH" \
    CC="arm-linux-gnueabihf-gcc" \
    CXX="arm-linux-gnueabihf-g++" \
    STRIP="arm-linux-gnueabihf-strip"

# === Stage: Trimui Smart Pro Toolchain (ARMv8) ===
FROM base AS trimui

RUN wget -q https://toolchains.bootlin.com/downloads/releases/toolchains/aarch64/tarballs/aarch64--glibc--stable-2022.08-1.tar.bz2 -O /tmp/trimui-toolchain.tar.bz2 && \
    mkdir -p /opt/trimui && \
    tar -xjf /tmp/trimui-toolchain.tar.bz2 -C /opt/trimui --strip-components=1 && \
    rm /tmp/trimui-toolchain.tar.bz2

ENV PATH="/opt/trimui/bin:$PATH" \
    CC="aarch64-linux-gcc" \
    CXX="aarch64-linux-g++" \
    STRIP="aarch64-linux-strip"

# === Stage: Trimui Brick Toolchain (MIPS, musl) ===
FROM base AS brick

RUN wget -q https://musl.cc/mipsel-linux-musl-cross.tgz -O /tmp/brick-toolchain.tar.gz && \
    mkdir -p /opt/brick && \
    tar -xzf /tmp/brick-toolchain.tar.gz -C /opt/brick --strip-components=1 && \
    mv /opt/brick/mipsel-linux-musl-cross/* /opt/brick/ && \
    rmdir /opt/brick/mipsel-linux-musl-cross && \
    rm /tmp/brick-toolchain.tar.gz

ENV PATH="/opt/brick/bin:$PATH" \
    CC="mipsel-linux-musl-gcc" \
    CXX="mipsel-linux-musl-g++" \
    STRIP="mipsel-linux-musl-strip"

# === Finale Stage: kombiniertes Build-Image mit allen Toolchains ===
FROM base AS final

COPY --from=miyoo /opt/miyoo /opt/miyoo
COPY --from=trimui /opt/trimui /opt/trimui
COPY --from=brick /opt/brick /opt/brick

# Kein COPY der toolchains/*.cmake nötig – die kommen vom Host
WORKDIR /build
