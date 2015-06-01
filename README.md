**Title**:        RTRender

**Author**:       Kasimov Ildar

**e-mail**:       ildar2571@yandex.ru

**Version**:      1.0

**Remark**: 

Some files of the project contained code's parts which were taken from other sources. If file includes
code from other source then it contains mark about this situation.

**Description**:

RTRender is my educational project, which was developed as part of course work. Main feature
of it is a support of parallelism. The engine provides using of MPI, OpenMP. It also can use sequential mode of
work. It includes a  BVH tree as an acceleration structure.

**Getting started**: 
       
This project was developed with using of Microsoft Visual Studio 2013 Community. 
If you use this version of Visual Studio you just should build sources. The result of building will be an execut-
able file of the program. Latest pre compiled program has already placed in Builds directory. 
       
The project depends on Microsoft MPI SDK (msmpi.dll). If you want to get maximum of program's
opportunities you need to install Microsoft MPI SDK. Or just use msmpi.dll which is placed with executable
file. However, you won't get control on the program, which uses MPI mode of running.

For getting an image by the program you need to prepare an XML file of scene's configuration. Also you
should create and edit configuration file of the program. This file must be named as config.xml. File with
scene's dscription has no pre defined name. You can name it as you wish. Information about syntax of
scene's defenition is contained in 'files descriptions' chapter.

**Supported platforms**: 

Main platform of project at the moment is Microsoft Windows OS.  But most of the code is crossplatform.
MPI implementation is the main part, which depends on specific OS. Therefore you just should rewrite 
the code which uses Microsoft MPI SDK. MPICH will be the best library for UNIX systems.

**Files description**:

For starting of a process of visualization the program needs two configuration files, which describe current
settings of a work. The first file 'config.xml' is a configuration file of the program. The second file has 
an arbitrary name and describes a scene.

The file config.xml is required file of the program. If the file contains  any error, the program will report
about it. A template of configuration file is placed below:

```xml
<?xml version="1.0" ?>

<config>

    <render type = "OMP" scheme="2" max_depth  = "4" />
    <camera type = "perspective" pos = "0.0;0.0;-1.0" target = "0.0;0.0;0.0" fov = "45.0" width="640" height="480" />
    <result type = "tga" path="." name = "result" />

    <scene path = "scene_1.xml" bg_color = "0.0;0.0;0.5">
        <bvh use = "true" max_depth = "64" />
    </scene>

</config>
```

=========================================================================================

Main section of the file is `<config>`, other subsections should be contained inside it.

The section `<render>` contains information about tracer settings. It should contain required attributes:
type, scheme, max_depth.

                 Attribute name | Valid values                     | Description 
                ------------------ | ------------------------   | --------------
                 type                      | OMP, MPI, Sequential | A type of ray tracer                 
                 scheme                | UINT                                | An index of scheme. For precompiled program the scheme index can be equals 1 or 2.                 
                 max_depth         | UINT                                | Mmaximal depth of recursion in ray tracing process. Too big values can freeze a run of program. 
                
The section `<camera>` contains information about camera. Required attributes: type, pos, target, width, height. 
There are additional attributes for the section.

                 Attribute name                                         | Valid values            | Description 
                ------------------------------------------ | -------------------- | --------------
               type                                                                |  perspective            | Camera's type. The program uses only perspective type of camera.               
               pos                                                                  | "float; float; float" | Position of camera in a space.               
               target                                                             | "float; float; float" | Sets point of interests for camera.               
               width                                                             | UINT                       | Width of an image.               
               height                                                            | UINT                       | Height of  an image.               
               (additional for perspective camera) fov | float                         | Angle of camera's view. It's used only for a perspective type of camera.
               
The section `<result>` contains information about result image. Main attributes: type, path, name.

                 Attribute name | Valid values | Description 
                ------------------ | ------------ | --------------
              type                         | tga, ppm     | Sets type of result image.              
              path                        | string          | Sets destination path for image.              
              name                      | string          | Sets name of result image.
              
The section `<scene>` describes settings of  the scene. Required attributes: path, bg_color. Information about
BVH tree is required to describe this section.
    
             Attribute name | Valid values            | Description 
             ------------------ | ------------             | --------------
             path                     | string                       | Path to the file with scene's defenition. The path should contain path + scene's file name. For instance: C:\Builds\OpenMP\scene1.xml.             
             bg_color             | "float; float; float" | Sets background color.
             
