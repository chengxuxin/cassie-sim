# cassie-mujoco-sim

A simulation library for Agility Robotics' Cassie robot using MuJoCo.

Developed and tested on Ubuntu 16.04. Also compiles for Windows using mingw-w64.

To build the shared library:
1.  Install libglfw3-dev
2.  Clone this repository
3.  Download mjpro200_linux.zip from roboti.us
4.  Put mjpro200_linux in a `.mujoco` directory in your home directory
5.  `make`

To cross-compile for Windows, use `make PLATFORM=WIN`. (NOT TESTED)

A directory named release will be created containing the files needed to use the library with a C interface. A Python interface can be generated with `make ctypes`, which requires ctypeslib2 to be installed.

A license file for MuJoCo (mjkey.txt) is required to run the simulation. The library also includes functions that can be used for communicating with Cassie over UDP, and MuJoCo is not required if only these functions are called.

To build and run the examples:
1.  Place mjkey.txt in the `.mujoco`directory
2.  Set the `MUJOCO_KEY_PATH` environment variable to be `~/.mujoco/mjkey.txt`
3.  (Optional) Set the `CASSIE_MODEL_PATH` environment variable to be `<where you cloned this repository>/model/cassie.xml`
4.  `make test`
5.  Run the examples in the test directory

The examples include cassiesim, which simulates a physical Cassie robot controlled over UDP, and cassiectrl, a null controller operating over UDP. The file cassietest.c is a minimal example of running the simulation, and cassietest.py demonstrates controlling the simulated robot in Python.

Documentation for the simulation functions is included in include/cassiemujoco.h. The file header/udp.h declares convenience functions for creating a UDP socket and obtaining the most recent packet. The remaining header files include functions for packing and unpacking structures used to communicate with Cassie into UDP packets and for simulating Cassie's internal safety mechanisms and state estimation processes.

## Changes:
* Updated libraries to use mujoco 2.00
* MuJoCo activation key is now loaded by the `MUJOCO_KEY_PATH` environment variable. Add `export MUJOCO_KEY_PATH = <path to mjkey.txt>` to your `.bashrc`.
* To make things cleaner when loading MuJoCo and GLFW libraries, we assume the `mujoco200_linux` folder is `~/.mujoco` folder. This allows there to be only one copy of mujoco on your machine, rather than having to copy the mujoco folder to every place where `libcassiemujoco.so` is. 
* There are now two options for specifying a Cassie model file. The library will check if there is a `CASSIE_MODEL_PATH` environment variable defined. If such a variable is defined, it loads the model on that path. Otherwise, it will load the modelfile that is inputted into `cassie_sim_init`/`cassie_vis_init` (which is then passed along to `cassie_mujoco_init`). If you are using a environment variable then you can just pass `NULL` or an empty string to these methods. Using an environment variable is makes the usage invariant to the directory a program is run from, but requires changing and sourcing your `.bashrc` everytime you want to change modelfile. Note that a model file path (or `NULL` input) is also required when making a python `CassieSim` or `CassieVis` object, though you can make the argument have a default value so you don't have to input a model everytime.
* If building fails because it can't find `mujoco.h`, specify the full path in the Makefile when including mujoco200_linux/include, rather than using `~`.

## New Visualization Features
The F1 key will display a help screen that lists the most useful of the following functions.
* Mouse Interactivity:
  * The camera can be rotated by holding left mouse button and can be moved by holding the right mouse button.
  * Zoom the camera in and out using the scroll wheel.
  * To have the camera track a certain body, double control right-click on the desired body. To go back to free camera mode press the esc key.
  * Double left-click to select a body to apply a force on.
  * Once a body is selected, control right-click and drag to apply a force in a direction. The magnitude of the force is proportional to the distance from the cursor the body.
  * Control left-click allows you to apply a rotational force.
* Key Commands:
  * The cassie_vis_t struct now has a `paused` flag that can be toggled using the spacebar key. This allows to simulation to be paused and restarted. For an example of this, see `cassietest.c`.
  * The backspace key resets the model to the initial qpos.
  * When paused, the model can be simulated step by step using the right arrow key. To step forward in increments of 100, use the down arrow key.
  * The equal ('=') and minus ('-') keys can be used to increase and decrease the font size of the info screen respectively.
  * Ctrl+a will recenter the camera to the center of the platform
  * Ctrl+p will print the current qpos to the terminal
  * Ctrl+q will close the visualization window.
  * Toggle visualization flags:
    * 'j': Highlight the joint locations
    * 'u': Highlight the actuator locations
    * 'n': Highlight the constraint locations
    * 'c': Highlight points of contact
    * 'f': Show contact force arrows
    * 't': Make the model transparent
    * 'm': Show the center of mass of the entire model
  * F4 will bring up and overlayed graph of the current ground reaction forces (z foot contact forces).
* Additional functions and changes:
  * The function `cassie_sim_apply_force` now takes in a char array of the body name to perturb rather than the body id for user convenience. So the user only has to input the string of body name (i.e. "cassie-pelvis") rather than having to find out what integer body id corresponds to the desired body. The body names correspond to the body names defined in the .xml model file. These changes have been reflected in `cassiemujoco.py` and `cassiemujoco_ctypes.py` as well
  * There is a new `cassie_vis_apply_force` function that will apply **__and__** visualize a force. Note that `cassie_sim_apply_force` will still apply the force in the simulation, but when rendering the arrow showing the direction and magnitude of the force will not be shown. To visualize this arrow, you should use `cassie_vis_apply_force`. The force will be cleared out after the next call to `cassie_vis_draw`. So when using this function in python and dealing with varying execution and rendering frequencies, you can call `cassie_vis_apply_force` and then call `cassie_sim_step_pd` multiple times, all of which will be taken with the applied force. However, once `cassie_vis_draw` is called, subsequent simulations forward will not have the force applied (since the `xfrc_applied` field is set to zero in `cassie_vis_draw`). To continue applying the force, you must call `cassie_vis_apply_force` again.

## Features to be added:
* Slowmotion functionality? (requires changes to `cassiemujoco.py` and using GLFW in python when executing policies)
* Add full mujoco 2.00 ui functionality?

## Citation
To cite this software package use the following format:\
Agility Robotics. cassie-mujoco-sim. (2018) [Online]. Available: https://github.com/osudrl/cassie-mujoco-sim