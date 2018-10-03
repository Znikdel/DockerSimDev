#include "DockerApps.h"
#define INPUT_FILE "/input.dat"
#define OUTPUT_FILE "/output.dat"
#define MAX_FILE_SIZE 2000000000000

Define_Module(DockerApps);

DockerApps::~DockerApps(){
}


void DockerApps::initialize(){

    std::ostringstream osStream;
    timeoutEvent = NULL;
    timeout = 1.0;

    // Set the moduleIdName
    cout<<"DockerApps::initialize()"<<endl;
    //printf("DockerApps.");
    osStream << "DockerApps." << getId();
    moduleIdName = osStream.str();

    // Init the super-class
    UserJob::initialize();

    // App Module parameters
    startDelay = par ("startDelay");
    inputSizeMB  = par ("inputSize");
    outputSizeMB  = par ("outputSize");
    MIs  = par ("MIs");

    iterations  = par ("iterations");
    currentIteration = 1;

    // Service times
    total_service_IO = 0.0;
    total_service_CPU = 0.0;
    startServiceIO = 0.0;
    endServiceIO = 0.0;
    startServiceCPU = 0.0;
    endServiceCPU = 0.0;
    readOffset = writeOffset = 0;

    // Boolean variables
    executeCPU = executeRead = executeWrite = false;

    // Assign names to the results
    jobResults->newJobResultSet("totalIO");
    jobResults->newJobResultSet("totalCPU");
    jobResults->newJobResultSet("Real run-time");
    jobResults->newJobResultSet("Simulation time");

}


void DockerApps::finish(){
    // Finish the super-class
    cout<<"DockerApps::finish   START"<<endl;
    cout<<endl;

   UserJob::finish();

   cout<<"DockerApps::finish   END"<<endl;


}

void DockerApps::startExecution (){

    cout<<"DockerApps::startExecution   START"<<endl;

    API_OS::startExecution();

    Enter_Method_Silent();
    // Create SM_WAIT_TO_EXECUTE message for delaying the execution of this application
    cMessage *waitToExecuteMsg = new cMessage (SM_WAIT_TO_EXECUTE.c_str());
    scheduleAt (simTime()+startDelay, waitToExecuteMsg);

    cout<<"DockerApps::startExecution   END"<<endl;

}

void DockerApps::processSelfMessage (cMessage *msg){
    cout<<"DockerApps::processSelfMessage   START"<<endl;

    if (!strcmp (msg->getName(), SM_WAIT_TO_EXECUTE.c_str())){

        // Starting time...
        simStartTime = simTime();
        runStartTime = time (NULL);

        // Init...
        startServiceIO = simTime();

        executeIOrequest (false, true);

    }else

        showErrorMessage ("Unknown self message [%s]", msg->getName());

    delete (msg);
    cout<<"DockerApps::processSelfMessage   END"<<endl;

}


void DockerApps::processRequestMessage (icancloud_Message *sm){

}


