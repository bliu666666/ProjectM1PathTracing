#include "render.h"

int main(int argc,char **argv)
{
    if (argc<7)
    {
        fprintf(stderr,"Usage:need 6 arguments\n");
        exit(0);
    }
    int num_procs = omp_get_num_procs(); // Get the number of available CPU cores
    omp_set_num_threads(num_procs);      // Set the number of threads to the number of cores

    double width=atof(argv[1]);
    double height=atof(argv[2]);
    int samples_per_pixel=atoi(argv[3]); //Each pixel is sampled argv[3] times
    char *output_path=argv[4];
    int max_depth=atoi(argv[5]);
    int render_mode=atoi(argv[6]); // Select the renderer type

    //user-defined camera parameters
    Vec3 origin,lookat,v_up;
    double v_fov;
    std::cout<<"Enter camera origin: (x,y,z)"<<std::endl;
    std::cin>>origin.x>>origin.y>>origin.z;
    std::cout<<"Enter camera lookat: (x,y,z)"<<std::endl;
    std::cin>>lookat.x>>lookat.y>>lookat.z;
    std::cout<<"Enter camera v_up: (x,y,z)"<<std::endl;
    std::cin>>v_up.x>>v_up.y>>v_up.z;
    std::cout<<"Enter camera v_fov: "<<std::endl;
    std::cin>>v_fov;

    std::cout<<"Rendering effect is being generated......"<<std::endl;

    //create a scene
    std::vector<Object*> scene=createScene();

    // record rendering start time
    auto start_time=std::chrono::high_resolution_clock::now();
    //render the image and output it to a file
    if (render_mode==0)
        render(width,height,scene,output_path,origin,lookat,v_up,v_fov,samples_per_pixel,max_depth);
    else if (render_mode==1)
    {
        renderMLT(width,height,scene,output_path,origin,lookat,v_up,v_fov,samples_per_pixel,max_depth);
        //renderMLT_DebugSinglePixel(width, height, scene, origin, lookat, v_up, v_fov, samples_per_pixel, max_depth);
    }
    else
    {
        std::cerr<<"Invalid render mode.Use 0 for normal render or 1 for MLT."<< std::endl;
        exit(1);
    }
     // Record rendering end time
    auto end_time=std::chrono::high_resolution_clock::now();
    // Calculate rendering time
    std::chrono::duration<double> elapsed=end_time-start_time;

    // Output rendering time
    std::cout<<"Render complete. Time taken: "<<elapsed.count()<<" seconds."<< std::endl;
    std::cout<<"Render complete.Please check the outputfile: "<<output_path<<std::endl;
    return 0;
}