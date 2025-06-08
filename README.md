# SysMon - A Multithreaded Linux System Monitor in C++

SysMon is a command-line (CLI) system monitoring and alerting tool for Linux, developed using modern C++ (C++17). This project demonstrates advanced C++ concepts, including multithreading, object-oriented design, and direct interaction with the Linux `/proc` filesystem.

## Features

* **Real-time Metric Monitoring:** Concurrently tracks key system metrics such as CPU usage, memory consumption, and disk I/O rates.
* **Multithreaded Architecture:** Each core function (metric collection, rule evaluation, UI rendering) runs in a separate thread for a responsive, non-blocking experience.
* **Rule-Based Alerting:** Generates alerts based on user-defined thresholds in an external configuration file.
* **Interactive CLI:** Features a continuously updating dashboard and accepts user commands (`exit`, `clear`) for interaction.
* **External Configuration:** System parameters like alert thresholds can be configured via a `monitor.conf` file without recompiling.

## Tech Stack & Core Concepts

* **Language:** C++17
* **Build System:** CMake
* **Platform:** Linux
* **Key Concepts Demonstrated:**
    * **Concurrency:** `std::thread`, `std::mutex`, `std::condition_variable`, `std::atomic`
    * **Object-Oriented Design:** Polymorphism, Abstraction, RAII
    * **Modern C++:** Smart Pointers (`std::unique_ptr`), STL containers, `std::variant`, `std::optional`, `std::chrono`
    * **System Interaction:** Parsing data from the `/proc` virtual filesystem.

## Build and Run

### Prerequisites

* A C++17 compatible compiler (e.g., `g++`)
* `CMake`
* `git`

On Debian/Ubuntu, install with:
```bash
sudo apt update
sudo apt install build-essential cmake git
```

## Compilation
1. Clone the repository:

```bash
git clone https://github.com/your-username/SysMon.git
cd SysMon
```

2. Build the project:

```bash
mkdir build && cd build
cmake ..
make
```

An executable named linux_monitor will be created in the build directory.

## Usage
1. (Optional) Create a configuration file:
In the build directory, create a monitor.conf file to set custom alert thresholds.

Example `monitor.conf`:

```ini
# Alert thresholds
CPU_THRESHOLD = 80.0
MEMORY_THRESHOLD = 75.0
DISK_IO_THRESHOLD = 5120.0 # in KB/s
```

2. Run the application:
From the build directory, execute:

```bash
./linux_monitor
```

3. Available Commands:

* `clear`: Clears active alerts from the screen.

* `exit` or `quit`: Shuts down the application gracefully.

* `Ctrl+C`: Sends a `SIGINT` to safely terminate the application.

## License
This project is licensed under the MIT License. See the `LICENSE` file for details.

