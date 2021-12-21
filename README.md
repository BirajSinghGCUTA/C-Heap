<div id="top"></div>
<!--
*** Thanks for checking out the Best-README-Template. If you have a suggestion
*** that would make this better, please fork the repo and create a pull request
*** or simply open an issue with the tag "enhancement".
*** Don't forget to give the project a star!
*** Thanks again! Now go create something AMAZING! :D
-->



<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->

<!--
[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]
-->


<!-- PROJECT LOGO -->
<br />
<div align="center">
  <h3 align="center">C-Heap: Heap Management For Linux</h3>

  <p align="center"> Implementation of <b>malloc</b> and <b>free</b> </p>
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

In C, the library function malloc is used to allocate a block of memory on the heap. The program accesses this block of memory via a pointer that malloc returns. When the memory is no longer needed, the pointer is passed to free which deallocates the memory so that it can be used for other purposes.

This is a basic implementation of Heap Management Library:
* This library that interacts with the OS to perform heap management on behalf of a user process 
* Implements the best-fit, worst-fit, next-fit, and first-fit memory allocation algorithms to locate free memory in the heap
* Allows users to efficiently use malloc, calloc, realloc, and free functions by created by utilizing doubly linked memory blocks

<p align="right">(<a href="#top">back to top</a>)</p>

### Built With

* [C]
* [C++]
* [Makefile]

<p align="right">(<a href="#top">back to top</a>)</p>


<!-- GETTING STARTED -->
## Getting Started

This library has been tested in a Linux OS, I recommend using Linux(RedHat or Ubuntu) while using this program. 

### Prerequisites

To run C++ or C, first install gcc compilers 
* buid-essential
  ```sh
  sudo apt install build-essential
  ```
* install manpage 
  ```sh
  sudo apt-get install manpages-dev
  ```

### Installation

The code compiles into four shared libraries and four test programs.  To build the code, change to your top level assignment directory and type: 

1. use make file to build
   ```sh
   make
   ```
2. Once you have the library, you can use it to override the existing malloc by using LD_PRELOAD: 
   ```sh
   $ env LD_PRELOAD=lib/libmalloc-ff.so tests/test1 
   ```
3. To run the other heap management schemes replace libmalloc-ff.so with the appropriate library: 
   ```sh
   Best-Fit:  libmalloc-bf.so  
   First-Fit: libmalloc-ff.so   
   Next-Fit:  libmalloc-nf.so  
   Worst-Fit: libmalloc-wf.so 
   ```
<p align="right">(<a href="#top">back to top</a>)</p>



<!-- USAGE EXAMPLES -->
## Usage Examples

1. To use malloc
 ```c
    int *p1 = malloc(4*sizeof(int));
   ```
2. To use calloc
 ```c
    int *p1 = calloc(4, sizeof(int));
   ```
3. To use relloc
 ```c
    int *p1 = realloc(7*sizeof(int));
   ```
4. To use free
 ```c
    malloc(p1);
   ```

 
Please refer to the [Documentation](https://example.com)_

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- ROADMAP -->
## Roadmap

- [x] Implement splitting and coalescing of free blocks. If two free blocks are adjacent then combine them. If a free block is larger           than the requested size then split the block into two. 
- [x] Implement three additional heap management strategies: Next Fit, Worst Fit, Best Fit, First Fit. 
- [x] Implement realloc and calloc. 
- [X] Allow realloc to decrease the allocation size.
- [X] Make the memory blocks doubly linked and use it for fast memory search and free

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- CONTRIBUTING -->
## Contributing

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE.txt` for more information.

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- CONTACT -->
## Contact

Biraj Singh GC- [@birajsinghgc@gmail.com](birajsinghgc@gmail.com)

Project Link: [https://github.com/BirajSinghGCUTA/C-Heap](https://github.com/BirajSinghGCUTA/C-Heap)

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

# PROF. Trevor J. Bakker

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/othneildrew/Best-README-Template.svg?style=for-the-badge
[contributors-url]: https://github.com/othneildrew/Best-README-Template/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/othneildrew/Best-README-Template.svg?style=for-the-badge
[forks-url]: https://github.com/othneildrew/Best-README-Template/network/members
[stars-shield]: https://img.shields.io/github/stars/othneildrew/Best-README-Template.svg?style=for-the-badge
[stars-url]: https://github.com/othneildrew/Best-README-Template/stargazers
[issues-shield]: https://img.shields.io/github/issues/othneildrew/Best-README-Template.svg?style=for-the-badge
[issues-url]: https://github.com/othneildrew/Best-README-Template/issues
[license-shield]: https://img.shields.io/github/license/othneildrew/Best-README-Template.svg?style=for-the-badge
[license-url]: https://github.com/othneildrew/Best-README-Template/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/othneildrew
[product-screenshot]: images/screenshot.png
