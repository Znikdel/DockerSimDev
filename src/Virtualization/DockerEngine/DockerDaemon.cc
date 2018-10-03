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

#include "DockerDaemon.h"

Define_Module(DockerDaemon);


DockerDaemon::~DockerDaemon() {
    // TODO Auto-generated destructor stub
}
DockerDaemon::DockerDaemon() {
   initialize();
}
void DockerDaemon::blockMessages (){
    migrateActive = true;
}

void DockerDaemon::activateMessages(){
    migrateActive = false;
    flushPendingMessages();
}

bool DockerDaemon::migrationPrepared (){
    return  ( ( pendingCPUMsg == 0 ) &&
              ( pendingIOMsg == 0 ) &&
              ( pendingNetMsg == 0 ) &&
              ( pendingMemoryMsg == 0 )
            );
}

void DockerDaemon::initialize(){
    cout<<"DockerDaemon::initialize"<<endl;

    icancloud_Base::initialize();

        std::ostringstream osStream;

        migrateActive = false;
        pendingMessages.clear();
        pendingCPUMsg = 0;
        pendingNetMsg = 0;
        pendingMemoryMsg = 0;
        pendingIOMsg = 0;
        uId = -1;
        pId = -1;
        conId=-1;

        // Init state to idle!

        fromOSApps  = new cGateManager(this);
        toOSApps = new cGateManager(this);
        fromApps = new cGateManager(this);
        toApps = new cGateManager(this);

}

void DockerDaemon::finish(){

}

void DockerDaemon::processSelfMessage (cMessage *msg){
    delete (msg);
    std::ostringstream msgLine;
    msgLine << "Unknown self message [" << msg->getName() << "]";
    throw cRuntimeError(msgLine.str().c_str());
}

void DockerDaemon::processRequestMessage (icancloud_Message *msg){

  //  cout<< "DockerDaemon::processRequestMessage"<<endl;
    icancloud_App_NET_Message *sm_net;
    int operation;

    sm_net = dynamic_cast<icancloud_App_NET_Message *>(msg);

    operation = msg->getOperation();
  //  cout<<"operation->"<<operation<<endl;
    if (operation == SM_STOP_AND_DOWN_CONTAINER) {

        if (!migrateActive){
            blockMessages();
            sm_net->setIsResponse(true);
            migration_msg = msg;

            if (migrationPrepared()){
                notifyContainerPreparedToMigrate();
            }

        }
        else delete (msg);

    }

    else if (((int)msg->getOperation()) == SM_CONTAINER_ACTIVATION) {

        finishMigration();
        delete (msg);
    }

    else {

        // Set the id of the message (the vm id)
        msg->setPid(pId);
        msg->setUid(uId);
    //    msg->setConid(conId);

        // Set as application id the arrival gate id (unique per job).
              if ((sm_net != NULL) && (sm_net->getCommId() != -1))
                  insertCommId (uId, pId, msg->getCommId(), msg->getId());


        if (migrateActive){
            pushMessage(msg);  // push msg to pending msgs
        }
        else {

            // If msg arrive from OS
            if (msg->arrivedOn("fromOSApps")){
                cout<<"fromOSApps->"<<endl;

                sendRequestMessage(msg, toApps->getGate(msg->getArrivalGate()->getIndex()));
            }

            else if (msg->arrivedOn("fromApps")){
          //      cout<<"fromApps->"<<endl;

                updateCounters(msg,1);
          //      cout<<"updateCounters->"<<endl;

                 msg->setCommId(msg->getArrivalGate()->getIndex());
           //      cout<<"setCommId->"<<endl;
//
                if (sm_net != NULL){
                    // If the message is a net message and the destination user is null
                    if (sm_net->getVirtual_user() == -1){
                        // Filter the name of the user
                        sm_net->setVirtual_user(msg->getUid());
                    }

                    sm_net->setVirtual_destinationIP(sm_net->getDestinationIP());
               //   sm_net->setVirtual_destinationContainerIP(sm_net->getDestinationIP());

                    sm_net->setVirtual_destinationPort(sm_net->getDestinationPort());
                    sm_net->setVirtual_localIP(sm_net->getLocalIP());
                    sm_net->setVirtual_localPort(sm_net->getLocalPort());
                }
            //    cout<<"set ports->"<<endl;
           //     cout<<"toOSApps->getGate(msg->getCommId())"<<msg->getCommId()<<endl;
//
                sendRequestMessage(msg, toOSApps->getGate(msg->getCommId()));

              //  sendRequestMessage(msg, toOSApps->getGate(0));
            }

}

    }
}

