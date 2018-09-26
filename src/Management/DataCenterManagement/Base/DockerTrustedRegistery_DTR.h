
/* This module communicate with docker daemon using docker API
 * it also manages replications and keep track of images on different machines
 *
 * create image
 * remove image
 * recieve requests over port 80 and 443
 * push images
 * pull images
 * replicate image
 * getimagesids on a specific machine
 * put image on a specific machine
 * get machine id of a specific image
 * delete user and its repositories: if image has only one user id then delete it
 *
 * if you delete the image owner then notify all other users using it and delete the image
 * if you are deleting a user then clear all vectors from that user id and keep image as long as image owner exists
 *
 *
 */
