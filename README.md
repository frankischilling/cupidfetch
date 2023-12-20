# cupidfetch

## Overview

![cupid](images/cupid.png)

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

**⬜ Enhance visual representation (soon):**

* ASCII art for distributions
* Unicode icons

**✔️ Built for beginners:**

* Simple code
* Easy to understand and contribute

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
sudo apt update
sudo apt install build-essential
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
2. **Compilation:** `gcc -o cupidfetch main.c libs/ini.c`
3. **Execution:** `./cupidfetch`
4. **Output:** System information with:
    * **(WIP)** ASCII art representing the Linux distribution
    * **(WIP)** Unicode icons for different details

## Requirements

* Linux system
* Basic understanding of C programming
* Curiosity for exploring system information

## How to Contribute

Welcome everyone! All skill levels can contribute:

* Beginners: Great project to learn and get involved
* Experienced developers: Share your knowledge and help improve

## To-do list

* **[ ]** Clean code and info display overall
* **[ ]** Add ASCII art for each distro with custom kitty art
* **[ ]** Add colors at the bottom
* **[ ]** Fix alignment with text for proper ASCII art display
* **[ ]** Add unicode icons for each info area (ex. terminal icon for what shell you are using)
* **[ ]** Create and use a function that will get the geometry for each terminal so we arent hard coding values
* **[ ]** Implement a soild function to detect a large amount of distros
* **[ ]** Make sure you can execute cupidfetch at any point in the system (right now you have to be in the directory)
* **[X]** Config system using INI files with [inih C parser](https://github.com/benhoyt/inih) (done, pushing soon)

![Intended UI](images/ui.png)

## Note

This project is a work-in-progress for learning and experimentation.

## License

[GNU General Public License 2.0 or later](https://www.gnu.org/licenses/old-licenses/gpl-2.0-standalone.html)
