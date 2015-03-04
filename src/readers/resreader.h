/**
 * This file is part of OpenAtomic.
 *
 * OpenAtomic is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * OpenAtomic is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenAtomic. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef RESREADER_H
#define RESREADER_H

#include <map>
#include <string>
#include <def/sdl.h>

#include <debug/errorreporter.h>

using namespace std;

enum options_keys
{
    /* is SELECTIVE sound pre-caching enabled? */
    o_soundCacheSelective = 3,
    /* is TOTAL sound pre-caching enabled? */
    o_soundCacheTotal = 4,
    /* how many different sound effects can be cached at once? Valid numbers are 10 to 200 */
    o_soundCacheDiffSounds = 5,
    /* how much memory can be consumed by these cached sounds before we start tossing out
    old ones to make more room? Valid numbers are .5mb to 20mb */
    o_soundCacheMemory = 6,
    /* how many seconds between voluntary clearings of the sound cache and
    re-choosing/re-loading of the soundlst.res file?
    (if you put a zero here, it will never voluntarily clear the entire cache.) */
    o_soundCacheRldSec = 7,
    /* how many concurrent sounds do we want to allow? */
    o_soundNumberConcurrent = 8,
    /* how often to flush cached sounds (see #7 above) when min memory footprint
    is active (i.e., you have less than 32mb of ram).  (time is in seconds) */
    o_soundFlushSec = 9,
    /* how many frames to cycle the regular flame animation */
    o_animCycles = 10,
    /* default number of seconds that the intro screens will wait for keypress */
    o_animSplashSec = 12,
    /* MINIMUM number of seconds to wait at screens so that other computers
    can catch up (does not affect non-net games) */
    o_netWaitForOtherSec = 13,
    /* default value of "do we assign a keyboard player?" */
    o_optAssignKeyboard = 14,
    /* is the online manual enabled? */
    o_optOnlineManEnabled = 15,
    /* how many different "glue" pcx files are available? (glue0, glue1, etc.)
    they are zero-based, so if you have glue0 and glue1, this should be 2. */
    o_optNumOfGlue = 16,
    /* how many frames to cycle the brick disintegration animation */
    o_animBrickDisInt = 20,
    /* how many frames (standard rate) do you have to out-survive the other guy?
    ** this is only a nominal frame rate that is used as a reference for other
    frame-count-based values in this file!  Do not change this value because
    it will not improve your performance at all, and may cause other things
    to malfunction! */
    o_animOutSurviveGuy = 25,
    /* default enclosement depth (how far the playfield will close in)
    0 is none, 1 is 2 rows, 2 is 4 rows, 3 is all the way */
    o_optEnclDepth = 27,
    /* the possible different enclosement depths (0, 1, 2, 3 right now) */
    o_optEnclDifDepth = 28,
    /* how many frames per second are we gonna attempt to get? */
    o_animFPS = 30,
    /* what is the maximum milliseconds the game can advance in any given
    "frame."  this prevents a disk hit from moving everybody a whole
    huge distance on the screen and screwing things up. */
    /* 31 */
    /* how many "standard" frames do we show true colors before switching over to
    show team colors (on team play only). */
    o_animShowTeamColFrm = 32,
    /* how many different levels (waves) are defined?
    if you modify this number and play over the net, make sure the other guys
    have EXACTLY the same numbers of different waves, as well as all the data
    files necessary for those waves! */
    o_optLevels = 35,
    /* default value of "do we randomize player starting positions?" */
    o_optRandomStart = 40,
    /* fuze length (frames) */
    o_animFuzeLength = 41,
    /* default starting speed (100ths of a pixel/frame) */
    o_animDefSpeed = 42,
    /* default bombtype: 0 - regular bombs, 1 - controlled (trigger) bombs, 2 - jelly bombs */
    o_animDefBombs = 43,
    /* when a wall segment closes in on a bomb, does it set the bomb off?
    0 - destroy the bomb, 1 - detonate the bomb */
    o_animBombSegClosed = 46,
    /* how many different default "Node Names" are in the messages.txt file (#500) */
    o_optDifDefNodes = 47,

    /* Defaults */
    o_optDefBombs = 50,
    o_optDefFlameLength = 51,
    o_optDefKickers = 53,
    o_optDefSkates = 54,
    o_optDefPunches = 55,
    o_optDefGrabs = 56,
    o_optDefSpooges = 57,
    o_optDefGoldflames = 58,
    o_optDefTriggerBombs = 59,
    o_optDefJellyBombs = 60,

    /* how much extra speed (100ths of a pixel/frame) does a skate give you? */
    o_optDefSkatesSpeed = 90,
    /* how much speed the CLOGS (special roulette power-"down") take away */
    o_optDefClogsSpeed = 91,
    /* how many seconds do we wait before entering "attract" mode?
    if you put a number < 5 here, it will never enter attract mode. */
    o_optSecBefAttractMode = 92,
    /* what are the chances of issuing a post-death taunt ("this is too easy") */
    /* 95 */
    /* the length (in seconds) of a default combat game */
    o_optGameLengthSec = 100


