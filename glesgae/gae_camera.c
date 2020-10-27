#include "gae_camera.h"

gae_camera_t* gae_camera_init(gae_camera_t* camera, gae_point_2d_t viewDims, gae_point_2d_t portDims)
{
	camera->view.x = 0;
	camera->view.y = 0;
	camera->view.w = (int)viewDims.x;
	camera->view.h = (int)viewDims.y;
	
	camera->port.x = 0;
	camera->port.y = 0;
	camera->port.w = (int)portDims.x;
	camera->port.h = (int)portDims.y;
	
	return camera;
}

gae_camera_t* gae_camera_move(gae_camera_t* camera, gae_point_2d_t delta)
{
	camera->view.x += (int)delta.x;
	camera->view.y += (int)delta.y;
	
	return camera;
}

gae_camera_t* gae_camera_position(gae_camera_t* camera, gae_point_2d_t absolute)
{
	camera->view.x = (int)absolute.x;
	camera->view.y = (int)absolute.y;
	
	return camera;
}

gae_camera_t* gae_camera_destroy(gae_camera_t* camera)
{
	return camera;
}
