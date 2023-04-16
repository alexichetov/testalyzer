# testalyzer
A rough attempt at a keyboard analyzer

pre-requisites: a corpus file named corpus, that contains only ascii characters

compile both `outngram.c` and `analyze.c` using gcc
run `./outngram` on its own first to create an ngram file that analyze can read from
then run `./analyze layouts/*` to get results  for all layouts in the folder

layouts in the folder must be a 3x10 grid of ascii characters. There is no real error handling so if you did something wacky it'll respond with something wacky.

the stats in order will be:
Finger usage, by finger (middle thumb is for space).
Row usage, by row.

SFB - same finger bigrams, two keys on the same finger being pressed consecutively

LSB - press on middle then far index of the same hand

BetterFSB - same hand, press on top row and bottom row, and one of the presses is either the middle of ring finger

SRB - same row bigram, same hand same row


SFS/LSS/BetterSFS/SRS are the same as their B counterbars, except it is two key presses separated by one key press.
