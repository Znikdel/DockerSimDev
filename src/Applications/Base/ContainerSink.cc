

#include "ContainerSink.h"

Define_Module(ContainerSink);


void ContainerSink::initialize()
{
    Sink::initialize();
    numJobsReceived = 0;
}

void ContainerSink::handleMessage(cMessage *msg)
{
    Sink::handleMessage(msg);

    char buf[80];
    sprintf(buf, "received jobs: %d", ++numJobsReceived);
    getDisplayString().setTagArg("t",0, buf);
}

