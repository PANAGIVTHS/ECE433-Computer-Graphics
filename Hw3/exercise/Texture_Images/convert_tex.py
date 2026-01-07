import os
from PIL import Image

def convert_images_to_bmp():
    # Look for both PNG and JPG extensions (case insensitive)
    extensions = ('.png', '.jpg', '.jpeg')
    files = [f for f in os.listdir('.') if f.lower().endswith(extensions)]

    if not files:
        print("No .png or .jpg files found in this folder!")
        return

    print(f"Found {len(files)} images. Converting to BMP...")

    for filename in files:
        try:
            # 1. Open the image
            img = Image.open(filename)
            
            # 2. Convert to RGB 
            # This handles JPEGs (already RGB) and PNGs (removes Transparency/Alpha)
            # Standard BMP does not support transparency well.
            img = img.convert('RGB')

            # 3. Flip Vertically for OpenGL?
            # OpenGL coordinates start at bottom-left. 
            # If your textures appear upside down in-game, uncomment the line below:
            # img = img.transpose(Image.FLIP_TOP_BOTTOM)

            # 4. Create new filename
            name_without_ext = os.path.splitext(filename)[0]
            new_filename = name_without_ext + ".bmp"

            # 5. Save
            img.save(new_filename)
            
            print(f"[OK] {filename} -> {new_filename}")

        except Exception as e:
            print(f"[ERROR] Failed {filename}: {e}")

if __name__ == "__main__":
    convert_images_to_bmp()
    input("\nConversion finished. Press Enter to exit...")