void DockerApps::processResponseMessage (icancloud_Message *sm){
    cout<<"DockerApps::processResponseMessage   START"<<endl;

    icancloud_App_IO_Message *sm_io;
    icancloud_App_CPU_Message *sm_cpu;
    bool isError;
    std::ostringstream osStream;
    int operation;


        // Init...
        operation = sm->getOperation ();
        sm_io = dynamic_cast<icancloud_App_IO_Message *>(sm);
        sm_cpu = dynamic_cast<icancloud_App_CPU_Message *>(sm);
        isError = false;


        // IO Message?
        if (sm_io != NULL){

            // Get time!
            endServiceIO = simTime();

            // Read response!
            if (operation == SM_READ_FILE){
                // All ok!
                if (sm_io->getResult() == icancloud_OK){
                    executeCPU = true;
                    executeWrite = false;
                    executeRead = false;
                    delete (sm);
                }

                // File not found!
                else if (sm_io->getResult() == icancloud_FILE_NOT_FOUND){
                    osStream << "File not found!";
                    isError = true;
                }

                // File not found!
                else if (sm_io->getResult() == icancloud_DATA_OUT_OF_BOUNDS){
                    executeCPU = true;
                }

                // Unknown result!
                else{
                    osStream << "Unknown result value:" << sm_io->getResult();
                    isError = true;
                }
            }


            // Write response!
            else if (operation == SM_WRITE_FILE){
                // All ok!
                if (sm_io->getResult() == icancloud_OK){
                    executeCPU = false;
                    executeWrite = false;
                    executeRead = true;
                    currentIteration++;
                    delete (sm);
                }

                // File not found!
                else if (sm_io->getResult() == icancloud_FILE_NOT_FOUND){
                    osStream << "File not found!";
                    isError = true;
                }

                // File not found!
                else if (sm_io->getResult() == icancloud_DISK_FULL){
                    osStream << "Disk full!";
                    isError = true;
                }

                // Unknown result!
                else{
                    osStream << "Unknown result value:" << sm_io->getResult();
                    isError = true;
                }
            }

            // Unknown I/O operation
            else{
                osStream << "Unknown received response message";
                isError = true;
            }

            // Increase total time for I/O
            total_service_IO += (endServiceIO - startServiceIO);
        }


        // Response came from CPU system
        else if (sm_cpu != NULL){

            // Get time!
            endServiceCPU = simTime ();

            // CPU!
            if (operation == SM_CPU_EXEC){
                executeCPU = false;
                executeWrite = true;
                executeRead = false;
                delete (sm);
            }

            // Unknown CPU operation
            else{
                osStream << "Unknown received response message";
                isError = true;
            }

            // Increase total time for I/O
            total_service_CPU += (endServiceCPU - startServiceCPU);
        }

        // Wrong response message!
        else{

            osStream << "Unknown received response message";
            isError = true;
        }

            // Error?
            if (isError){

                showErrorMessage ("Error in response message:%s. %s",
                                    osStream.str().c_str(),
                                    sm_io->contentsToString(true).c_str());
            }

            // CPU?
            else if (executeCPU){

                // Execute CPU!
                executeCPUrequest ();
            }

            // IO?
            else if ((executeRead) || (executeWrite)){

                if ((executeRead) && (currentIteration > iterations))
                    printResults();
                else
                    executeIOrequest(executeRead, executeWrite);
            }

            // Inconsistency error!
            else
                showErrorMessage ("Inconsistency error!!!! :%s. %s",
                                    osStream.str().c_str(),
                                    sm->contentsToString(true).c_str());


            cout<<"DockerApps::processResponseMessage   END"<<endl;

}


void DockerApps::changeState(string newState){

}


void DockerApps::executeIOrequest(bool executeRead, bool executeWrite){
    cout<<"DockerApps::executeIOrequest   START"<<endl;

    // Reset timer!
    startServiceIO = simTime();

    // Executes I/O operation
    if (executeRead){

        if ((readOffset+(inputSizeMB*MB))>=MAX_FILE_SIZE)
            readOffset = 0;

        if (DEBUG_Application)
            showDebugMessage ("[%d/%d] Executing (Read) Offset:%d; dataSize:%d", currentIteration, iterations, readOffset,  inputSizeMB*MB);

        icancloud_request_read (INPUT_FILE, readOffset, inputSizeMB*MB);
        readOffset += (inputSizeMB*MB);


    }
    else{

        if ((writeOffset+(outputSizeMB*MB))>=MAX_FILE_SIZE)
            writeOffset = 0;

        if (DEBUG_Application)
            showDebugMessage ("[%d/%d] Executing (Write) Offset:%d; dataSize:%d", currentIteration, iterations, writeOffset,  outputSizeMB*MB);

        icancloud_request_write (OUTPUT_FILE, writeOffset, outputSizeMB*MB);
        writeOffset += (outputSizeMB*MB);
    }


    cout<<"DockerApps::executeIOrequest   END"<<endl;

}


void DockerApps::executeCPUrequest(){
    cout<<"DockerApps::executeCPUrequest   START"<<endl;

    // Debug?
    if (DEBUG_Application)
        showDebugMessage ("[%d/%d] Executing (CPU) MIs:%d", currentIteration, iterations, MIs);

    // Reset timer!
    startServiceCPU = simTime ();
    icancloud_request_cpu (MIs);


    cout<<"DockerApps::executeCPUrequest   END"<<endl;

}


void DockerApps::printResults (){
    cout<<"DockerApps::printResults   START"<<endl;

    std::ostringstream osStream;

    //Init..
        simEndTime = simTime();
        runEndTime = time (NULL);

    //Assign values to the results
        osStream <<  total_service_IO.dbl();
        jobResults->setJobResult(0, osStream.str());
        osStream.str("");

        osStream <<  total_service_CPU.dbl();
        jobResults->setJobResult(1, osStream.str());
        osStream.str("");

        osStream <<  difftime (runEndTime,runStartTime);
        jobResults->setJobResult(2, osStream.str());
        osStream.str("");

        osStream << (simEndTime - simStartTime).dbl();
        jobResults->setJobResult(3, osStream.str());

        addResults(jobResults);

    //Send results list to the cloudManager
        userPtr->notify_UserJobHasFinished(this);


        cout<<"DockerApps::printResults   END"<<endl;

}


