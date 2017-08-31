#define COL_ADMIN_1	"{FF0A00}"
#define COL_ADMIN_2	"{FFFFFF}"
#define COL_VIP_1	"{411053}"
#define COL_VIP_2	"{7B498D}"

#define SQL_SERVER	"127.0.0.1"
#define SQL_USER	""
#define SQL_PASS	""
#define SQL_DB		""

#define COOLDOWN_COMMAND	5		//time in seconds between command executes
#define COOLDOWN_CHAT		2		//time in seconds between chat messages
#define COOLDOWN_VIP_HEAL	600		//time in seconds between vip healing (/vipheal)

#define MAX_WARNINGS		5		//max warnings a player can have before he gets kicked/banned
#define MAX_PING			500		//max ping a player can get, above = kick (0 to disable)

#define BAN_ON_WARN			false 	//if true, a player which reaches MAX_WARNINGS he'll get banned, else kicked.
#define MUTE_EQUALS_NOCMDS	true 	//if true, a player that is muted won't be able to execute any commands either.
#define ADMIN_SKIN			84		//the skinid of admins when u do /adminduty
#define FREE_DIALOG_ID		100		//specify a dialogid that isn't used by your gamemode
#define AC_NAME 			"Server"//a name for your server or anti-cheat.

#define LOG_COMMANDS		false 	//Log ALL commands.
#define LOG_ADMINCOMMANDS	true 	//log admin commands with all the info they give (almost the same like LOG_COMMANDS but u get more info in the db)
#define LOG_REPORTS			true 	//log all the reports that have been sent to admins
#define LOG_ANTICHEAT		true 	//log all the actions from the anti-cheat
