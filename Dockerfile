# # Use the official Ubuntu image as the base image
# FROM alpine:3.20
#
# # Expose the port on which the API will listen
# # EXPOSE 8080
#
# # Install necessary dependencies
# RUN apk upgrade
#
# RUN apk add --no-cache \
#     bash \
#     # zsh \
#     make \
#     git \
#     clang \
#     mingw-w64-gcc \
#     python3
#
# ENV DISPLAY=:0
#
# # Install emscripten
# RUN if [ ! -d "/usr/lib/emsdk" ]; then git clone https://github.com/emscripten-core/emsdk.git /usr/lib/emsdk; fi
#
# # Update emsdk
# WORKDIR /usr/lib/emsdk 
#
# RUN git pull 
# RUN ./emsdk install latest 
# RUN ./emsdk activate latest
# RUN source ./emsdk_env.sh
# RUN echo "source /usr/lib/emsdk/emsdk_env.sh" > ~/.bashrc && echo "clear" >> ~/.bashrc
#
# WORKDIR /
#
# # Install raylib
# RUN if [ ! -d "/usr/lib/raylib" ]; then git clone https://github.com/raysan5/raylib.git /usr/lib/raylib; fi 
#
# WORKDIR /usr/lib/raylib/src
#
# # Build raylib libs: debug/release versions of desktop/web
# RUN mkdir -p lib/web/debug 
# # RUN make PLATFORM=PLATFORM_WEB RAYLIB_BUILD_MODE=DEBUG -B  
# # RUN cp libraylib.a lib/web/debug
# #
# # RUN mkdir -p lib/web/release 
# # RUN make PLATFORM=PLATFORM_WEB RAYLIB_BUILD_MODE=RELEASE -B 
# # RUN cp libraylib.a lib/web/release
#
# RUN mkdir -p lib/desktop/debug 
# RUN make PLATFORM=PLATFORM_DESKTOP RAYLIB_BUILD_MODE=DEBUG -B 
# RUN cp libraylib.a lib/desktop/debug
#
# RUN mkdir -p lib/desktop/release 
# RUN make PLATFORM=PLATFORM_DESKTOP RAYLIB_BUILD_MODE=RELEASE -B 
# RUN cp libraylib.a lib/desktop/release
#
# # WORKDIR /
#
# #Add project
# RUN mkdir -p /projects/roguelike
#
# # Set the working directory in the container
# WORKDIR /projects/roguelike
#
# # Copy the source code into the container
# COPY . .
#
# # # Compile the C++ code
# # RUN make init
#
# # Command to run the API when the container starts
# CMD ["/bin/bash"]
