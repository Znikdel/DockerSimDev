
/* This module communicate with docker daemon using docker API
 * it also manages replications and keep track of images on different machines
 *
 * create image
 * remove image
 * recieve requests over port 80 and 443
 * push images
 * pull images
 * replicate image
 * getimagesids on a specific machine
 * put image on a specific machine
 * get machine id of a specific image
 * delete user and its repositories: if image has only one user id then delete it
 *
 * if you delete the image owner then notify all other users using it and delete the image
 * if you are deleting a user then clear all vectors from that user id and keep image as long as image owner exists
 *
 *
 */
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

#ifndef DOCKERTRUSTEDREGISTERY_DTR_H_
#define DOCKERTRUSTEDREGISTERY_DTR_H_

#include "MachinesMap.h"
#include "AbstractNode.h"
#include "icancloud_Base.h"

class DockerTrustedRegistery_DTR;

class DockerTrustedRegistery_DTR : virtual public icancloud_Base {

protected:

   /* struct image{
          AbstractUser* userPtr;
          int imageId;
          string imageName;
          double imageSize_MB;
      };
      typedef struct image image_T; */

      /** Structure to manage the images */
          vector <image_T*> imageList;
    //bool isBaseImage;
    virtual ~DockerTrustedRegistery_DTR();

public:


    /*
     * Module initialization
     */
     void initialize();

     /**
     * Module ending.
     */
     void finish();

  /*   * create image
      * remove image
      * recieve requests over port 80 and 443
      * push images
      * pull images
      * replicate image
      * getimagesids on a specific machine
      * put image on a specific machine
      * get machine id of a specific image
      * delete user and its repositories */
     void createImage();
     void removeImage();
     void pushImage();
     image_T* pullImage();
     void replicateImage();
     image_T* getImagebyId(int id);
     image_T* getImagebyName(string name);
     double getImageSizebyId(int id);
     double getImageSizebyName(string name);

};

#endif /* DOCKERTRUSTEDREGISTERY_DTR_H_ */