void DockerDaemon::processResponseMessage (icancloud_Message *sm){

        // If msg arrive from OS

    updateCounters(sm,-1);

 /*   if (migrateActive){
        pushMessage(sm);
        if (migrationPrepared()){
            notifyContainerPreparedToMigrate();
        }
    } else {*/

        icancloud_App_NET_Message *sm_net;
        sm_net = dynamic_cast<icancloud_App_NET_Message *>(sm);

        if (sm_net != NULL){
            updateCommId(sm_net);
        }

        sendResponseMessage(sm);
   // }
}

cGate* DockerDaemon::getOutGate (cMessage *msg){

    // Define ..
        cGate* return_gate;
        int i;
        bool found;

    // Initialize ..
        i = 0;
        found = false;

        // If msg arrive from OS
        if (msg->arrivedOn("fromOSApps")){

            while ((i < gateCount()) && (!found)){
                if (msg->arrivedOn ("fromOSApps", i)){
                    return_gate = (gate("toOSApps", i));
                    found = true;
                }
                i++;
            }

        }

        else if (msg->arrivedOn("fromApps")){

            while ((i < gateCount()) && (!found)){
                if (msg->arrivedOn ("fromApps", i)){
                    return_gate = (gate("toApps", i));
                    found = true;
                }
                i++;
            }

        }

        return return_gate;
}

void DockerDaemon::notifyContainerPreparedToMigrate(){
    sendResponseMessage(migration_msg);
}

void DockerDaemon::finishMigration (){
    migrateActive = false;
    flushPendingMessages();
}

void DockerDaemon::pushMessage(icancloud_Message* sm){

    pendingMessages.insert(pendingMessages.end(),sm);

}

icancloud_Message* DockerDaemon::popMessage(){
    vector<icancloud_Message*>::iterator msgIt;

    msgIt = pendingMessages.begin();
    pendingMessages.erase(msgIt);

    return (*msgIt);

}

void DockerDaemon::sendPendingMessage (icancloud_Message* msg){
    int smIndex;

    // The message is a Response message
    if (msg->getIsResponse()) {
        sendResponseMessage(msg);
    }

    // The message is a request message
    else {
        smIndex = msg->getArrivalGate()->getIndex();
        if (msg->arrivedOn("fromOSApps")){
            sendRequestMessage(msg, toApps->getGate(smIndex));
        }

        else if (msg->arrivedOn("fromApps")){
            updateCounters(msg,1);
            sendRequestMessage(msg, toOSApps->getGate(smIndex));
        }

    }
}

void DockerDaemon::flushPendingMessages(){

    // Define ..
    vector<icancloud_Message*>::iterator msgIt;

    // Extract all the messages and send to the destinations

    while (!pendingMessages.empty()){
        msgIt = pendingMessages.begin();

        sendPendingMessage((*msgIt));
        pendingMessages.erase(pendingMessages.begin());
    }

}

int DockerDaemon::pendingMessagesSize(){
    return pendingMessages.size();
}

void DockerDaemon::updateCounters (icancloud_Message* msg, int quantity){
    icancloud_App_CPU_Message* cpuMsg;
    icancloud_App_IO_Message* ioMsg;
    icancloud_App_MEM_Message* memMsg;
    icancloud_App_NET_Message* netMsg;

    cpuMsg = dynamic_cast<icancloud_App_CPU_Message*>(msg);
    ioMsg = dynamic_cast<icancloud_App_IO_Message*>(msg);
    memMsg = dynamic_cast<icancloud_App_MEM_Message*>(msg);
    netMsg = dynamic_cast<icancloud_App_NET_Message*>(msg);

    if (cpuMsg != NULL){
        pendingCPUMsg += quantity;
    }
    else if (ioMsg != NULL){
        pendingIOMsg += quantity;
    }
    else if (memMsg != NULL){
        pendingMemoryMsg += quantity;
    }
    else if (netMsg != NULL){
        pendingNetMsg += quantity;
    }
}

void DockerDaemon::linkNewContainer(cModule* conAppModule, cGate* scToApp, cGate* scFromApp){

    cout<<"DockerDaemon::linkNewContainer"<<endl;

    // Connections to App
       int idxToApps = toApps->newGate("toApps");
    //   toApps->connectOut(jobAppModule->gate("fromOS"), idxToApps);
       toApps->connectOut(conAppModule->gate("fromOS"), idxToApps);
       int idxFromApps = fromApps->newGate("fromApps");
       fromApps->connectIn(conAppModule->gate("toOS"), idxFromApps);

   // Connections to VMmsgController
       int idxToOs = toOSApps->newGate("toOSApps");
       toOSApps->connectOut(scFromApp, idxToOs);

       int idxFromOS = fromOSApps->newGate("fromOSApps");
       fromOSApps->connectIn(scToApp, idxFromOS);

}

