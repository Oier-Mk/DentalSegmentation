# Use a base image with the necessary tools for building C++ projects
FROM gcc:latest

# Set the working directory inside the container
WORKDIR /app

# Copy the CMakeLists.txt and your source code into the container
COPY CMakeLists.txt .
COPY main.cpp .
COPY scripts/ ./scripts/

# Install any libraries or dependencies your project needs
# For example, if you need to install libboost, you would add a line like:
# RUN apt-get update && apt-get install -y libboost-dev

# Create a build directory and run cmake to generate build files
RUN mkdir build && cd build && cmake ..

# Build your C++ project
RUN cd build && make

# Define the command to run your program
CMD ["./build/dental_seg"]

