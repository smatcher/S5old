#include "pandaFramework.h"
#include "pandaSystem.h"
 
PandaFramework framework;
 // The global task manager
PT(AsyncTaskManager) taskMgr = AsyncTaskManager::get_global_ptr(); 
// The global clock
PT(ClockObject) globalClock = ClockObject::get_global_clock();
// Here's what we'll store the camera in.
NodePath camera;
 
// This is our task - a global or static function that has to return DoneStatus.
// The task object is passed as argument, plus a void* pointer, cointaining custom data.
// For more advanced usage, we can subclass AsyncTask and override the do_task method.
AsyncTask::DoneStatus spinCameraTask(GenericAsyncTask* task, void* data) {
  // Calculate the new position and orientation (inefficient - change me!)
  double time = globalClock->get_real_time();
  double angledegrees = time * 6.0;
  double angleradians = angledegrees * (3.14 / 180.0);
  camera.set_pos(20*sin(angleradians),-20.0*cos(angleradians),3);
  camera.set_hpr(angledegrees, 0, 0);
 
  // Tell the task manager to continue this task the next frame.
  return AsyncTask::DS_cont;
}
 
int main(int argc, char *argv[]) {
    //open a new window framework
  framework.open_framework(argc, argv);
    //set the window title to My Panda3D Window
  framework.set_window_title("My personnal Panda3D Window");
    //open the window
  WindowFramework *window = framework.open_window();
  camera = window->get_camera_group(); 

  //here is room for your own code
  NodePath model = window->load_model(framework.get_models(),"../data/test");
  model.reparent_to(window->get_render());
//  model.set_scale(0.25,0.25,0.25);
  model.set_pos(-8,42,0);
	window->setup_trackball();

  taskMgr->add(new GenericAsyncTask("Spins the camera", &spinCameraTask, (void*)NULL));

    //do the main loop, equal to run() in python
  framework.main_loop();
    //close the window framework
  framework.close_framework();
  return (0);
}

