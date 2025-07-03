# Ubuntu-Basis mit Build-Tools
FROM ubuntu:22.04 AS base
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y \
    build-essential cmake git wget pkg-config xz-utils \
    libtool automake autoconf unzip ca-certificates && \
    rm -rf /var/lib/apt/lists/*

# Miyoo Mini (ARMv7) - Offizielle Toolchain
FROM base AS miyoo
RUN wget -q --show-progress https://github.com/MiyooMini/miyoo-toolchain/releases/download/v1.0/miyoo-toolchain-armv7h.tar.gz -O /tmp/miyoo-toolchain.tar.gz && \
    mkdir -p /opt/miyoo && \
    tar -xzf /tmp/miyoo-toolchain.tar.gz -C /opt/miyoo --strip-components=1 && \
    rm /tmp/miyoo-toolchain.tar.gz
ENV PATH="/opt/miyoo/bin:$PATH" \
    CC="arm-linux-gnueabihf-gcc" \
    CXX="arm-linux-gnueabihf-g++" \
    STRIP="arm-linux-gnueabihf-strip"

# Trimui Smart Pro (ARMv8) - Alternative Toolchain
FROM base AS trimui
RUN wget -q --show-progress https://toolchains.bootlin.com/downloads/releases/toolchains/aarch64/tarballs/aarch64--glibc--stable-2022.08-1.tar.bz2 -O /tmp/trimui-toolchain.tar.bz2 && \
    mkdir -p /opt/trimui && \
    tar -xjf /tmp/trimui-toolchain.tar.bz2 -C /opt/trimui --strip-components=1 && \
    rm /tmp/trimui-toolchain.tar.bz2
ENV PATH="/opt/trimui/bin:$PATH" \
    CC="aarch64-linux-gcc" \
    CXX="aarch64-linux-g++" \
    STRIP="aarch64-linux-strip"

# Trimui Brick (MIPS) - Musl Toolchain
FROM base AS brick
RUN wget -q --show-progress https://musl.cc/mipsel-linux-musl-cross.tgz -O /tmp/brick-toolchain.tar.gz && \
    mkdir -p /opt/brick && \
    tar -xzf /tmp/brick-toolchain.tar.gz -C /opt/brick --strip-components=1 && \
    mv /opt/brick/mipsel-linux-musl-cross/* /opt/brick/ && \
    rmdir /opt/brick/mipsel-linux-musl-cross && \
    rm /tmp/brick-toolchain.tar.gz
ENV PATH="/opt/brick/bin:$PATH" \
    CC="mipsel-linux-musl-gcc" \
    CXX="mipsel-linux-musl-g++" \
    STRIP="mipsel-linux-musl-strip"

# Finales Image
FROM base AS final
COPY --from=miyoo /opt/miyoo /opt/miyoo
COPY --from=trimui /opt/trimui /opt/trimui
COPY --from=brick /opt/brick /opt/brick
WORKDIR /build