# Ubuntu-Basis mit Build-Tools
FROM ubuntu:22.04 AS base
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y \
    build-essential cmake git wget pkg-config xz-utils \
    libtool automake autoconf unzip ca-certificates && \
    rm -rf /var/lib/apt/lists/*

# Miyoo Mini (ARMv7)
FROM base AS miyoo
RUN wget -q --show-progress https://github.com/JustEnoughLinuxOS/toolchain-miyoo/releases/download/v1.0.0/miyoo-toolchain-armv7h.tar.gz -O /tmp/miyoo-toolchain.tar.gz && \
    mkdir -p /opt/miyoo && \
    tar -xzf /tmp/miyoo-toolchain.tar.gz -C /opt/miyoo --strip-components=1 && \
    rm /tmp/miyoo-toolchain.tar.gz
ENV PATH="/opt/miyoo/bin:$PATH" \
    CC="arm-linux-gnueabihf-gcc" \
    CXX="arm-linux-gnueabihf-g++" \
    STRIP="arm-linux-gnueabihf-strip"

# Trimui Smart Pro (ARMv8)
FROM base AS trimui
RUN wget -q --show-progress https://github.com/JustEnoughLinuxOS/toolchain-trimui/releases/download/v1.0.0/trimui-aarch64-toolchain.tar.xz -O /tmp/trimui-toolchain.tar.xz && \
    mkdir -p /opt/trimui && \
    tar -xf /tmp/trimui-toolchain.tar.xz -C /opt/trimui --strip-components=1 && \
    rm /tmp/trimui-toolchain.tar.xz
ENV PATH="/opt/trimui/bin:$PATH" \
    CC="aarch64-linux-gnu-gcc" \
    CXX="aarch64-linux-gnu-g++" \
    STRIP="aarch64-linux-gnu-strip"

# Trimui Brick (MIPS)
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

# Finales Image (Multi-Stage)
FROM base AS final
COPY --from=miyoo /opt/miyoo /opt/miyoo
COPY --from=trimui /opt/trimui /opt/trimui
COPY --from=brick /opt/brick /opt/brick
WORKDIR /build