/*
; the time remaining when the "hurry" message first flashes
; it is NOT RECOMMENDED that you modify this value!  A lot of strange
; things will happen if you set it to "non standard" values.  Leaving it
; at 60 should keep most of the program happy...
101,60                                          ; PGT

; the period of time when "over-powerful" powers won't appear (will go elsewhere)
102,40                                          ; PGT

; how many different death animations do we have? (die 1 through die 24)
; *** if you modify this and play over the net, make sure the other guys have
; EXACTLY the same number of death anims!!!
105,24      ;;; 24

; how many pixels per reference frame does death anim #9 move upwards (the angel)?
106,4

; coordinates on screen to put the timer values
110,525
111,36
112,4       ; extra pixels of space between the timer font digits

; two vertical (Y) coordinates of each player row across the top
113,6
114,26

; left (X) coordinates of each player column across the top
115,10
116,110
117,210
118,310
119,410

; stuff about diseases (default settings)
; can diseases be blown up like all other powerups?
; gbl_diseases_can_be_destroyed;
120,1                                           ; PGT

; does a disease "wear off" after some amount of time?
; gbl_diseases_are_time_limited;
121,1                                           ; PGT

; will a disease "recycle" like other powerups when it comes out of you?
; gbl_diseases_will_recycle;
122,0                                           ; PGT

; does a disease multiply when spread, or just "hand off"?
; gbl_diseases_multiply;
123,1                                           ; PGT

; does acquiring a fresh powerup (or another disease) "cure" you of the previous disease?
; gbl_diseases_are_curable;
124,1                                           ; PGT

; here's what the chances are that a disease gets cured when you get a new
; powerup (larger numbers are a slimmer chance; it is 1 in N)
; gbl_diseases_cure_chance;
125,10                                          ; PGT

; minimum time to "have" a disease before it can be passed again (to prevent
; random passing multiple times upon single contact).  (disease_freshness)
129,10                                          ; PGT

; how long each type of disease lasts (see DISEASE.H)
130,300                                         ; PGT
131,300                                         ; PGT
132,300                                         ; PGT
133,300                                         ; PGT
134,300                                         ; PGT
135,300                                         ; PGT
136,300                                         ; PGT
137,300                                         ; PGT
138,300                                         ; PGT

; end stuff about diseases

; three speeds that a conveyor belt moves (100th of a pixel)
189,3       ; here's how many different speeds there are...
190,250     ; low                               ; PGT
191,350     ; medium                            ; PGT
192,450     ; high                              ; PGT

; color remaping default values (RGB pairs, from 0 to 9 of the .RMP files)
200,100             ; 0.RMP: white
201,100
202,100

205,20              ; 1.RMP: black
206,20
207,20

210,100             ; 2.RMP: red
211,0
212,0

215,0               ; 3.RMP: blue
216,0
217,100

220,0               ; 4.RMP: green
221,100
222,0

225,100             ; 5.RMP: yellow
226,100
227,0

230,0               ; 6.RMP: cyan
231,100
232,100

235,100             ; 7.RMP: magenta
236,0
237,100

240,100             ; 8.RMP: orange
241,50
242,0

245,50              ; 9.RMP: purple
246,0
247,100

; some bomb speeds...
300,1000    ; how fast does a kicked bomb move?  (1000 is nice)     ; PGT
301,1300    ; how fast does a punched bomb move? (1300 is nice)     ; PGT

; how many wins to win a match? (default; override by settings configuration)
310,2

; minimum number of seconds between potential dud bombs
320,180
; additional random number of seconds between potential dud bombs
321,180
; chance of a dud bomb (1 in this number; bigger numbers are rarer; don't use zero!)
322,3
; how many frames of animation does a dud bomb wait (minimum)
323,120
; how many frames of animation does a dud bomb wait (random additional)
324,200

; how many different cornerhead animations we have (0, 1, etc.)
; if you change this number, make sure all other network players have all
; your new cornerhead animations and that this number is EXACTLY the same on
; all machines!!
330,13      ;;; 13

; which levels have regenerating tiles, and how many seconds between tile
; regeneration attempts.  (zero means no regeneration)
340,0
341,0
342,0
343,0
344,0
345,0
346,0
347,4   ; cemetary/mortuary
348,0
349,0
350,0

; these are how many of each type of powerup appear on a level
; a negative number here indicates how many times we'll do a 1-in-10 chance
; of putting the powerup down.
400,10  ; bombs
401,10  ; flames
402,3   ; diseases
403,4   ; kickers
404,8   ; skates
405,2   ; punch
406,2   ; grab
407,1   ; spooge
408,-2  ; goldflame
409,-4  ; trigger bombs
410,1   ; jelly bombs
411,-4  ; super bad diseases
412,-2  ; random powerups

; these are the "ice delay" values (how much the controls are slowed by
; the presence of ice on each level).  This is measured in milliseconds.
449,0       ; just to prevent invalid access in case the net screws up...
450,0       ; new traditionalist green acres        ; PGT
451,0       ; classic green acres                   ; PGT
452,250     ; hockey rink                           ; PGT
453,0       ; ancient egypt                         ; PGT
454,0       ; coal mine                             ; PGT
455,0       ; beach                                 ; PGT
456,0       ; aliens                                ; PGT
457,0       ; haunted house                         ; PGT
458,0       ; under the ocean                       ; PGT
459,0       ; deep forest green                     ; PGT
460,0       ; inner city trash                      ; PGT

; this is the curve (upwards) of a bomb being picked up
500,12,10
502,25,20
504,25,30
506,12,40

; the following are the LIMITS as to how many of each powerup a player
; can accumulate.  Make this too big and a lot of stuff starts to break...
; if the number is zero (0), there is no limit to how many you can accumulate.
550,8       ; number of bombs                                   ; PGT
551,8       ; flame length (cells beyond epicenter blast)       ; PGT
552,0       ; disease (this is not used!!)                      ; PGT
553,1       ; kickers                                           ; PGT
554,4       ; skates (speed powerups)                           ; PGT
555,1       ; punches (red glove)                               ; PGT
556,1       ; grabs (blue gloves)                               ; PGT
557,1       ; spooges                                           ; PGT
558,1       ; goldflames                                        ; PGT
559,1       ; trigger bombs                                     ; PGT
560,1       ; jelly bombs                                       ; PGT
561,0       ; super bad diseases( this is not used!!)           ; PGT
562,0       ; randoms (this is not used!!)                      ; PGT
563,0       ; not used yet                                      ; PGT
564,0       ; not used yet                                      ; PGT

; here are the default starting X,Y locations for each of the players
; (negative numbers wrap around from the right edge/bottom edge of the screen)
600,0,0
602,-1,-1
604,0,-1
606,-1,0
608,6,4
610,8,0
612,-3,4
614,2,-5
616,-5,-3
618,6,-1

; for the many-dropped bomb powerup, this determines the chance (1 in N) of
; the "Clear" or "Fire In The Hole" type audio playing
650,4
; and this is what constitutes "many" dropped bombs
651,4

; how high an "arc" does a punched bomb make?
660,65      ; initial three-space punch bounces
661,20      ; subsequent small 1-space punch bounces

665,2       ; how long do you "pause" when picking up a bomb?   ; PGT

; at each 0,0 intersection, what is the chances that a punched
; jelly bomb will change directions crazily?
667,3                                           ; PGT

670,1       ; what's the minimum number of powers you lose when hit on the head?        ; PGT
671,3       ; what's the additional random number of powers you might lose?             ; PGT

; how many frames do you bounce on a trampoline?
680,30                                          ; PGT
681,35      ; how many pixels vertically do you move each frame?        ; PGT

; blink rate of the little bomber-dude cursor (base, random)
690,2,2

; the "clear" cell radius that must exist around a potentially-regenerating
; tile spot.  Nobody can be within this radius.
695,4

; the following are coordinates for various menu screens
; they are given as X, Y, YS, and W, where:
;       X, Y - first (top) item in the menu
;       YS - y-spacing down the list
;       W - width of the printed field (if applicable)
; MAINMENU:
; this is the list of selectable points on the mainmenu:
700,332,140, 38,  0

; PLAYER INPUT TYPE SELECTION:
; text heading of player input type listings
705, 40,140,  0,200
; actual listing of player input types
710, 70,170, 24,150
; text heading of available joysticks
715,300,140,  0,170
; actual listing of available joysticks
720,320,170, 24,320

; OPTIONS SCREEN:
; where the "sample" blocks are displayed onscreen
; X, Y, XSize, YSize (number of blocks)
730,400,100,5,5
; the actual listing of options items
735, 55,170, 24,300

; SETTINGS SCREEN:
; the actual listing of options items
745, 55, 40, 22,500

; JOIN NET GAME SCREEN:
; "Our nodename is:"
750,120, 80,  0,400
; "Available Net Games:
755,120,120,  0,250
; eacn individual server listing
760,120,140, 20,400

; START NET GAME SCREEN:
; "Our nodename is:"
765,120, 80,  0,400
; "Available Net Games:
770,120,120,  0,250
; eacn individual server listing
775,120,140, 20,400

; WINNER SCREEN
; "Winner was:"
780,150,140,  0,400
; each individually-listed player
785,150,210, 20,400

; "Press F1 for help" (goes on a lot of different screens)
790,320,440,  0,300

; "Continue with same net game?"
795, 80,150,  0,500

; player %u wins the match...
800,150, 94,  0,400

; Goldman Roulette Wheel (title at top)
805,320, 30,  0,400

; Editor - mainmenu header
810, 50,100,  0,400
; Editor - mainmenu items
815, 80,140, 20,400

; how many different AI personalities are predefined?
900,1

; how farahead does the fire-god (final closing-in bricks) tell the
; AIs that it is in itself a threat?
910,15

; chance that an AI will execute the "blast bricks" routine
915,5

; how close does a powerup have to be for an AI to go for it?
920,4

; center of roulette wheel for goldman screen
1000,320,240
; radius(es) (x,y) of roulette wheel perimeter
1002,200,150
; resolution of the circle the roulette wheel turns on (300 is minimum!)
1004,70
; X,Y parameters for lissajous-shaped roulette wheel... (grin)
1006,1,1

; how many seconds the twinkling of goldman lasts
; (zero will make it last indefinitely)
1010,5

; how many milliseconds to wait before a critical packet is declared overdue
; and needs to be re-sent.
; this can be adjusted based on expected network latency.  if it is set too
; low then you will have many gratuitous retransmits and network performance
; will suffer.  if it is set too high then the potential lag between the
; games (due to missed packets) can become noticeable and affect game
; integrity/synchronization.
1100,200        ; cancel - no net play.
1101,200        ; IPX
1102,750        ; modem
1103,400        ; serial
1104,400        ; TCP/IP

; how many different protocols (see list above at 1100) are supported?
1110,4

; the following array enables/disables various levels in the RANDOM LEVEL
; selection (so that you can lock out some levels you Never want to play).
1150,1
1151,1
1152,0      ; too annoying to have control delays!
1153,1
1154,1
1155,1
1156,0      ; too hard to see visually!
1157,1
1158,1
1159,1
1160,1
1161,1

; stuff related to campaign mode
; chance that a ghost or rover will change directions at an intersection
1200,3
; chance that the direction change will NOT towards a human
1205,3

; points for killing an AI (in campaign mode only!)
1300,250
; points for killing a rover (in campaign mode only)
1310,15
; points for killing a ghost (in campaign mode only)
1320,25
*/
    
};

class CResReader
{
private:
    string whiteSpaces;
    map<Uint16, string> data;
    map<Uint16, string> ::iterator cur;
    map<Uint16, string> ::iterator prev;
    map<Uint16, string> ::iterator next;
    Uint16 index;
    void trimRight ( string& str, string& trimChars );

    CErrorReporter er;
public:
    CResReader();
    Uint8 read ( string filename );
    string getItem ( Uint16 index );
    string getNext ();
    string getPrev ();
};

#endif // RESREADER_H
