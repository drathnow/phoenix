FROM ubuntu:18.04
WORKDIR /build

RUN apt-get update && apt-get install -y git python build-essential wget cpio unzip rsync bc nodejs libgtest-dev google-mock
RUN mkdir /root/.ssh && chmod 700 /root/.ssh

COPY arm-buildroot-linux-uclibcgnueabi_sdk-buildroot.tar.gz .
RUN cd /usr/local \
    && tar -xf /build/arm-buildroot-linux-uclibcgnueabi_sdk-buildroot.tar.gz \
    && cd arm-buildroot-linux-uclibcgnueabi_sdk-buildroot \
    && ./relocate-sdk.sh \
    && rm /build/arm-buildroot-linux-uclibcgnueabi_sdk-buildroot.tar.gz

RUN mkdir /root/.zdev
RUN printf "CROSS_COMPILER_DIR = /usr/local/arm-buildroot-linux-uclibcgnueabi_sdk-buildroot/bin\n\
CROSS_COMPILER_BIN_PREFIX = /usr/local/arm-buildroot-linux-uclibcgnueabi_sdk-buildroot/bin/arm-linux-\n\
ARM_INCLUDES = -I/usr/local/arm-buildroot-linux-uclibcgnueabi_sdk-buildroot/arm-buildroot-linux-uclibcgnueabi/sysroot/usr/include\n\
ARM_LIB_PATH = -L/usr/local/arm-buildroot-linux-uclibcgnueabi_sdk-buildroot/arm-buildroot-linux-uclibcgnueabi/sysroot/usr/lib\n" >> /root/.zdev/platform.mk

ARG PACKAGE_VERSION
RUN if [ "$PACKAGE_VERSION" = "" ]; then \
        printf "#!/bin/bash\ncd /src\nmake PLT=ReleaseARM PROD_BUILD= BUILD_NO=Final-1 package\n" > build_zios.sh; chmod +x build_zios.sh; \
    else \
        printf "#!/bin/bash\ncd /src\nmake PLT=ReleaseARM PROD_BUILD= BUILD_NO=$PACKAGE_VERSION package\n" > build_zios.sh; chmod +x build_zios.sh; \
    fi

ENTRYPOINT ["/bin/sh", "/build/build_zios.sh"]