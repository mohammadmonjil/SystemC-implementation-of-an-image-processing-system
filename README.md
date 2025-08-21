# Median and Sobel Filtering in SystemC

This project demonstrates **image filtering using SystemC**, applying **median filtering** and **Sobel edge detection** on grayscale images. The implementation is structured into three modules — input, processing, and output — and leverages **OpenCV** for image operations.

The project was developed as part of **EEL 4930 – System-on-Chip Design**, Spring 2023.

---

## Project Summary

The design simulates a **hardware-like pipeline** for image filtering in SystemC:

- **Input Module**: Reads image and sends pixel data to the processing module through a 24-bit `sc_signal`.
- **Processing Module**: Applies either **median filtering** or **Sobel edge detection** using OpenCV functions.
- **Output Module**: Receives processed pixels, reconstructs the image, and saves the result.

Data synchronization across modules is achieved using handshake signals (`start`, `done`, `clk`), along with the 24-bit pixel channel.

---

## Modules

### 🔹 `inputx.h`

- Reads the image using OpenCV
- Sends each 24-bit pixel to the processing module via an `sc_signal`
- Synchronizes using `start` and `done` signals

### 🔹 `processx.h`

- Receives pixel data and reconstructs the full image as a `cv::Mat`
- Applies either:
  - **Median filter** (e.g., with kernel size = 7)
  - **Sobel edge detection**
- Converts the processed image back into pixel stream

### 🔹 `outputx.h`

- Receives the filtered image data
- Converts it into an OpenCV matrix
- Saves the final image as `processed.jpg`

---

## Sample Results

- **Sobel Edge Detection**: Highlights image edges effectively
- **Median Filtering**: Reduces noise while preserving important features

Example output images are shown in the project report.

---

## How to Build and Run

### Requirements

- **SystemC** library installed (`libsystemc`)
- **OpenCV** development libraries (`opencv_core`, `opencv_imgproc`, `opencv_imgcodecs`)

### Compilation

Make sure to provide paths to SystemC and OpenCV include and lib directories when compiling.

Example command:
g++ -I$SYSTEMC_HOME/include -L$SYSTEMC_HOME/lib-linux
-I/usr/include/opencv4 -lopencv_core -lopencv_imgcodecs -lopencv_imgproc
top.cpp -o median -lsystemc
### Run the Application
./median input.jpg

- The result will be saved as: `processed.jpg`

---

## Implementation Notes

- All pixel data is transferred through 24-bit integer channels (8 bits per channel).
- OpenCV handles filtering to simplify logic.
- Modules communicate using `sc_signal`-based handshake mechanism.
- The runtime is not fully optimized — the initial version sends the entire image before processing.

---

## Attempted Optimization

An experimental version was started using **FIFO buffering** in the processing module to reduce runtime by streaming data. However, debugging was not completed before submission.

---


