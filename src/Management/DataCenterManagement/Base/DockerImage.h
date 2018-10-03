/*
 *
 * An image is a read-only template with instructions for creating a Docker container.
 * Often, an image is based on another image, with some additional customization.
 * For example, you may build an image which is based on the ubuntu image,
 *  but installs the Apache web server and your application,
 *  as well as the configuration details needed to make your application run.

You might create your own images or you might only use those created by others
and published in a registry.
To build your own image,
you create a Dockerfile with a simple syntax for defining the steps needed to create the image and run it.
Each instruction in a Dockerfile creates a layer in the image.
When you change the Dockerfile and rebuild the image, only those layers which have changed are rebuilt.
This is part of what makes images so lightweight, small, and fast, when compared to other virtualization technologies.
 */

#ifndef DOCKERIMAGE_H_
#define DOCKERIMAGE_H_

#include "MachinesMap.h"
#include "AbstractNode.h"
#include "icancloud_Base.h"

class DockerImage;

class DockerImage : virtual public icancloud_Base {

   /* id
    * user_id owner
    * vector of user_ids who can access/use this image
    * machine id  // host id
    * memory  // how much memory this image needs
    * baseImageID  default is 0 means no base image
    *
    *
    *

    */
protected:
    int id;
    double imageSize_MB;
    int userID;
    //bool isBaseImage;
    virtual ~DockerImage();

public:


    /*
     * Module initialization
     */
     virtual void initialize();

     /**
     * Module ending.
     */
     virtual void finish();

};

#endif /* DOCKERIMAGE_H_ */
