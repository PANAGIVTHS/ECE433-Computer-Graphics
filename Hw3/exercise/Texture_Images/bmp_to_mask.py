from PIL import Image
import sys

def grayscale_to_red_channel(input_bmp, output_bmp):
    img = Image.open(input_bmp).convert("RGB")
    pixels = img.load()

    width, height = img.size

    for y in range(height):
        for x in range(width):
            r, g, b = pixels[x, y]

            # Standard luminance grayscale (ITU-R BT.601)
            gray = int(0.299 * r + 0.587 * g + 0.114 * b)

            # Store grayscale ONLY in red channel
            pixels[x, y] = (gray, 0, 0)

    img.save(output_bmp, format="BMP")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python gray_to_red.py input.bmp output.bmp")
        sys.exit(1)

    grayscale_to_red_channel(sys.argv[1], sys.argv[2])