int DockerDaemon::unlinkContainer(cModule* conAppModule){

    cout<<"DockerDaemon::unlinkContainer"<<endl;
    int gateIdx = conAppModule->gate("fromOS")->getPreviousGate()->getId();
    int position = toApps->searchGate(gateIdx);
    cout<<"unlinkContainer-->position--->"<<position<<endl;

        toOSApps->freeGate(position);
        toApps->freeGate(position);

   // Connections to VMmsgController
        fromApps ->freeGate(position);
        fromOSApps ->freeGate(position);

        conAppModule->gate("fromOS")->disconnect();
        conAppModule->gate("toOS")->disconnect();

   return position;

}
cGateManager* DockerDaemon::getmyGate(string gateName)
{
    cout <<"DockerDaemon::getmyGate--->"<<gateName<<endl;
    if (gateName.compare("fromOSApps")==0)
        {
        cout <<"fromOSApps inside if--->"<<endl;

            return fromOSApps;
        }
    else if (gateName.compare("fromApps")==0)return fromApps;
    else if (gateName.compare("toOSApps")==0)return toOSApps;
    else if (gateName.compare("toApps")==0)return toApps;
    else return fromOSApps;




}
cGate* DockerDaemon::myNewGate(string gateName){
    cout <<"DockerDaemon::myNewGate--->"<<gateName<<endl;
        if (gateName.compare("fromOSApps")==0)
            {
            cout <<"fromOSApps inside if--->"<<endl;

                int idx=fromOSApps->newGate("fromOSApps");
                return fromOSApps->getGate(idx);
            }
        else if (gateName.compare("fromApps")==0) {
            cout <<"fromApps inside if--->"<<endl;

                int idx=fromApps->newGate("fromApps");
                return fromApps->getGate(idx);
            }
        else if (gateName.compare("toOSApps")==0) {
            cout <<"toOSApps inside if--->"<<endl;

                int idx=toOSApps->newGate("toOSApps");
                return toOSApps->getGate(idx);
            }
        else if (gateName.compare("toApps")==0) {
            cout <<"toApps inside if--->"<<endl;

                int idx=toApps->newGate("toApps");
                return toApps->getGate(idx);
            }
        else {
            throw cRuntimeError("There is no such a gate in docker daemon");
            }

}

void DockerDaemon::setId(int userId, int vmId){
    uId = userId;
    pId = vmId;
  //  conId=contId;

}


void DockerDaemon::insertCommId(int uId, int pId, int commId, int msgId){

    // Define ..
        bool found;
        commIdVector* comm;
        commIdVectorInternals* internals;

    // Initialize ..
        found = false;

    // Search at structure if there is an entry for the same VM
        for (int i = 0; (i < (int)commVector.size()) && (!found); i++){

            // The VM exists
            if ( ((*(commVector.begin() + i))->uId == uId) && ((*(commVector.begin() + i))->pId == pId) ){

                // Create the new entry
                    internals = new commIdVectorInternals();
                    internals -> msgId = msgId;
                    internals -> commId = commId;

                // Add the new entry to the structure
                    (*(commVector.begin() + i))->internals.push_back(internals);

                found = true;      // break the loop
            }
        }

    // There is no entry for the vector..-
        if (!found){
            // Create the general entry
                comm = new commIdVector();
                comm->uId = uId;
                comm->pId = pId;
                comm->internals.clear();

            // Create the concrete entry for the message
                internals = new commIdVectorInternals();

                internals -> msgId = msgId;
                internals -> commId = commId;
                comm->internals.push_back(internals);

                commVector.push_back(comm);
        }
}

void DockerDaemon::updateCommId (icancloud_App_NET_Message* sm){
    // Define ..
        bool found;
        commIdVector* comm;
        commIdVectorInternals* internals;

        // Initialize ..
            found = false;

        for (int i = 0; i < (int)commVector.size(); i++){
            comm = (*(commVector.begin() + i));
            for (int j = 0; (j < (int)comm->internals.size()) && (!found); j++){
                internals = (*(comm->internals.begin() + j));
            }
        }

        for (int i = 0; (i< (int)commVector.size()) && (!found); i++){

            comm = (*(commVector.begin() + i));

            if ( ( sm->getUid() == comm->uId ) && ( sm->getPid() == comm->pId )
               ){

                for (int j = 0; (j < (int)comm->internals.size()) && (!found); j++){

                    internals = (*(comm->internals.begin() + j));

                    if ( internals->msgId ==  sm->getId() ){
                        sm->setCommId(internals->commId);
                        comm->internals.erase(comm->internals.begin() + j);
                        found = true;
                    }
                }

                if ((found) && ((int)comm->internals.size() == 0)){
                    commVector.erase (commVector.begin() + i);
                }

            }
        }
}
