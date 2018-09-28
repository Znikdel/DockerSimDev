
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

#include "Request.h"
#include "VM.h"
#include "AbstractNode.h"
#include "icancloud_Base.h"

class DockerTrustedRegistery_DTR;

class DockerTrustedRegistery_DTR : virtual public icancloud_Base {
protected:

    /*
     *  This second level is to manage virtualization. If the operation came from a not virtualized
     *  environment spId will be equal to pId.
     */
    vector <image_T*> imageList;


        struct subprocessOperations_t{
            int spId;
            int operation;
           vector<connection_T*> pendingOperation;
           int numberOfConnections;
        };
        typedef struct subprocessOperations_t subprocessOperations;

        struct processOperations_t{
            int pId;
            vector<subprocessOperations*> pendingOperation;
        };
        typedef struct processOperations_t processOperations;

        struct pendingImageRequest_t{
            int uId;
            vector <processOperations*> processOperation;
        };
        typedef struct pendingImageRequest_t PendingImageRequest;

      // Struct for delete fs from a user
   /*       struct PendingRemoteStorageDeletion{
              int uId;
              int pId;
              int remoteStorageQuantity;
          };*/

      // Struct for waiting to close network connections
          struct PendingConnectionDeletion{
              int uId;
              int pId;
              int contId;
              int connectionsQuantity;
          };

protected:
      /** This vector allocates the storage requests for create FS or files until they will be realized*/
      vector <PendingImageRequest*> pendingImageRequests;

      /** This vector allocates the storage requests until it will be performed*/
      //vector <PendingRemoteStorageDeletion*> pendingRemoteStorageDeletion;

      /* this vector allocates the connections until it will be performed */
      vector<PendingConnectionDeletion*> connectionsDeletion;

      // The number of Parallel file system remote servers (from .ned parameter)
      //int numberOfPFSRemoteServers;


    /********************************************************************************************
     *              Operations for managing the storage of the system
     *********************************************************************************************/

    /*
   *  Create the filesystem and load a set of files (if there exists any) included into the the request.
   *  All the parameters are included into the request. The destination node, the files and the structure of the file system
   */
    void manageImageRequest(RequestContainer* cont_req, VM* vmHost);


    void connection_realized(RequestContainer* attendeed_req);


public:

    /*
     * It notify to the manager any event
     */
    virtual void notifyManager(icancloud_Message* msg);

    /*
     * This method returns the quantity of remote servers that will be used when a virtual machine request resources.
     */
protected:

   /* struct image{
          AbstractUser* userPtr;
          int imageId;
          string imageName;
          double imageSize_MB;
      };
      typedef struct image image_T; */

      /** Structure to manage the images */
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

