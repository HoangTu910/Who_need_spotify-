FROM python:3.9-slim

# Still in development, unable to use recently

# Install system dependencies for building C++ components
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    g++ \
    && rm -rf /var/lib/apt/lists/*

# Set the working directory
WORKDIR /app

# Copy the requirements file and install Python dependencies
COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

# Copy the source code into the container
COPY src/ ./src/
COPY test/ ./test/
COPY inc/ ./inc/
COPY common/ ./common/
COPY scripts/ ./scripts/
COPY utils/ ./utils/
COPY CMakeLists.txt ./

ENTRYPOINT ["/bin/bash"]
# # Build the C++ components
# RUN mkdir build && cd build && cmake .. && make

# # Set the entry point for the container
# CMD ["python", "./src/python/plot_frequency_response.py"]