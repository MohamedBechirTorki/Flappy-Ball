Flappy-Ball
===========

A simple C++ game inspired by Flappy Bird with sound effects.

---

How to Build and Run
--------------------

1. Clone the repository:
   ```bash
   git clone https://github.com/MohamedBechirTorki/Flappy-Ball
   cd Flappy-Ball

3. Build the project:
   ```bash
   cmake -S . -B build
   cmake --build build

5. Run the game:
   ```bash
   cd build
   ./FlappySound

---

Notes
-----

- The game uses assets (images, fonts, audio) located in the 'assets/' folder.
- Running the executable from the 'build/' directory requires copying the 'assets/' folder there.
- Some audio warnings related to ALSA/JACK may appear on Linux but can usually be ignored if audio works.
- Ensure all dependencies are installed (C++ compiler, CMake, SFML/SDL2 or related libs).

---

Dependencies
------------

- C++ compiler (e.g. g++)
- CMake (>= 3.10 recommended)
- Multimedia libraries (check CMakeLists.txt for specifics)

---

Contact
-------

For questions or contributions, contact Mohamed Bechir Torki.
