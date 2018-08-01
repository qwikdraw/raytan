ffmpeg -framerate 30 -f image2 -i ./render/%04d.png -c:v libvpx-vp9 -pix_fmt yuva420p -pass 1 -b:v 0 -crf 33 -threads 4 -speed 4 -tile-columns 6 -frame-parallel 1 -an -f webm /dev/null

ffmpeg -framerate 30 -f image2 -i ./render/%04d.png -c:v libvpx-vp9 -pix_fmt yuva420p -vf scale=1024:-1 -pass 2 -b:v 0 -crf 18 -threads 4 -speed 2 -tile-columns 6 -frame-parallel 1 -auto-alt-ref 1 -lag-in-frames 25 -b:a 64k -f webm out.webm
