//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#include <DockerTrustedRegistery_DTR.h>


DockerTrustedRegistery_DTR::~DockerTrustedRegistery_DTR() {
    // TODO Auto-generated destructor stub
}

void DockerTrustedRegistery_DTR::initialize(){

    // The number of Parallel file system remote servers (from .ned parameter)
    icancloud_Base::initialize();
}

void DockerTrustedRegistery_DTR::finish(){
    icancloud_Base::finish();
}

void DockerTrustedRegistery_DTR::manageImageRequest(RequestContainer* cont_req, VM* vmHost){

    // Extract the information from the storage request for the storage management
        int uid;
        int pId;
        int spId;
        int contId;

    // Init..
        uid = cont_req->getUid();
        pId = cont_req->getPid(); //vmId
        spId = cont_req->getSPid();
        contId= cont_req->getCid();

    // Define ..
       PendingImageRequest* pendingRequest;
       processOperations* processOperation;
       subprocessOperations* subprocessOperation;
       connection_T* connection;
       vector<connection_T*> connections;
       bool userFound;
       bool processFound;
       bool subprocessFound;

   // Init..
       connections.clear();
       userFound = false;
       processFound = false;
       subprocessFound = false;

   // Search if the connection exists..
       if (nodeHost == NULL) throw cRuntimeError ("DockerTrustedRegistery_DTR::userStorageRequest->Host to manage operations is NULL!");
   // Build the structure pending storage
       for (int i = 0; (i < (int)pendingStorageRequests.size()) && (!userFound); i++){

           pendingRequest = (*(pendingStorageRequests.begin() + i));

           // the user exists
           if (pendingRequest->uId == uid){
               userFound = true;

               // Search for the first level (processId)
               for(int j = 0; j < (int)(pendingRequest->processOperation.size()) && (!processFound); j++){

                   processOperation = (*(pendingRequest->processOperation.begin() + j));

                   // the job exists
                   if (processOperation->pId == pId){
                       processFound = true;

                       for (int k = 0; (k < (int)processOperation->pendingOperation.size()) && (!subprocessFound); k++){
                           subprocessOperation = (*(processOperation->pendingOperation.begin() + k));

                           // if the subprocessExists
                           if (subprocessOperation->spId == processOperation->pId){
                               // Not virtualized environment
                               subprocessFound = true;
                           } else if(subprocessOperation->spId == spId){
                               // Virtualized environment
                               subprocessFound = true;
                           }
                       }
                   }
               }
           }
       }

       // If not exists subprocess requests, create a new entry
       if (!subprocessFound){
           subprocessOperation = new subprocessOperations();
           subprocessOperation->spId = spId;
           subprocessOperation->operation = cont_req->getOperation();
           subprocessOperation->numberOfConnections = nodesTarget.size() + 1;
           subprocessOperation->pendingOperation.clear();

       }

           // Its time to distinguish between local or remote storage.
           if (cont_req->getOperation() == REQUEST_LOCAL_STORAGE){

              string opIp = cont_req->getConnection(0)->ip;

           // Create the connection
              connection = new connection_t();
              connection->ip = cont_req->getConnection(0)->ip;
              connection->pId =cont_req->getConnection(0)->pId;
              connection->port =cont_req->getConnection(0)->port;
              connection->uId =cont_req->getConnection(0)->uId;
             subprocessOperation->pendingOperation.push_back(connection);

             nodeHost->setLocalFiles(uid, pId, spId, opIp, cont_req->getPreloadFilesSet(), cont_req->getFSSet());

           } else if(cont_req->getOperation() == REQUEST_REMOTE_STORAGE){
               // Connections have the created data for connect node host with nodes target
               connections = createConnectionToStorage (cont_req, nodeHost, nodesTarget);

               for (int i = 0; i < (int)connections.size(); i++){
                   // save each connection as pending operation
                   connection = new connection_t();
                   connection->ip = (*(connections.begin() +i))->ip;
                   connection->pId =(*(connections.begin() +i))->pId;
                   connection->port =(*(connections.begin() +i))->port;
                   connection->uId =(*(connections.begin() +i))->uId;
                   subprocessOperation->pendingOperation.push_back(connection);

               }

           }else
               throw cRuntimeError  ("DockerTrustedRegistery_DTR::manageStorageRequest->error. Operation unknown\n");

       // Continue with the process for storaging a pending request
       if (!processFound){
           processOperation = new processOperations();
           processOperation->pId = pId;
           processOperation->pendingOperation.clear();
       }

       processOperation->pendingOperation.push_back(subprocessOperation);

       // If the user does not exists, create an entry for him.
       if (!userFound){
           pendingRequest = new PendingStorageRequest();
           pendingRequest->uId = uid;
           pendingRequest->processOperation.clear();
       }

       pendingRequest->processOperation.push_back(processOperation);

       pendingImageRequests.push_back(pendingRequest);

}




void DockerTrustedRegistery_DTR::notifyManager(icancloud_Message* msg){
    throw cRuntimeError ("DockerTrustedRegistery_DTR->to be implemented");
}

void DockerTrustedRegistery_DTR::connection_realized (StorageRequest* attendeed_req){

    // TODO
    throw cRuntimeError (" DockerTrustedRegistery_DTR::connection_realized ->to be implemented\n");

}
/* struct image{
          AbstractUser* userPtr;
          int imageId;
          string imageName;
          double imageSize_MB;
      };
      typedef struct image image_T; */

      /** Structure to manage the images */
void DockerTrustedRegistery_DTR::createImage(int userId, string imgName, double size)
{
    image_T* newImage;
    newImage->uId=userId;
    newImage->imageId=(int)imageList.size()+1;
    cout << "newImage->imageId:"<<newImage->imageId<<endl;
    newImage->imageName=imgName;
    newImage->imageSize_MB=size;

   imageList.insert(newImage,imageList.end());
}
     void DockerTrustedRegistery_DTR::removeImage()
     {

     }
     void DockerTrustedRegistery_DTR::pushImage()
     {

     }
     image_T* DockerTrustedRegistery_DTR::pullImage(){

     }
     void DockerTrustedRegistery_DTR::replicateImage(){

     }
     image_T* DockerTrustedRegistery_DTR::getImagebyId(int id){

     }
     image_T* DockerTrustedRegistery_DTR::getImagebyName(string name){

     }
     double DockerTrustedRegistery_DTR::getImageSizebyId(int id){

     }
     double DockerTrustedRegistery_DTR::getImageSizebyName(string name){

     }

