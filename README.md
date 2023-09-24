Don't Touch Grass by It's a Feature: Game Feature Guide
================================================================

For Don't Touch Grass, we implemented all priority 1-4 features 
described in our game design doc, as well as several bonus
features, including two extra unlockable sprites and audio.

This is a brief guide to the different game features to make
them easier to test and assess. The game is designed to be
intuitive enough to be played without a guide, but this will
hopefully make testing easier.

There are no bugs.

GAME STRUCTURE
----------------------------------------------------------------

There are four primary game states: in-game, menu/pause, restart,
and shop.

In-game: During this state, the gameplay (run) is ongoing. The 
game moves to the restart state on a game over condition, or to 
the menu/pause state upon pausing.

Menu/pause: The game initiates in this state. From this state,
the player can either resume the ongoing run, or enter the shop.

Restart: Following a game over condition, the game returns to
this state. From this state, the player can enter the menu/pause
state by resetting the run, or enter the shop.

Shop: The shop is accessible through the restart and the menu/
pause states. In this state, the player can purchase different
upgrades using coins collected in-game, which are described in
PLAYER CHARACTERS and UPGRADES. The player can return from the
shop to the previous state.

GAMEPLAY
----------------------------------------------------------------

During a run, the player can control the character with arrow
keys and jump on platforms to move up. The game scrolls with
the player, and platforms and collectables are pseudorandomly
generated indefinitely. The game difficulty increases with
altitude.

The player can also gather coins, which are saved between runs.
These coins may be spent in the shop.

The player triggers a game over condition if they fall off the
bottom of the map, or if they land on a grass platform. This
ends the run.

The player can pause the run at any point, as long as a game
over condition has not been triggered.

PLATFORMS AND COLLECTABLES
----------------------------------------------------------------

During the in-game state, the player can interact with a number
of different platforms and collectable items. The collectable  
items available are coins and jetpacks.

The game starts with only providing bronze Dogecoins, which are
only worth 1 each. Silver and gold Dogecoins can be unlocked, as
decsribed in UPGRADES. The total coin counter is displayed in 
the upper left. This counter supports a display of up to 9999 
coins. Even CS TAs touch too much grass to gather more than that.

The jetpack is an unlockable item described in UPGRADES.

There are five types of platforms.

Standard: The player can jump on this platform.

Breakable: Like the standard platform, but can only be used once.

Moving: Like the standard platform, but moves left to right
across the screen at some randomized speed.

Spring: Like the standard platform, but gives the player a boost
in jump height.

Grass (deadly): Since CS majors cannot touch grass, this
platform triggers a game over if the player lands on it.

PLAYER CHARACTERS
----------------------------------------------------------------

There are three player characters available in the shop, one
default and two unlockable, each priced as labeled. The player 
can select any unlocked sprite to play as. It is possible to 
unlock and/or switch sprites mid-run using the pause feature.

When selected, each character speaks a catchphrase.

CS major (default): "I have sets to do!" 
The default sprite, with no special abilities.

Physics major: "I'm a REAL scientist!"
Wielding a mysterious floating gyroscope, the CS-Physics major
can manipulate gravity to slow their fall.

BEM major: "money money money"
With a briefcase full of cash (and NFTs), the CS-BEM major has
the business expertise to gather twice as many coins.

UPGRADES
----------------------------------------------------------------

There are six upgrades available in the shop, each priced as
labeled. Upgrades can be toggled on or off once unlocked. It is 
possible to unlock and/or toggle upgrades mid-run using the 
pause feature.

Speed: Increases the player's lateral movement speed.

Jump: Increases the player's jump height.

Death Save: Creates a bonus last-chance platform under the player
should they fall off the bottom of the screen, saving the run.
The only single-use upgrade; must be purchased again following
each use.

Jetpack: Enables the generation of a collectable jetpack during
runs. Once collected, this jetpack allows players to fly upwards
for a few seconds.

New Coins: Enables the generation of silver and gold Dogecoins
in addition to the standard bronze Dogecoins during runs. 
These new coins are worth 2 and 5 coins respectively.

Download More RAM: Downloads more RAM onto your computer.

GRAPHICS, TEXT, AND AUDIO
----------------------------------------------------------------

Don't Touch Grass comes with texture, text, and audio support.

There are custom hand-drawn graphics for everything displayed in
the game, including: restart, menu/pause, and shop pages; three
player characters with three states each (default, falling, and
jetpack); platforms and collectables; game backdrop; and ground.

There is also text display for the coin cointer.

Finally, we also implemented audio. A scream track is played
upon a game over in each run. Additionally, when characters are
selected in the shop, each plays their own catchphrase.
