//
//

#ifndef __CONTAINERSINK_H__
#define __CONTAINERSINK_H__

#include <omnetpp.h>
#include "Sink.h"

/**
 * Sink module that displays the number of received jobs.
 */
class ContainerSink : public queueing::Sink
{
  protected:
    int numJobsReceived;

    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

#endif
