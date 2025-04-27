from PIL import Image
import sys
import os

def convert_ppm_to_png(input_path, output_path=None):
    try:
        # Si aucun chemin de sortie n'est spécifié, utiliser le même nom avec extension .png
        if output_path is None:
            output_path = os.path.splitext(input_path)[0] + '.png'

        # Ouvrir et convertir l'image
        with Image.open(input_path) as img:
            img.save(output_path, 'PNG')
        print(f"Conversion réussie : {input_path} -> {output_path}")
        
    except Exception as e:
        print(f"Erreur lors de la conversion : {str(e)}")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python convert_ppm.py <input_ppm_file> [output_png_file]")
        sys.exit(1)

    input_file = sys.argv[1]
    output_file = sys.argv[2] if len(sys.argv) > 2 else None

    convert_ppm_to_png(input_file, output_file) 