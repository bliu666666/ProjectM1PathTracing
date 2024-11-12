import sys
from PIL import Image

def ppm_to_image(ppm_path,output_path):
    with open(ppm_path,'r') as f:
        #read the header information of the PPM file
        format=f.readline().strip()
        if format!='P3':
            raise ValueError("Not a P3 PPM file")
        width,height=map(int,f.readline().strip().split())
        max_color=int(f.readline().strip())
        #read pixel data
        pixels=[]
        for line in f:
            pixels.extend(map(int,line.split()))
        #convert pixel data to image
        image=Image.new("RGB",(width, height))
        image.putdata([(pixels[i],pixels[i+1],pixels[i+2]) for i in range(0,len(pixels),3)])
        #save the image
        image.save(output_path)

if __name__=="__main__":
    if len(sys.argv)!=3:
        print("Usage: python convert_ppm.py input.ppm output.png")
    else:
        ppm_to_image(sys.argv[1], sys.argv[2])