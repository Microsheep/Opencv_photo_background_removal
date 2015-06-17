Background removal using opencv.

1. photos: image for testing
2. bgremove: code for background removal
(bgremove.out is the executable for linux)

Background Remove: (-h for help) 
[-i input] [-o output] [-mo mask_output]
[-l lonely_start lonely_end lonely_times]
[-bw black_white] [-m margin] [-h | help]

Default:
input - in.png
output - out.png
mask_output - mask.out.png
black_white = 20 (20/255 black-white seperate)
lonely_start = 6 (less than 6/25 is black)
lonely_end = 20 (greater than 20/25 is white)
lonely_times = 1 (the times lonely kill is executed)
margin = 2 (the radius killed when flood)
