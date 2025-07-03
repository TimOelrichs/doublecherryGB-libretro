# Basis-Image mit mehr Build-Tools
FROM ubuntu:22.04 AS base
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y \
    build-essential cmake git wget pkg-config xz-utils \
    libtool automake autoconf unzip && \
    rm -rf /var/lib/apt/lists/*

# Miyoo Mini (ARMv7)
FROM base AS miyoo
RUN wget -q --show-progress https://github.com/miyoomini/toolchain/releases/download/v1.0/miyoo-toolchain-armv7h.tar.gz -O /tmp/miyoo-toolchain.tar.gz && \
    mkdir -p /opt/miyoo && \
    tar -xzf /tmp/miyoo-toolchain.tar.gz -C /opt/miyoo --strip-components=1 && \
    rm /tmp/miyoo-toolchain.tar.gz
ENV PATH="/opt/miyoo/bin:$PATH" \
    CC="arm-linux-gnueabihf-gcc" \
    CXX="arm-linux-gnueabihf-g++"

# Trimui Smart Pro (ARMv8)
FROM base AS trimui
RUN wget -q --show-progress https://trimui.com/toolchain/trimui-aarch64-toolchain.tar.xz -O /tmp/trimui-toolchain.tar.xz && \
    mkdir -p /opt/trimui && \
    tar -xf /tmp/trimui-toolchain.tar.xz -C /opt/trimui --strip-components=1 && \
    rm /tmp/trimui-toolchain.tar.xz
ENV PATH="/opt/trimui/bin:$PATH" \
    CC="aarch64-linux-gnu-gcc" \
    CXX="aarch64-linux-gnu-g++"

# Trimui Brick (MIPS)
FROM base AS brick
RUN wget -q --show-progress https://github.com/trimui/trimui-brick-toolchain/releases/download/v1.0/mipsel-linux-musl.tar.xz -O /tmp/brick-toolchain.tar.xz && \
    mkdir -p /opt/brick && \
    tar -xf /tmp/brick-toolchain.tar.xz -C /opt/brick --strip-components=1 && \
    rm /tmp/brick-toolchain.tar.xz
ENV PATH="/opt/brick/bin:$PATH" \
    CC="mipsel-linux-musl-gcc" \
    CXX="mipsel-linux-musl-g++"

# Finales Image (Multi-Stage)
FROM base AS final
COPY --from=miyoo /opt/miyoo /opt/miyoo
COPY --from=trimui /opt/trimui /opt/trimui
COPY --from=brick /opt/brick /opt/brick
WORKDIR /build