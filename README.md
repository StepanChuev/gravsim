# gravsim
Gravitation Simulation

## Building
To build gravsim, you must have ``SDL2``.

```
git clone https://github.com/StepanChuev/gravsim.git 
cd gravsim
mkdir obj
make
```

I don't know how gravsim builds on Windows, but I made source code compatible with Windows, I hope.

## Configuration Files
The base for interaction with gravsim is configuration files, they enables write information about system and uses for saves.
What means "system"? System is necessary information for physics calculations and drawing:

- Iteration. Uses for countdowning iterations, saves and exiting from gravsim.
- Gravitational constant.
- Amount of bodies.
- Physics information of body: x, y, vx, vy, m.
- Rendering information of body: color (RGB) and radius. It's not required if flag ``-fnouseren`` set.

To write a configuration file, you may stick next template:

```
# It is the test config file for gravsim

@iter: 0
@len: 3
@G: 1
# Bodies:
x:   0.0
y:   0.0
vx:  0.0
vy:  0.0
m:   20.0
r:   7
rgb: FFFF00
%
x:   0.0
y:  -100.0
vx:  -0.2
vy:  0.0
m:   1.0
r:   5
rgb: FF0000
%
x:   0.0
y:   100.0
vx:  0.3
vy:  0.0
m:   1.0
r:   5
rgb: 0000FF
```

Every configuration file shall be correspond to the farther rules:
- Designations: 
	- @iter - iteration.
	- @len - amount of bodies.
	- @G - gravitational constant (may be decimal fraction).
	- r - radius (may be decimal fraction).
	- rgb - color in RRGGBB hexadecimal format.
	- And x, y, vx, vy, m - also may be decimal fraction.
- Spliter between key and value is ":".
- Between bodies must be wrote "%", it's spliter between bodies.
- For single-line commentaries write "#".
- @len must be wrote before bodies.
- If you set ``-fnouseren`` flag, you may don't write r and rgb.

## Flags and Options
When you built gravsim, you can start program from cli. The -sys option is **required**.

``./gravsim -sys path/to/your/system [Another flags and options]``

- ``-sys PATH`` - Path to system configuration file for drawing and calculating. This option is required.
- ``-sdir DIR`` - Save directory.
- ``-spref PREF`` - Prefix for configuration save-files.
- ``-siter N`` - Program will automatically save current state in configuration file every N iterations.
- ``-eiter N`` - Exit iteration. If save iteration (``-siter``) multiple exit iteration, then save will be executed before exit.
- ``-w N`` - The width of gravsim window.
- ``-h N`` - The height of gravsim window.
- ``-fps N`` - FPS
- ``-ss N`` - Scale step.
- ``-ms N`` - Move step.
- ``-fixi N`` - The index of fixed body. This body centered by window.
- ``-fpause`` - Stop simulation in the begining.
- ``-fnodraw`` - Don't create window
- ``-fnouseren`` - Don't read/write rendering information from/to configuration files.
- ``-fnolog`` - Don't print all information in cli.
- ``-fnologi`` - Don't print iteration.
- ``-fnologb`` - Don't print bodies.

## Saves
Specially, I explain how saves works. Every saved file names in such format: ``PREFIX_ITERATION.conf``, 
PREFIX defines via ``-spref``, ITERATION is iteration when save done, also you may define save-directory via ``-sdir``.
Sometimes you need in automatically saves, then ``-siter`` set number and when number multiple iteration, save will be done automatically.
When you (or gravsim) create save, file will be named in save-file format and this file maybe use after.

## Keyboard and Mouse Control
- The keys w, a, s, d for movement to forward, right, backward and left consequently.
- The key l to save.
- The SPACE key for pauses.
- The ARROW UP/DOWN for scaling.
- Click on body by right mouse button to fix on body.
- Click on left mouse button to reset movement and fixation.