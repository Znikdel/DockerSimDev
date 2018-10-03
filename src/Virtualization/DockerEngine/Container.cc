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

#include "Container.h"

//Define_Module(Container);


Container::~Container() {
    states_log.clear();

}

void Container::initialize(){

    // Define ..
    // cModule* osMod;
   //  string ContainerTypeName;
    // Machine::initialize();
     icancloud_Base::initialize();

    // Init ..
     states_log.clear();
     pending_operation = NOT_PENDING_OPS;
     userID = -1;
     nodeName = -1;
     nodeSetName = "";
     containerName = "";
     containerState="";

    // osMod = getSubmodule("osModule")->getSubmodule("syscallManager");
    // os = dynamic_cast<ContainerSyscallManager*>(osMod);
    // Machine::changeState(MACHINE_STATE_OFF);

}

void Container::finish(){

    icancloud_Base::finish();
}

void Container::changeState(string newState){
    containerStatesLog_t* log;
    string oldstate;
    containerState=newState;
    if (states_log.size() == 0 ){

        log = new containerStatesLog_t();
        log->container_state = newState;
        log->init_time_M = simTime().dbl() / 60;

        states_log.push_back(log);
    } else {
       oldstate = (*(states_log.end() -1))->container_state;

       if (strcmp(oldstate.c_str(), newState.c_str()) != 0){
           log = new containerStatesLog_t();
           log->container_state = newState;
           log->init_time_M = simTime().dbl() / 60;

           states_log.push_back(log);
       }
    }


};


void Container::stop_Container(){

	changeState("stopped");

}
void Container::start_Container(){
if (containerState=="stopped")
    {
        changeState("running");
    }


}void Container::create_container(){

    //TO Do
    // take care of image : pulling image
    initialize();

}
void Container::run_Container(){

    changeState("running");

}
void Container::remove_Container(){

    //TO DO
    //remove container resources
    finish();
}

bool Container::isContainerRunning(int pId){
    if (containerState=="running") return true;
    else return false;

}

