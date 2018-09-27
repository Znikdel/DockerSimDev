/*
 * @class RequestContainer RequestContainer.h "RequestContainer.h"
 *
 * This class define and manages a container request by a tenant
 * it offers methods for:
 *      managing single Container request
 *      analyzing the resources requested
 * @author zahra nikdel
 * @date 2018-09-027
 */

#ifndef REQUESTCONTAINER_H_
#define REQUESTCONTAINER_H_

#include "Container.h"
#include "Request.h"

class Container;

class RequestContainer : public AbstractRequest{

private:

    // To return the started Containers
    vector<Container*> ContainerSet;
    vector<connection_T*> connections;

    // To request Containers
    struct userContainerType_t{
        elementType* type;
        int quantity;
    };

    typedef userContainerType_t userContainerType;

    vector<userContainerType*> ContainersToBeSelected;

public:

    RequestContainer();

    RequestContainer(int userID, int op, vector<Container*> newContainerSet);

    virtual ~RequestContainer();

    int getContainerQuantity(){return ContainerSet.size();};
    Container* getContainer(int position);
    void eraseContainer(int position);
    int getNumberContainer (){return ContainerSet.size();};
    void setVectorContainer(vector<Container*> newContainerSet){ContainerSet = newContainerSet;};
    vector<Container*> getVectorContainer(){return ContainerSet;};

    void setConnection(connection_T* c){connections.push_back(c);};
    connection_T* getConnection(int index){return (*(connections.begin()+index));};
    int getConnectionSize(){return connections.size();};
    void eraseConnection(int i){connections.erase(connections.begin()+i);};
    void eraseConnection(){connections.clear();};

    // Methods for analyzing the resources requested
    void setNewSelection(string typeName, int quantity);
    int getDifferentTypesQuantity(){return ContainersToBeSelected.size();};
    string getSelectionType(int indexAtSelection);
    int getSelectionQuantity(int indexAtSelection);
    void eraseSelectionType(int indexAtSelection);
    void decreaseSelectionQuantity(int indexAtSelection);

    // Methods for managing single Container request
    void cleanSelectionType(){ContainersToBeSelected.clear();}
    void setForSingleRequest(elementType* el);
    elementType* getSingleRequestType(){return (*(ContainersToBeSelected.begin()))->type;};

    RequestContainer* dup ();

    bool compareReq(AbstractRequest* req);

};

#endif /* REQUESTContainer_H_ */
