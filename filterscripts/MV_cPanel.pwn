#include <a_samp>
#include <a_mysql>
#include <zcmd>

#define SQL_SERVER	"127.0.0.1"
#define SQL_USER	""
#define SQL_PASS	""
#define SQL_DB		""

#define COL_RED		"{FF0000}"
#define COL_WHITE	"{FFFFFF}"
#define COL_BLUE	"{0000FF}"

#define COLOR_RED	0xAA3333AA
#define COLOR_GREEN	0x00FF00FF

new MySQL:gCon;

enum
{
	DIALOG_NORESPONSE,
	DIALOG_REGISTER,
	DIALOG_LOGIN
};

enum gPlayerInfo
{
	Name[MAX_PLAYER_NAME],
	IP[16],
	Score,
	Money,
	Adminlevel,
	Deaths,
	Kills
};

new PlayerInfo[MAX_PLAYERS][gPlayerInfo];

public OnFilterScriptInit()
{
	print("\n--------------------------------------");
	print("[MV]_cPanel by Michael@Belgium");
	print("--------------------------------------\n");

	gCon = mysql_connect(SQL_SERVER, SQL_USER, SQL_PASS, SQL_DB);

	if(mysql_errno(gCon) != 0)
	{
		printf("Could not connect to database %s on %s", SQL_DB, SQL_SERVER);
		print("Shutting down server.");

		SendRconCommand("exit");
	}
	else
		mysql_log(ALL);
	return 1;
}

public OnFilterScriptExit()
{
	return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
	return 1;
}

public OnPlayerConnect(playerid)
{
	new string[128];

	GetPlayerName(playerid, PlayerInfo[playerid][Name], MAX_PLAYER_NAME);
	GetPlayerIp(playerid, PlayerInfo[playerid][IP], 16);

	PlayerInfo[playerid][Score] = 
	PlayerInfo[playerid][Money] = 
	PlayerInfo[playerid][Kills] =
	PlayerInfo[playerid][Deaths] =
	PlayerInfo[playerid][Adminlevel] = 0;

	mysql_format(gCon, string, sizeof(string), "SELECT Playername FROM Players WHERE Playername = '%e'", PlayerInfo[playerid][Name]);
	mysql_tquery(gCon, string, "OnAccountCheck", "i", playerid);
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
	new string[128];
	if(GetPlayerState(playerid) != PLAYER_STATE_NONE)
	{
		mysql_format(gCon, string, sizeof(string), "UPDATE Players SET Score = %i, Money = %i, Adminlevel = %i, Kills = %i, Deaths = %i, lIP = '%s' WHERE Playername = '%e'", PlayerInfo[playerid][Score], PlayerInfo[playerid][Money], PlayerInfo[playerid][Adminlevel], PlayerInfo[playerid][Kills], PlayerInfo[playerid][Deaths], PlayerInfo[playerid][IP], PlayerInfo[playerid][Name]);
		mysql_query(gCon, string, false);
	}
	return 1;
}

public OnPlayerSpawn(playerid)
{
	return 1;
}

public OnPlayerDeath(playerid, killerid, reason)
{
	if(killerid != INVALID_PLAYER_ID)
	{
		PlayerInfo[killerid][Kills]++;
		PlayerInfo[playerid][Deaths]++;
	}
	return 1;
}

public OnVehicleSpawn(vehicleid)
{
	return 1;
}

public OnVehicleDeath(vehicleid, killerid)
{
	return 1;
}

public OnPlayerText(playerid, text[])
{
	return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
	return 0;
}

public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
	return 1;
}

public OnPlayerExitVehicle(playerid, vehicleid)
{
	return 1;
}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	return 1;
}

public OnPlayerEnterCheckpoint(playerid)
{
	return 1;
}

public OnPlayerLeaveCheckpoint(playerid)
{
	return 1;
}

