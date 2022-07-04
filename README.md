<div id="top"></div>

<!-- PROJECT LOGO -->
<br />
<div align="center">
  <h3 align="center">Battleship</h3>
  <p align="center">
    <a href="#">View Demo (TODO)</a>
  </p>
</div>

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li><a href="#about-the-project">About The Project</a></li>
    <li><a href="#context">Context</a></li>
    <li><a href="#built-with">Built With</a></li>
    <li><a href="#documentation">Documentation</a></li>
    <li><a href="#getting-started">Getting Started</a></li>
    <li><a href="#usage">Usage</a></li>
  </ol>
</details>

<!-- ABOUT THE PROJECT -->
## About The Project

<div align="center">
  <img src="project-image.png">
</div>
<br />

This project basic principle is: a reproduction of the existing game **[Battleship](https://en.wikipedia.org/wiki/Battleship_(game))** playable with bots (no multiplayer).

<p align="right">(<a href="#top">back to top</a>)</p>

<!-- CONTEXT -->
## Context

This project is a personal project and it was my first steps with the C language and Linux programming.

<p align="right">(<a href="#top">back to top</a>)</p>

## Built With

Entire project was built with pure C and is only compatible with Linux.

<p align="right">(<a href="#top">back to top</a>)</p>

<!-- DOCUMENTATION -->
## Documentation

Doxygen docs can be generated in HTML by running `doxygen` command on file `doxy/Doxyfile` ([Doxygen](https://doxygen.nl/) must be installed on your system).

<!-- GETTING STARTED -->
## Getting Started

To get a local copy up and running follow the steps:

### Prerequisites

To compile the project, you will need:
* [Make](https://en.wikipedia.org/wiki/Make_(software))
```sh
apt-get install make
```

Optional dependency:
```sh
apt-get install doxygen
```

### Installation

1. Clone the repo:
   ```sh
   git clone https://github.com/Akwd22/bataille-navale.git
   ```
2. Compile the project:
   ```sh
   cd bin
   make
   ```

<p align="right">(<a href="#top">back to top</a>)</p>

<!-- USAGE EXAMPLES -->
## Usage

You can configure game options such as the size of the game board and the number of boats through launch parameters:
* `--help` to get the list of parameters.

<p align="right">(<a href="#top">back to top</a>)</p>
