/*
 * Services allow you to scale containers across multiple Docker daemons,
 * which all work together as a swarm with multiple managers and workers.
 * Each member of a swarm is a Docker daemon,
 * and the daemons all communicate using the Docker API.
 * A service allows you to define the desired state,
 * such as the number of replicas of the service that must be available at any given time.
 * By default, the service is load-balanced across all worker nodes.
 * To the consumer, the Docker service appears to be a single application.
 */
