
## Profiling with gprof via Docker

We use scripts that automate the entire profiling process across different platforms :

### Linux / macOS

```bash
# Execute the profiling script
./profiling.sh
```

### Windows (CMD)

```cmd
profiling.bat
```


This script will automatically :
1. Create a Docker container with x86 architecture
2. Compile your code with profiling flags
3. Run the program with predefined camera parameters
4. Generate a detailed report with gprof
5. Display the 10 most expensive functions

### Parameters used

By default, the script uses the following parameters of a cornell Box :

```
./render 1080 720 100 output.ppm 10
```

- **1080** : width of the image in pixels
- **720** : height of the image in pixels
- **100** : number of samples per pixel
- **output.ppm** : output file
- **10** : maximum depth of ray reflection


