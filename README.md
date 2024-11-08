# cupidfetch

## Overview

**this is cupid, the cat.**

![cupid](images/cupid.png)


![baby cupid](images/smol.png)

**cupid the cat loves linux!**

cupidfetch is a system information retrieval tool written in C for Linux systems. It's a beginner-friendly, work-in-progress hobby project aimed at learning and exploring programming concepts.

## Features

**✔️ Fetches and displays various system details:**

* Hostname
* Distribution
* Kernel version
* Uptime
* Package count
* Shell
* Terminal
* Desktop environment
* Username
* Memory
* Signal Handling for Window Resize, Automatically updates display with terminal resizing (`SIGWINCH`)
* and others

**⬜ Enhance visual representation (soon):**

* ASCII art for distributions
* Unicode icons

**✔️ Built for beginners:**

* Simple code
* Easy to understand and contribute

**Supported Distros:**
* Debian (Ubuntu, elementary, Mint) [Verified ✔️]
* Arch (Manjaro, Artix, EndeavourOS) [Verified ✔️]
* Fedora [Verified ✔️]
* Others are in `data/distros.def` (feel free to add your)

## Dependencies

cupidfetch relies on the following components and tools:

1. **C Compiler:** cupidfetch is written in the C programming language, and compilation requires a C compiler. The recommended compiler is GCC (GNU Compiler Collection).

   - [GCC (GNU Compiler Collection)](https://gcc.gnu.org/): The GNU Compiler Collection is a set of compilers for various programming languages, including C.

2. **inih C Parser:** cupidfetch uses the [inih C parser](https://github.com/benhoyt/inih) to handle configuration files in INI format.

   - [inih C parser](https://github.com/benhoyt/inih): A simple C library for reading and writing INI files.
     
3. **Git:** cupidfetch uses Git to clone the repository for easy deployment.
   
   - If you don't have Git installed, you can download and install it from the [Git website](https://git-scm.com).

## How to Install Dependencies

### GCC (GNU Compiler Collection)

GCC is often available through the package manager of your Linux distribution. For example, on Debian/Ubuntu-based systems, you can install it using:

```
sudo apt update && sudo apt install build-essential
```

### Git

Git can be installed through the package manager of your Linux distribution. For example, on Debian/Ubuntu-based systems, you can install it using:

```
sudo apt install git 
```

## inih C Parser

The inih C parser can be integrated into your project either by manually downloading the source files from the [inih GitHub repository](https://github.com/benhoyt/inih) or by using a package manager if available.

## Usage
1. **Clone** `git clone https://github.com/frankischilling/cupidfetch`
2. **Compilation:** `gcc -o cupidfetch src/config.c src/main.c src/modules.c src/print.c libs/ini.c` or use the makefile `make`. We plan on having a `make install` soon. 
3. **Execution:**
   - **To run from the current directory:** `./cupidfetch`
   - **To run from anywhere:**
         - Move the executable to `/usr/local/bin`: `sudo mv cupidfetch /usr/local/bin`
     
         - Execute it directly: `cupidfetch`
4. **Debug** Use `make clean asan` or `make clean ubsan` to check overflows/memory leaks or undefined behavior
5. **Output:** System information with:
    * **(WIP)** ASCII art representing the Linux distribution
    * **(WIP)** Unicode icons for different details

## Configuration File

You can use the `install-config.sh` script to create cupidfetch the config. You can test some of the examples in `examples/` by running `cupidfetch < examples/example.ini`.

The configuration file for `cupidfetch` is located at `${XDG_CONFIG_HOME or $HOME/.config}/cupidfetch/cupidfetch.ini`. This file allows you to customize the displayed information by toggling options on or off. An example config is at the `data` directory of this repository. It includes some comments that may help you.

**Storage units**

The config lets you change what memory unit you prefer to use for storage and memory information. The storage unit size is measured in bytes, so if you wanted to use MB, it would be `1000000` bytes but if you wanted to use GB, it would be `1000000000` bytes.

## Log File

If it can't create a log at `.../cupidfetch/log.txt`, it will output to the stderr. If you want to ignore it, please do `cupidfetch 2> /dev/null`.

## Requirements

* Linux system
* Basic understanding of C programming
* Curiosity for exploring system information

## How to Contribute

Welcome everyone! All skill levels can contribute:

* Join our Discord!: [Discord](https://discord.gg/698GBkg2KR)
* Beginners: Great project to learn and get involved
* Experienced developers: Share your knowledge and help improve

## Adding support for more distros

To add other Linux distros to be supported you can do so by adding them to the `data/distros.def` file. Here is an example of the dsitros.def file.

```
DISTRO("ubuntu", "Ubuntu", "dpkg -l | tail -n+6 | wc -l")
```

Let's say you wanted to add a new distro that uses dpkg called cupidOS; you could edit the distros.def like this.

```
DISTRO("ubuntu" , "Ubuntu" , "dpkg -l | tail -n+6 | wc -l")
DISTRO("cupidOS", "cupidOS", "dpkg -l | tail -n+6 | wc -l")
```

## To-do list

- [ ] Add ASCII art for each distro, and funny text along side each distro for example the arch kitten could be saying "I use arch btw"
- [ ] Auto check for distro if found in config cont, if not then auto add to distros.def  
- [ ] Add colors/user theming
- [ ] Fix alignment with text for proper ASCII art display
- [ ] Add unicode icons (nerd fonts?)
- [ ] Make install
- [ ] Arch AUR
- [ ] Individual module config (the section is the module being configured)
- [ ] Implement WM & DE functions that arent hard-coded
- [X] Signal Handling for Window Resize: Automatically updates display with terminal resizing (`SIGWINCH`).
- [X] Implement a solid function to detect a large amount of distros
- [X] Add memory info
- [X] Add storage info
- [X] Fix terminal info, DE, and WM info and include more support
- [X] Config system
- [X] Clean code and info display overall

![Intended UI](images/ui.png)

## Notes

This project is a work-in-progress for learning and experimentation.

## License

[GNU General Public License 2.0 or later](https://www.gnu.org/licenses/old-licenses/gpl-2.0-standalone.html)


