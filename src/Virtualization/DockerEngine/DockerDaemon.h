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
