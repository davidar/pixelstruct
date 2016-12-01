ABOUT
PixelStruct[1] is an opensource tool for visualizing 3D scenes reconstructed
from photographs. It uses the opensource structure-from-motion system
Bundler[2], which is based on the same research as Microsoft Live Labs
Photosynth[3].



HOW TO COMPILE
PixelStruct requires the following libraries:
 - Qt5 (with OpenGL support)
 - CGAL


OSX   
-------------------------------   
brew install boost --with-python   
brew install qt5   
brew install cgal   
   
export CXXFLAGS=-std=c++11 //maybe not needed anymore?   
export CPLUS_INCLUDE_PATH=/usr/include/python2.7   
   
   
Troubleshooting   
https://github.com/Homebrew/legacy-homebrew/issues/29938   
replace 5.7.0 -> current version of qt!!!   
sudo ln -s /usr/local/Cellar/qt5/5.7.0/mkspecs /usr/local/mkspecs && ln -s /usr/local/Cellar/qt5/5.7.0/plugins /usr/local/plugins   



You will also need to install CMake in order to build PixelStruct.

All of the above can be installed in Ubuntu (and likely other Debian-based
distributions) by running
    sudo aptitude install libqt4-dev libcgal-dev cmake

Once you have installed these, compile the program by running `make` in the
project root directory.

This will create a binary called 'pixelstruct' in the current directory.
You can either copy this executable into your PATH for system-wide access, or
just run it immediately by either calling `./pixelstruct` in a terminal or by
opening it with your file browser.

USAGE
PixelStruct requires Bundler, which can be downloaded from [2]. First, you need
to manually run Bundler on a directory of images as described in the README.txt
file contained in the Bundler distribution. Once Bundler has completed
processing the images, the image set can be loaded into PixelStruct.

If you do not want to manually run Bundler, and would rather view a
pre-processed dataset, you have two options:
- download the small (3.6MB) Kermit example from [4], untar it, and open the
  kermit/ directory in PixelStruct
- download the very large (1.8GB) Notre Dame example from [5], unzip it, create
  a bundle/ directory inside the NotreDame/ directory, copy/move notredame.out
  to bundle/bundle.out, and open the NotreDame/ directory in PixelStruct

To view a processed dataset, launch PixelStruct as described above, and go to
"File>Open image directory". Navigate into the image directory you ran Bundler
on, and hit "Choose"/"Open". Alternatively you can specify the image directory
as a command-line argument when launching PixelStruct. The 3D point cloud will
be loaded into the window, and once the viewer has focus (you may have to click
on it for this), the camera will focus on the first image.

You can now hit your Return key to move to the next image, or use your arrow
keys to move to another image in the chosen direction. Pressing Page Up and
Page Down will attempt to move the camera closer to and further away from the
photographed object respectively.

The View menu allows you to choose between several transition modes, as
described in section 5.3 of [6].

QPL-GPL INCOMPATIBILITY NOTICE
As portions of CGAL are licensed under the QPL[7] which is incompatible with
the GPL (see the QPL secction of [8]), the following notice is required:
  As a special exception, you have permission to link this program
  with the CGAL library and distribute executables, as long as you
  follow the requirements of the GNU GPL in regard to all of the
  software in the executable aside from CGAL.

[1] http://da.vidr.cc/projects/pixelstruct/
[2] http://phototour.cs.washington.edu/bundler/
[3] http://photosynth.net/
[4] http://pixelstruct.googlecode.com/files/kermit-example-bundler0.3.tar.gz
[5] http://phototour.cs.washington.edu/datasets/NotreDame.zip
[6] http://phototour.cs.washington.edu/Photo_Tourism.pdf
[7] http://doc.trolltech.com/4.3/qpl.html
[8] http://www.fsf.org/licensing/licenses/index_html
