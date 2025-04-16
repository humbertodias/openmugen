FROM ubuntu
ENV DEBIAN_FRONTEND=noninteractive

RUN apt update -y && apt install -y libsdl2-dev libsdl2-image-dev libsdl2-gfx-dev libsdl1.2-compat-dev build-essential cmake make