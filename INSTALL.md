# INSTALLATION & BUILD INSTRUCTIONS

# OSSIA build instructions
For a recent documentation : please refer to https://github.com/OSSIA/OSSIA

# Doxygen Documentation

In order to have documentation of the sources you have to install Doxygen (www.doxygen.org),
and the Graphviz package to generate graph with Doxygen (www.graphviz.org).
To generate documentation,
load ./doxygen/Doxyfile with Doxygen and run it.
			
# Manual installation/build (old way)

- Download and install libraries from :
	-- Qt (LGPL) 4.8 : http://download.qt-project.org/official_releases/qt/4.8/4.8.6/qt-opensource-mac-4.8.6.dmg 
	-- Gecode 3.2.0 : http://www.gecode.org/download/Gecode-3.2.0.dmg
	-- LibXml2 : http://www.explain.com.au/download/combo-2007-10-07.dmg.gz -> copy libxml.framework to /Library/Frameworks

- Get git repositories by downloading (or cloning) Score : https://github.com/OSSIA/Score

- Build Score, following the steps in the README.

- cd into i-score folder with your terminal.

- Compile by running the script : ./build.sh

