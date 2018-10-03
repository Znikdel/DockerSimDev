// Module that implements a Server application. 
//
// The parameters have to be used depending of the distribution. 
//
// @author zahra nikdel
// @date 2018-10-02
package icancloud.src.Applications.Apps.DockerApps;

import icancloud.src.Applications.Apps.IAPP;


simple DockerApps like IAPP

{
    parameters:
           string application_netType = default ("INET");		// Network type (INET or BASIC)        
        double startDelay;									// Starting delay time!        
        int inputSize;										// Input size of data
        int outputSize;										// Output size of data
        int MIs;											// Number of MIs to execute
        int iterations = default (0);						// Number of iterations
        @display("i=msg/job");
        
        
    gates:
        input fromOS @loose;								// Input gate from OS (Operating System)
        output toOS @loose;									// Output gate to OS (Operating System)
		
}