public OnPlayerEnterRaceCheckpoint(playerid)
{
	return 1;
}

public OnPlayerLeaveRaceCheckpoint(playerid)
{
	return 1;
}

public OnRconCommand(cmd[])
{
	return 1;
}

public OnPlayerRequestSpawn(playerid)
{
	return 1;
}

public OnObjectMoved(objectid)
{
	return 1;
}

public OnPlayerObjectMoved(playerid, objectid)
{
	return 1;
}

public OnPlayerPickUpPickup(playerid, pickupid)
{
	return 1;
}

public OnVehicleMod(playerid, vehicleid, componentid)
{
	return 1;
}

public OnVehiclePaintjob(playerid, vehicleid, paintjobid)
{
	return 1;
}

public OnVehicleRespray(playerid, vehicleid, color1, color2)
{
	return 1;
}

public OnPlayerSelectedMenuRow(playerid, row)
{
	return 1;
}

public OnPlayerExitedMenu(playerid)
{
	return 1;
}

public OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid)
{
	return 1;
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	return 1;
}

public OnRconLoginAttempt(ip[], password[], success)
{
	return 1;
}

public OnPlayerUpdate(playerid)
{
	return 1;
}

public OnPlayerStreamIn(playerid, forplayerid)
{
	return 1;
}

public OnPlayerStreamOut(playerid, forplayerid)
{
	return 1;
}

public OnVehicleStreamIn(vehicleid, forplayerid)
{
	return 1;
}

public OnVehicleStreamOut(vehicleid, forplayerid)
{
	return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	new string[256];
	switch(dialogid)
	{
		case DIALOG_LOGIN:
		{
			if(!response) 
				ShowPlayerDialogEx(playerid,DIALOG_LOGIN);
			else
			{
				mysql_format(gCon, string, sizeof(string), "SELECT * FROM Players WHERE Password = SHA2('%e', 512) AND Playername = '%e'", inputtext, PlayerInfo[playerid][Name]);

				new Cache:result = mysql_query(gCon, string);
				new rows = cache_num_rows();
				cache_delete(result);

				if(rows == 1)
				{
					SendClientMessage(playerid, COLOR_GREEN, "Successfully logged in.");

					cache_get_value_name_int(0, "Score", PlayerInfo[playerid][Score]);
					cache_get_value_name_int(0, "Money", PlayerInfo[playerid][Money]);
					cache_get_value_name_int(0, "Adminlevel", PlayerInfo[playerid][Adminlevel]);
					cache_get_value_name_int(0, "Kills", PlayerInfo[playerid][Kills]);
					cache_get_value_name_int(0, "Deaths", PlayerInfo[playerid][Deaths]);

					ResetPlayerMoney(playerid);
					GivePlayerMoney(playerid, PlayerInfo[playerid][Money]);
					SetPlayerScore(playerid, PlayerInfo[playerid][Score]);
				}
				else
				{
					SendClientMessage(playerid, COLOR_RED, "Wrong password");
					ShowPlayerDialogEx(playerid, DIALOG_LOGIN);
				}
			}
		}

		case DIALOG_REGISTER:
		{
			if(!response)
				ShowPlayerDialogEx(playerid,DIALOG_REGISTER);
			else
			{
				mysql_format(gCon, string, sizeof(string), "INSERT INTO Players (Playername, Password, rIP, lIP) VALUES ('%e', SHA2('%e', 512), '%s', '%s')",PlayerInfo[playerid][Name], inputtext, PlayerInfo[playerid][IP], PlayerInfo[playerid][IP]);
				mysql_query(gCon, string, false);

				SendClientMessage(playerid, COLOR_GREEN, "Successfully registered.");
			}
		}
	}
	return 1;
}

public OnPlayerClickPlayer(playerid, clickedplayerid, source)
{
	return 1;
}

#include <MV_cPanel/cPanel_Functions>
#include <MV_cPanel/cPanel_Callbacks>