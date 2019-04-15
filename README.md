# PPM Image Editor 

## Project Summary
The program reads a PPM image file, alter the RGB elements of each pixel, and prints the resulting image as a new PPM file. 

## PPM File Format 
The PPM (or Portable Pix Map) image format is encoded in human-readable ASCII text. The image has two parts: 
- Header (metadata)
- Body (pixel values)

## Project Features 
The project implements following functionalities: 
- **`negate_red` :**  negates the red number of each pixel
- **`negate_green` :** negates the green number of each pixel
- **`negate_blue` :** negates the blue number of each pixel
-  **`flatten_red` :** sets the red value to zero
- **`flatten_green` :** sets the green value to zero
- **`flatten_blue` :** sets the blue value to zero
- **`flip_horizontal` :** flips each row horizontally
- **`grey_scale` :** sets each pixel value to the average of the three
- **`extreme_contrast` :** changes each color number to either the highest color number possible or to 0
- **`blur_horizontal` :** replaces the values of the red numbers of three adjacent pixels with their average
- **`random_noise` :** adds a random number to each color number or subtracts a random number

## User Interface
The program has a simple command line interface.<br />
<img src="assets/interface.PNG" align="center">

## Sample Output 
<center>

| Original Image | Greyscale | 
| ------------- |:-------------:| 
| <img src="assets/cake.jpg" width="300"> | <img src="assets/cake_greyscale.jpg" width="300"> |
| **Flip Horizontal** | **Extreme Contrast** |
| <img src="assets/cake_horizontal.jpg" width="300"> | <img src="assets/cake_contrast.jpg" width="300"> |
| **Blur Horizontal** | **Negate Red** |
| <img src="assets/cake_blur.jpg" width="300"> | <img src="assets/cake_negativered.jpg" width="300"> |
| **Flatten RG** | **Random Noise** |
| <img src="assets/cake_justblue.jpg" width="300"> | <img src="assets/cake_noise.jpg" width="300"> |

</center>
## Reference
Further details about the project can be found [here.](http://nifty.stanford.edu/2012/guerin-image-editor/)




