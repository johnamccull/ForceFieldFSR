ARG CROSS_SDK_BASE_TAG=3.0.9-bookworm
ARG BASE_VERSION=3.0.8-bookworm
##
# Board architecture
# arm or arm64
##
ARG IMAGE_ARCH=

##
# Application Name
##
ARG APP_EXECUTABLE=ForceFieldFSR


# BUILD ------------------------------------------------------------------------
FROM torizon/debian-cross-toolchain-${IMAGE_ARCH}:${CROSS_SDK_BASE_TAG} As Build

ARG IMAGE_ARCH
ARG COMPILER_ARCH
ENV IMAGE_ARCH ${IMAGE_ARCH}

# __deps__
RUN apt-get -q -y update && \
    apt-get -q -y install \
    cmake \
# DO NOT REMOVE THIS LABEL: this is used for VS Code automation
    # __torizon_packages_dev_start__
	libgpiod-dev:arm64 \
	i2c-tools:arm64 \
	can-utils:arm64 \
	libgpiod2:arm64 \
	iproute2:arm64 \
	automake:arm64 \
	libtool:arm64 \
	valgrind:arm64 \
	autoconf:arm64 \
	wget:arm64 \
	unzip:arm64 \
	gpiod:arm64 \
	vim:arm64 \
	sudo:arm64 \
	libc6-dev:arm64 \
	libyaml-cpp-dev:arm64 \
	libssl-dev:arm64 \
	libssl-dev:arm64 \
	net-tools:arm64 \
	libsocketcan-dev:arm64 \
    # __torizon_packages_dev_end__
# DO NOT REMOVE THIS LABEL: this is used for VS Code automation
    && \
    apt-get clean && apt-get autoremove && \
    rm -rf /var/lib/apt/lists/*
# __deps__

COPY . /app
WORKDIR /app

RUN if [ "$IMAGE_ARCH" = "arm64" ] ; then \
        cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=aarch64-linux-gnu-g++ -DCMAKE_C_COMPILER=aarch64-linux-gnu-gcc -Bbuild ; \
    elif [ "$IMAGE_ARCH" = "arm" ] ; then \
        cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=arm-linux-gnueabihf-g++ -DCMAKE_C_COMPILER=arm-linux-gnueabihf-gcc -Bbuild ; \
    fi

RUN cmake --build build

# BUILD ------------------------------------------------------------------------


# DEPLOY ------------------------------------------------------------------------
FROM --platform=linux/${IMAGE_ARCH} torizon/debian:${BASE_VERSION} AS Deploy

ARG IMAGE_ARCH
ARG APP_EXECUTABLE
ENV APP_EXECUTABLE ${APP_EXECUTABLE}

RUN apt-get -y update && apt-get install -y --no-install-recommends \
# DO NOT REMOVE THIS LABEL: this is used for VS Code automation
    # __torizon_packages_prod_start__
	emacs-nox:arm64 \
	i2c-tools:arm64 \
	can-utils:arm64 \
	libgpiod2:arm64 \
	iproute2:arm64 \
	software-properties-common:arm64 \
	apt-utils:arm64 \
	build-essential:arm64 \
	automake:arm64 \
	libtool:arm64 \
	valgrind:arm64 \
	autoconf:arm64 \
	wget:arm64 \
	unzip:arm64 \
	git:arm64 \
	gpiod:arm64 \
	vim:arm64 \
	sudo:arm64 \
	net-tools:arm64 \
	libsocketcan-dev:arm64 \
    # __torizon_packages_prod_end__
# DO NOT REMOVE THIS LABEL: this is used for VS Code automation
	&& apt-get clean && apt-get autoremove && rm -rf /var/lib/apt/lists/*

# copy the build
COPY --from=Build /app/build/debug /app

# ADD YOUR ARGUMENTS HERE
CMD [ "./app/ForceFieldFSR" ]

# DEPLOY ------------------------------------------------------------------------
