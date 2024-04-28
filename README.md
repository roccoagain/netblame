# NetBlame

## Introduction

NetBlame is a real-time network traffic monitoring tool designed to capture and analyze IP packets over a network interface. This project assists network administrators and cybersecurity professionals by providing detailed insights into network traffic, aiding in the diagnosis and resolution of network issues, and enhancing security measures. NetBlame is developed as an open-source, lightweight solution that can be easily deployed on various Unix devices without heavy resource demands.

## Getting Started

### Installation

Before you begin, ensure you have a Unix-like operating system. You will also need GCC or Clang as your C compiler, along with the `make` utility, and the development libraries for `libpcap` and `ncurses`.

#### Installing Dependencies

On macOS, use `brew` to install the libraries:

```bash
brew install libpcap ncurses
```

For Debian-based systems (like Ubuntu), use `apt` to install the libraries:

```bash
sudo apt update
sudo apt install libpcap-dev libncurses-dev
```

For Red Hat-based systems (like Fedora or CentOS), use `dnf` (or `yum` on older versions) to install the libraries:

```bash
sudo dnf install libpcap-devel ncurses-devel
```

#### Building NetBlame

Clone the repository:

```bash
git clone https://github.com/roccoagain/netblame.git
```

Navigate to the project root:

```bash
cd netblame
```

Compile the source code using the provided makefile:

```bash
make
```

This will generate the executable `netblame` in the current directory.

### Running NetBlame

To monitor network traffic using NetBlame, run the executable with administrative privileges (necessary for packet capturing):

```bash
sudo ./netblame
```

To exit the program, simply press 'q' or 'Ctrl+C' at any time.

Expected Output: Real-time traffic analysis is displayed on your terminal.
