# MV_cPanel

This filterscript is an admin system + vip system for your San Andreas Multiplayer server. 

The script was originally created on 15/04/2011. Now this version will be a full rescript of [the older version from some years ago](http://forum.sa-mp.com/showthread.php?t=248711).

## Requirements
* SQL plugin (R41-1 or higher)
* ZCMD
* sscanf

## Features
* Login/Register system
* SQL based
* Admin system (5 levels)
* VIP system
* Anti-stuff: anti-(command)spam, anti-money hack (serversided money and score)

## Config

Before you should use this filterscript, you have to take a look at least at the config file. 
I've put default values which are not that bad but you might think different about some stuff.
If true, edit the config file and re-compile the filterscript.

```PAWN
#define COOLDOWN_COMMAND	5	//time in seconds between command executes
#define COOLDOWN_CHAT		2	//time in seconds between chat messages
#define COOLDOWN_VIP_HEAL	600	//time in seconds between vip healing (/vipheal)

#define MAX_WARNINGS		5		//max warnings a player can have before he gets kicked/banned
#define BAN_ON_WARN			false 	//if true, a player which reaches MAX_WARNINGS he'll get banned, else kicked.
#define MUTE_EQUALS_NOCMDS	true 	//if true, a player that is muted won't be able to execute any commands either.
#define ADMIN_SKIN			84		//the skinid of admins when u do /adminduty
#define FREE_DIALOG_ID		100		//specify a dialogid that isn't used by your gamemode
```

## Media

