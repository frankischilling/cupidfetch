# cupidfetch

## Overview

![cupid](images/cupid.png)

**this is cupid, the cat.**

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
* openSUSE
* Gentoo
* Void
* NixOS
* And more!

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
2. **Compilation:** `gcc -o cupidfetch main.c libs/ini.c` or use the makefile `make`. We plan on having a `make install` soon. 
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

The configuration file for `cupidfetch` is located at `~/.config/cupidfetch/cupidfetch.ini`. This file allows you to customize the displayed information by toggling options on or off. Below is an example of the configuration file:

```
[DisplayOptions]
host_name = 1
username = 1
distro = 1
linux_kernel = 1
uptime = 1
package_count = 1
shell = 1
desktop_environment = 1
local_ip = 1
available_memory = 1
memory_unit = MB
memory_unit_size = 1000000
```

### Display Options

- `host_name`: Display the host name.
- `username`: Display the username.
- `distro`: Display the Linux distribution.
- `linux_kernel`: Display the Linux kernel version.
- `uptime`: Display the system uptime.
- `package_count`: Display the package count.
- `shell`: Display the shell in use.
- `desktop_environment`: Display the desktop environment.
- `local_ip`: Display the local IP address.
- `available_memory`: Display available memory information.
- `memory_unit`: String representation of the unit used for the displaying of the RAM usage
- `memory_unit_size`: Value in bytes of the forementioned unit

To customize the displayed information, set the corresponding option to `1` to enable or `0` to disable.

## Requirements

* Linux system
* Basic understanding of C programming
* Curiosity for exploring system information

## How to Contribute

Welcome everyone! All skill levels can contribute:

* Beginners: Great project to learn and get involved
* Experienced developers: Share your knowledge and help improve
* Join our Discord!: [Discord](https://discord.gg/698GBkg2KR)

## To-do list

* **[X]** Clean code and info display overall
* **[ ]** Add ASCII art for each distro with custom kitty art
* **[ ]** Add colors at the bottom
* **[ ]** Fix alignment with text for proper ASCII art display
* **[ ]** Add unicode icons for each info area (ex. terminal icon for what shell you are using)
* **[ ]** Create and use a function that will get the geometry for each terminal so we arent hard coding values
* **[X]** Implement a soild function to detect a large amount of distros
* **[X]** Add memory info
* **[ ]** Add storage info
* **[ ]** Make install 
* **[X]** Config system using INI files with [inih C parser](https://github.com/benhoyt/inih) (done, pushing soon)

![Intended UI](images/ui.png)

## Notes

This project is a work-in-progress for learning and experimentation.

## License

[GNU General Public License 2.0 or later](https://www.gnu.org/licenses/old-licenses/gpl-2.0-standalone.html)