The section `<bvh>` contains information about BVH tree. Required attributes:use, max_depth.
                          
                          Attribute name | Valid values  | Description 
                            ------------------ | ------------ | --------------
                          use                           | true, false   | Sets using of the BVH structure. If it's false, then the structure is not used.                          
                          max_depth            | UINT          | A maximal depth of the constructed tree.
                          
============================================================================================

Example of scene's definition file is submitted below:

```xml
<?xml version="1.0" ?>

<scene>
  <materials>
    <material name = "std_diffuse" kd = "1.0" kr = "0.0" ks = "0.8" kt = "0.0" refract_coeff = "0.0" />    
    <material name = "std_transp" kd = "1.0" kr = "0.0" ks = "0.7" kt = "1.0" refract_coeff = "2.0" />
    <material name = "std_refl" kd = "1.0" kr = "0.4" ks = "0.9" kt = "0.0" refract_coeff = "0.0" />
    <material name = "mirror" kd = "1.0" kr = "0.8" ks = "0.9" kt = "0.0" refract_coeff = "0.0" />
  </materials>
  
  <lights>
    <light type = "point" pos = "60.0;150.0;60.0" color="1.0;1.0;1.0"/>
  </lights> 
  
  <primitives>                                                                                                         
     <primitive type = "mesh" path = ".\..\Models\floor.obj" material = "mirror" color = "0.7;0.0;0.0"/>
     <primitive type = "mesh" path = ".\..\Models\bunny.obj" material = "std_refl" color = "1.0;0.8;0.0"/>
     <primitive type = "mesh" path = ".\..\Models\mirror.obj" material = "mirror" color = "1.0;1.0;1.0"/>
     <primitive type = "sphere" center = "60.0;10.0;100.0" radius = "20.0" material = "std_transp" color = "1.0;1.0;1.0"/>
  </primitives>
</scene>            
```

Main section is `<scene>`, rest of it should be contained inside it.

The section `<materials>` contains information about used materials. This section MUST be the first in
definition file and contain all materials, which are used by primitives. There is subsection `<material>` 
describes a material. Required attributes: name, kd, kr, ks, kt, refract_coeff.
       
                 Attribute name | Valid values     | Description 
                ------------------ | ------------       | --------------       
              name                       | string               | Name of material.              
              kd                             | float in [0 .. 1] | Diffuse coefficient.              
              kr                             |float in  [0 .. 1]  | Reflection coefficient.              
              ks                             | float in [0 .. 1]  | Specular coefficient.              
              kt                             |float in  [0 .. 1]  | Transparency coefficient.              
              refract_coeff         | float in [1 .. n]  | Refraction coefficient. 
              
The section `<lights>` contains set of `<light>` elements, which describes light sources in scene. 
Required attributes `<light>` : type, pos, color. Also it contains additional attributes.

                 Attribute name | Valid values           | Description 
                ------------------ | ------------             | --------------            
              type                         | point                       | Light's type.              
              pos                           | "float; float; float"| Position of light source in a space.              
              color                       | "float; float; float" | Light's color.
              
The section `<primitives>` contains set of `<primitive>` elements, which sets primitives in scene.
Required attributes `<primitive>`: type, material, color. There are additional attributes, which
are depend on type of primitive.

                 Attribute name | Valid values                    | Description 
                ------------------ | ------------                      | -------------- 
               type                        | sphere, mesh, triangle | primitive's type.               
               material                | string                               | Name of the material. The material, which name is used in definition, SHOULD be predefined in the `<materials>` section.                 
               color                      | "float; float; float"         | Color of primitive.
               
Additional sphere's attributes: radius, center.
               
               Attribute name | Valid values            | Description 
                ----------------- | ------------              | -------------- 
               radius                   | float in (0, n)         | Radius of sphere.               
               center                  | "float; float; float" | Position of sphere's center.
                              
A triangle mesh contains an atribute, which describes path to the OBJ file of mesh.
               
Triangle is described by three attributes: p0, p1, p2.
               
                Attribute name | Valid values             | Description 
                ------------------ | ------------              | -------------- 
               p0, p1, p2               | "float; float; float" | Coordinates of triangle's points in a space.  
