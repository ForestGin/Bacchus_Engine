# Bacchus_Engine
3D Engine built from scratch during our 3rd year of Game Development Bachelor's Degree.

Version 0.2

Made by ForestGin: Pol Bosch & Oscar Larios

Github links:

* [ForestGin](https://github.com/ForestGin): [Bacchus Engine](https://github.com/ForestGin/Bacchus_Engine)
* [Pol Bosch](https://github.com/Xulu-u)
* [Oscar Larios](https://github.com/Megaoski)

## How to use

Just open the .exe file and "enjoy" the engine

### Navigation

* Mouse and Keyboard controls enabled on focused window.

### Camera Controls

* Mouse Right button for look around
* Mouse Right button + WASD to do free movement
* Mouse Wheel button for pan
* Mouse scrolling to zoom in/out
* Alt + Mouse Right button to orbit
* Hold shift for double speed
* F to frame selected object
* If there's a camera GO in the scene you can select it as active/culling.

### Mouse Picking

* Mouse Left button to select game object

### Game Objects

* All fbx in assets folder will be imported at startup.
* Head to the GameObject window and select empty, camera or any of the available 3D primitives. 
* In the case of an FBX file just drag and drop it inside the screen.
* Each GO has an inspector panel in which you will see some useful information or play with the GO Transform, normals... etc.
* Press "supr" key to delete selected 

### Debug

* To see and use the debug features(wireframe mode, depth, lightning...) go to Window, open the Settings window and then enter the Renderer. 

## Features that did not make it

* Importer Settings Window and functionality
* Asset Window and functionality

##Versions Changelog
### 0.2

* Primitive Structure of Resource Manager
* Reference Counting for ResourceMesh and ResourceTexture
* Scene serialization to file and loading
* New import method (creates own file format files in library)
* New Time Manager (Options to PLAY STOP STEP)
* Spatial partitioning using Octree
* Creation of cameras
* Frustum culling
* AABB's
* Project window
* Mouse picking 

### 0.1

* Game Object and Component Structure
* Initial Implementation on Inspector (properties for the different components)
* Modification of many Menus/Windows
* Possibility of primitives creation through GameObject Menu
* Unity-like Camera Controls 
* Extra primitive shapes: Tetrahedron, Octahedron, Cubesphere, Cone.

### 0.05

* Libraries Integration
* Basic Editor with Inspector/Hierarchy/Console/Menu
* Initial GameObject and Component structure
* Save & Load Editor Configuration
* Load 3D Models through assimp (fbx format)
* Load Textures through DevIL (dds and png format)
* Docking
* Imgui personalization of the engine

## Licence
MIT License

Copyright (c) 2020 ForestGin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
