#include "render.h"

int main(int argc,char **argv)
{
    if (argc<8)  // Augmenté à 8 pour inclure le nombre de threads
    {
        fprintf(stderr,"Usage: %s width height samples output_path max_depth render_mode num_threads [chunk_size]\n", argv[0]);
        exit(0);
    }

    double width=atof(argv[1]);
    double height=atof(argv[2]);
    int samples_per_pixel=atoi(argv[3]); //Each pixel is sampled argv[3] times
    char *output_path=argv[4];
    int max_depth=atoi(argv[5]);
    int render_mode=atoi(argv[6]); // Select the renderer type
    int num_threads=atoi(argv[7]); // Nombre de threads à utiliser
    int chunk_size = 1; // Default chunk size

    // If chunk size is provided as an argument, use it
    if (argc > 8) {
        chunk_size = atoi(argv[8]);
    }

    // Set the number of threads to use
    omp_set_num_threads(num_threads);

    //user-defined camera parameters
    //Vec3 origin,lookat,v_up;
    //double v_fov;
    //std::cout<<"Enter camera origin: (x,y,z)"<<std::endl;
    //std::cin>>origin.x>>origin.y>>origin.z;
    //std::cout<<"Enter camera lookat: (x,y,z)"<<std::endl;
    //std::cin>>lookat.x>>lookat.y>>lookat.z;
    //std::cout<<"Enter camera v_up: (x,y,z)"<<std::endl;
    //std::cin>>v_up.x>>v_up.y>>v_up.z;
    //std::cout<<"Enter camera v_fov: "<<std::endl;
    //std::cin>>v_fov;

    Vec3 origin(0,0,2);
    Vec3 lookat(0,0,-1);
    Vec3 v_up(0,-1,0);
    double v_fov=60;

    std::cout<<"Rendering effect is being generated......"<<std::endl;
    std::cout<<"Using " << num_threads << " threads" << std::endl;

    //create a scene
    std::vector<Object*> scene=createScene();
    // std::vector<Object*> scene=createTestScene(); // Use only when testing performance

    // record rendering start time
    auto start_time=std::chrono::high_resolution_clock::now();
    //render the image and output it to a file
    if (render_mode==0)
        render(width,height,scene,output_path,origin,lookat,v_up,v_fov,samples_per_pixel,max_depth, chunk_size);
    else if (render_mode==1)
    {
        int num_iterations;
        std::cout<<"Enter MLT iterations: ";
        std::cin>>num_iterations;
        renderMLT(width,height,scene,output_path,origin,lookat,v_up,v_fov,samples_per_pixel,max_depth,num_iterations);
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