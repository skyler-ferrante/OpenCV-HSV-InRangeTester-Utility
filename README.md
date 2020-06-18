# OpenCV-HSV-InRangeTester-Utility
Test if your HSV threshold values work on multiple images.
Make sure you have the OpenCV library. Then you can build with:

    $ cmake . 
    $ make

Then to get images either run:

    $./PHOTO_MANAGER

or provide your own images and put the file names in images/list.txt.

Then add your HSV values in PhotoAnalyzer and run with: 

    $./PHOTO_ANALYZER

to test the given HSV values.

PhotoAnalyzer will blur each image, convert them to hsv, and then run inRange on them. It will then find all contours in the image and put bounding boxes around them. Then it will give you information on each bounding box so you can tell if the HSV values you gave were correct for the object you were trying to filter for. If you have the DEBUG flag on in PhotoAnalyzer.cpp (which is on by defualt) then you will see the filtered image and the bounding box while running.
