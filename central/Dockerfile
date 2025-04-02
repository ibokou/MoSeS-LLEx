FROM ubuntu:22.@sha256:58b87898e82351c6cf9cf5b9f3c20257bb9e2dcf33af051e12ce532d7f94e3fe
LABEL authors="Giorgos Marinou, Ibrahima Kourouma, Katja Mombaur"

ARG TARGET_ARCH
ENV TARGET_ARCH=$TARGET_ARCH

COPY scripts ./scripts
COPY multiarch ./multiarch

USER root

RUN apt-get update \
    && apt upgrade

RUN chmod +x ./multiarch/setup.sh && ./multiarch/setup.sh && \
    chmod +x ./scripts/install-pkgs.sh && ./scripts/install-pkgs.sh

RUN useradd -ms /bin/bash ubuntu
USER ubuntu
