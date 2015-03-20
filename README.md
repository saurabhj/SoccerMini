# SoccerMini
Repository for Soccer Mini

##A little History
Soccer Mini (or Foosball) was a game that we wrote way back in 2005 in C++ for a couple of Game Development contests in engineering colleges around Pune, India <brag>(we won all of them)</brag>

Found the code lying around GMail (as third year students, we did not have access to version control systems back then and often mailed files back and forth) - and decided to publish this on Github under an MIT license.

##Files

###src/FOOSBALL.CPP
Contains all the code of the game. We know it is monolithic and we apologize (we were just kids then).

###src/MOUSE.H
Contains many mouse based functions used by the game. As you probably know, you had to do this to get Mouse access on DOS based C++ projects.

*You can plug this into any project and get access to mouse methods.*

###src/MUSIC.H
This contains music in the game played on the mono PC speaker (when normal speakers that we see nowadays were quite optional).

We reverse-engineered Nokia Ringtones (don't remember what they are called at the moment) and written a converter to simulate this for our game.

The trick was to play specific frequencies - for specific durations in the background using interrupts so as to not have the game stop while the music is playing.

###src/CONFIG.INI
This contains the location of the VGA drivers (EGAVGA.bgi) which ship with Turbo C 3.0

We cannot bundle those here for obvious reasons - but that is required to get this to work.
[Google is your friend](https://www.google.co.in/search?q=download+egavga.bgi).

##Screenshots
Some screens from the game.

![alt text](https://github.com/saurabhj/SoccerMini/blob/master/screens/01_main_menu.png "Main Menu of the Game")

**Main Menu of the Game**

![alt text](https://github.com/saurabhj/SoccerMini/blob/master/screens/02_ai_level.png "Choose your level of AI")

**Choose your level of AI**

![alt text](https://github.com/saurabhj/SoccerMini/blob/master/screens/03_game.png "The Actual Soccer Mini Game")

**The Actual Soccer Mini Game**

![alt text](https://github.com/saurabhj/SoccerMini/blob/master/screens/04_options.png "Change Game settings and Options")

**Change Game settings and Options**

![alt text](https://github.com/saurabhj/SoccerMini/blob/master/screens/05_instruction1.png "The Instructions Page - Page 1")

**The Instructions Page - Page 1**

![alt text](https://github.com/saurabhj/SoccerMini/blob/master/screens/06_instruction2.png "Choose your level of AI")

**The Instructions Page - Page 2**

Thanks.
- Gaurang Sinha & Saurabh Jain