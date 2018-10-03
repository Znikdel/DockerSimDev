//The daemon creates and manages Docker objects, such as images, containers, networks, and volumes.
//Docker uses a client-server architecture. The docker client talks to the docker daemon
//daemon does building, running, and distributing docker containers
//the docker client and server can run on the same system or client can connect to a remote daemon
//They communicate using a REST API, over unix sockets or a network interface
// A daemon can communicate with other daemons to manage docker services
/*
 *
 *
 * Example docker run command

The following command runs an ubuntu container, attaches interactively to your local command-line session, and runs /bin/bash.

$ docker run -i -t ubuntu /bin/bash

When you run this command, the following happens (assuming you are using the default registry configuration):

    1-If you do not have the ubuntu image locally, Docker pulls it from your configured registry, as though you had run docker pull ubuntu manually.

    2-Docker creates a new container, as though you had run a docker container create command manually.

    3-Docker allocates a read-write filesystem to the container, as its final layer. This allows a running container to create or modify files and directories in its local filesystem.

    4-Docker creates a network interface to connect the container to the default network, since you did not specify any networking options. This includes assigning an IP address to the container. By default, containers can connect to external networks using the host machine’s network connection.

    5-Docker starts the container and executes /bin/bash. Because the container is running interactively and attached to your terminal (due to the -i and -t flags), you can provide input using your keyboard while the output is logged to your terminal.

    6-When you type exit to terminate the /bin/bash command, the container stops but is not removed. You can start it again or remove it.
 *
 */
//
// This module defines a virtual machine messages controller
//
// All the messages that a virtual machine generates are controlled by this module. It has two main functionalites:
//   - The messages has a commId parameter. This parameter is setted by the applications. For example, when an application is distributed
//     it is used to identify the rank of the source/destination process. This module, storage the original value for this parameter
//     and it is replaced with the position of the application at the gates vector. This is in order to know which is the owner of the message if
//     it is responsed with a request.
//   - The second functionality (under developement) is the migration manager. It control the quantity of messages that are out of
//     application in order to block the out messages and wait for the incoming messages to disconnect gates and reconnect to the destination
//     host when it would be possible
//
// @author Gabriel Gonz&aacute;lez Casta&ntilde;&eacute
// @date 2012-10-23
//

#ifndef DOCKERDAEMON_H_
#define DOCKERDAEMON_H_

#include "icancloud_Base.h"

class DockerDaemon : public icancloud_Base {

private:

    bool migrateActive;                             // If the migration is active or not.
    vector<icancloud_Message*> pendingMessages;     // Vector to storage the pending messages
    int pendingCPUMsg;                              // Quantity of pending cpu messages
    int pendingNetMsg;                              // Quantity of pending net messages
    int pendingMemoryMsg;                           // Quantity of pending memory messages
    int pendingIOMsg;                               // Quantity of pending I/O messages

    icancloud_Message *migration_msg;               // to store the message to the remote storage app until the messages will be blocked

    cGateManager* fromOSApps;                       // Input gate from this class to the operative system
    cGateManager* toOSApps;                         // Output gate from this class to the operative system
    cGateManager* fromApps;                         // Input gate from this class to an application
    cGateManager* toApps;                           // Output gate from this class to an application

    int uId;                                        // The user identification value
    int pId;                                        // The process (VM) identification value
    int conId;                                      // The container id
    /*
     * Data structures to control the communication identificators
     */
    struct commIdVectorInternals_t{
        int commId;
        int msgId;
    };

    typedef commIdVectorInternals_t commIdVectorInternals;

    struct commIdVector_t{
        vector<commIdVectorInternals*> internals;
        int uId;
        int pId;
        int conId;
    };

    typedef commIdVector_t commIdVector;
    vector<commIdVector*> commVector;

public:

    DockerDaemon();
    virtual ~DockerDaemon();

    /*
     * Method to block the output of messages from the VM to the OS
     */
    void blockMessages ();

    /*
     * Method to reactivate again the messages from the controller to the operating system
     */
    void activateMessages();

    /*
     * If there is no pending messages
     */
    bool migrationPrepared();

    /*
     * This method links a new applications.
     */
    void linkNewContainer(cModule* conAppModule, cGate* scToApp, cGate* scFromApp);

    /*
     * This method unlink the given application as parameter. It disconnects all the gates (4)
     */
    int unlinkContainer(cModule* conAppModule);

    /*
     * This method set the user identification and the VM identification to be setted at each message from app to the OS
     */
    void setId(int userId, int vmId);
    cGateManager* getmyGate(string gateName);
    cGate* myNewGate(string gateName);

protected:

    /*
     * Module initialization
     */
    void initialize();

   /**
    * Module ending.
    */
    void finish();

   /**
    * Process a self message.
    * @param msg Self message.
    */
    void processSelfMessage (cMessage *msg);

   /**
    * Process a request message.
    * @param sm Request message.
    */
    void processRequestMessage (icancloud_Message *sm);

   /**
    * Process a response message.
    * @param sm Request message.
    */
    void processResponseMessage (icancloud_Message *sm);

   /**
    * Get the out Gate to the module that sent <b>msg</b>.
    * @param msg Arrived message.
    * @return Gate (out) to module that sent <b>msg</b> or NULL if gate not found.
    */
    cGate* getOutGate (cMessage *msg);

private:

    /*
     * Migration methods // TODO: Those methods are not done. To the next version of iCAnCloud
     */

    void notifyContainerPreparedToMigrate();
    void finishMigration ();
    void pushMessage(icancloud_Message* sm);
    icancloud_Message* popMessage();
    void sendPendingMessage (icancloud_Message* msg);
    void flushPendingMessages();
    int pendingMessagesSize();
    void updateCounters (icancloud_Message* msg, int quantity);

    /*
     *  Control structure to parallel and distributed applications.
     *  This method storage the communication ID of a message replacing it with
     *  the app id returned from getId() method.
     */
    void insertCommId(int uId, int pId, int commId, int msgId);

    /*
     * This method is invoked by processResponseMessage to substitute the commId by the
     * original befor sending if it was neccesary.
     */
    void updateCommId (icancloud_App_NET_Message* sm);
};

#endif /* DOCKERDAEMON_H_ */
