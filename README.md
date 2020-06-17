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
