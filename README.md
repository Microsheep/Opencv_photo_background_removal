<h1>Background removal using opencv.</h1><br>

1. photos: image for testing<br>
2. bgremove: code for background removal<br>
(bgremove.out is the executable for linux)<br>

Background Remove: (-h for help)<br>
[-i input] [-o output] [-mo mask_output]<br>
[-l lonely_start lonely_end lonely_times]<br>
[-bw black_white] [-m margin] [-h | help]<br>

Default:<br>
input - in.png<br>
output - out.png<br>
mask_output - mask.out.png<br>
black_white = 20 (20/255 black-white seperate)<br>
lonely_start = 6 (less than 6/25 is black)<br>
lonely_end = 20 (greater than 20/25 is white)<br>
lonely_times = 1 (the times lonely kill is executed)<br>
margin = 2 (the radius killed when flood)<br>
