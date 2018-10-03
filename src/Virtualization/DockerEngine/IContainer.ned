// @author Zahra Nikdel
// @date 2018-09-27
//

package icancloud.src.Virtualization.DockerEngine;

moduleinterface IContainer
{
    parameters:
  
        int numCores; // default is the same as VM host but it is possible to have constraints												
        double memorySize_MB;
        int storageSize_GB;  // takes from storage

}