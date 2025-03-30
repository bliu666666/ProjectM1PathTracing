#include "render.h"

int main(int argc,char **argv)
{
    if (argc!=6)
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

    //create a scene
    std::vector<Object*> scene=createScene();
    // record rendering start time
    auto start_time=std::chrono::high_resolution_clock::now();
    //render the image and output it to a file
    render(width,height,scene,output_path,origin,lookat,v_up,v_fov,samples_per_pixel,max_depth);
     // Record rendering end time
    auto end_time=std::chrono::high_resolution_clock::now();
    // Calculate rendering time
    std::chrono::duration<double> elapsed=end_time-start_time;

    // Output rendering time
    std::cout<<"Render complete. Time taken: "<<elapsed.count()<<" seconds."<< std::endl;
    std::cout<<"Render complete.Please check the outputfile: "<<output_path<<std::endl;
    return 0;
}