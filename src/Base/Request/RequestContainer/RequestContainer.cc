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

#include "RequestContainer.h"

RequestContainer::RequestContainer (){

    operation = -1;
    userId = -1;
    ContainerSet.clear();
    timesEnqueue = 0;
    state = 0;
    jobId = -1;
    vmId = -1;
    contId=-1;
    connections.clear();

}

RequestContainer::RequestContainer (int userID, int op, vector<Container*> newContainerSet){

    operation = op;
    userId = userID;
    ContainerSet.clear();
    ContainerSet = newContainerSet;
    timesEnqueue = 0;
    state = 0;
    jobId = -1;
 //   ContainerId = -1;
    connections.clear();
}

RequestContainer::~RequestContainer() {

    ContainerSet.clear();
    connections.clear();
}

Container* RequestContainer::getContainer(int position){

    // Define

        vector<Container*>::iterator it;
        Container* Container;
        int ContainerSetSize;
    // Init ..

        Container = NULL;
        ContainerSetSize = -1;

    // Begin
        ContainerSetSize = ContainerSet.size();
        if ((position <= ContainerSetSize) && (position > -1)) {

            Container = (*(ContainerSet.begin()+position));

        }

        return Container;
}

void RequestContainer::eraseContainer(int position){

    // Define

        vector<Container*>::iterator it;
        int ContainerSetSize;

    // Init ..
        ContainerSetSize = -1;

    // Begin

        ContainerSetSize = ContainerSet.size();
        if ((position < ContainerSetSize) && (position > -1)) {

            it = ContainerSet.begin() + position;
            ContainerSet.erase(it);

        }

}

RequestContainer* RequestContainer::dup (){

    RequestContainer* newRequest;
    vector <Container*> Containers;
    vector<connection_T*> connections;
    int i;

    newRequest = new RequestContainer();
    Containers.clear();
    connections.clear();

    newRequest->timesEnqueue = timesEnqueue;
    newRequest->state = state;
    newRequest->operation = getOperation();
    newRequest->jobId =getSPid();
    newRequest->userId = getUid();
    newRequest->vmId = getPid();
    newRequest->contId = getCid();
    newRequest->ip = ip;
    // ContainerSet
    if (ContainerSet.size() != 0){
        newRequest->setVectorContainer(ContainerSet);
    }

    if (connections.size() != 0){
        for (i =0; i < (int)connections.size(); i++){
            newRequest->setConnection(getConnection(i));
        }
    }

    // Duplicate the requested resources

    int differentTypes;

    differentTypes = ContainersToBeSelected.size();

    for (int i = 0; i < differentTypes; i++){
        newRequest->setNewSelection(getSelectionType(i), getSelectionQuantity(i));
    }

    return newRequest;

}

bool RequestContainer::compareReq(AbstractRequest* req){

        RequestContainer* reqContainer = dynamic_cast<RequestContainer*>(req);
        vector <Container*> Containers;
        vector<connection_T*> connections;
        bool equal = false;
        if (reqContainer != NULL){

            if ((reqContainer->getOperation() == getOperation()) &&
            (reqContainer->getUid() == getUid()) &&
            (reqContainer->getPid() == getPid()) &&
            (getSPid() == req->getSPid()) &&
            (getTimesEnqueue() == reqContainer->getTimesEnqueue()) &&
            (getState() == reqContainer->getState()) &&
            (reqContainer->getConnectionSize() == getConnectionSize()) &&
            (strcmp(getIp().c_str(), req->getIp().c_str()) == 0) &&
            (getContainerQuantity() == reqContainer->getContainerQuantity()) &&
            ((int)ContainersToBeSelected.size() == reqContainer->getDifferentTypesQuantity())
            ){
                equal = true;
            }
        }

        return equal;
}

void RequestContainer::setNewSelection(string typeName, int quantity){

    elementType* el;
    userContainerType* type;

    el = new elementType();
    el->setType(typeName);

    type = new userContainerType();
    type->type = el;
    type->quantity = quantity;

    ContainersToBeSelected.push_back(type);

}

string RequestContainer::getSelectionType(int indexAtSelection){
    return ((*(ContainersToBeSelected.begin() + indexAtSelection))->type->getType());
}

int RequestContainer::getSelectionQuantity(int indexAtSelection){
    return ((*(ContainersToBeSelected.begin() + indexAtSelection))->quantity);

}

void RequestContainer::decreaseSelectionQuantity(int indexAtSelection){
   ( (*(ContainersToBeSelected.begin() + indexAtSelection))->quantity--);

}

void RequestContainer::eraseSelectionType(int indexAtSelection){
    ContainersToBeSelected.erase(ContainersToBeSelected.begin() + indexAtSelection);

}


void RequestContainer::setForSingleRequest(elementType* el){

    userContainerType* type;

    type = new userContainerType();
    type->type = el;
    type->quantity = 1;

    ContainersToBeSelected.push_back(type);
}
