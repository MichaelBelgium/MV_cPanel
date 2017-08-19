#include <a_samp>
#include <a_mysql>
#include <sscanf2>
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
	Kills,
	OnlineTime
};

new PlayerInfo[MAX_PLAYERS][gPlayerInfo], query[256];

enum
{
	LEVEL_TRIAL_MOD = 1,
	LEVEL_MOD,
	LEVEL_TRIAL_ADMIN,
	LEVEL_ADMIN,
	LEVEL_OWNER
};

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
	GetPlayerName(playerid, PlayerInfo[playerid][Name], MAX_PLAYER_NAME);
	GetPlayerIp(playerid, PlayerInfo[playerid][IP], 16);

	PlayerInfo[playerid][Score] = 
	PlayerInfo[playerid][Money] = 
	PlayerInfo[playerid][Kills] =
	PlayerInfo[playerid][Deaths] =
	PlayerInfo[playerid][OnlineTime] = 
	PlayerInfo[playerid][Adminlevel] = 0;

	mysql_format(gCon, query, sizeof(query), "SELECT Playername FROM Players WHERE Playername = '%e'", PlayerInfo[playerid][Name]);
	mysql_tquery(gCon, query, "OnAccountCheck", "i", playerid);
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
	if(GetPlayerState(playerid) != PLAYER_STATE_NONE)
	{
		mysql_format(gCon, query, sizeof(query), "UPDATE Players SET Score = %i, Money = %i, Adminlevel = %i, Kills = %i, Deaths = %i, lIP = '%s', OnlineTime = OnlineTime + %i WHERE Playername = '%e'", PlayerInfo[playerid][Score], PlayerInfo[playerid][Money], PlayerInfo[playerid][Adminlevel], PlayerInfo[playerid][Kills], PlayerInfo[playerid][Deaths], PlayerInfo[playerid][IP], NetStats_GetConnectedTime(playerid)/1000,  PlayerInfo[playerid][Name]);
		mysql_query(gCon, query, false);
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

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	switch(dialogid)
	{
		case DIALOG_LOGIN:
		{
			if(!response) 
				ShowPlayerDialogEx(playerid,DIALOG_LOGIN);
			else
			{
				mysql_format(gCon, query, sizeof(query), "SELECT * FROM Players WHERE Password = SHA2('%e', 512) AND Playername = '%e'", inputtext, PlayerInfo[playerid][Name]);

				new Cache:result = mysql_query(gCon, query);
				new rows = cache_num_rows();
				
				if(rows == 1)
				{
					SendClientMessage(playerid, COLOR_GREEN, "Successfully logged in.");

					cache_get_value_name_int(0, "Score", PlayerInfo[playerid][Score]);
					cache_get_value_name_int(0, "Money", PlayerInfo[playerid][Money]);
					cache_get_value_name_int(0, "Adminlevel", PlayerInfo[playerid][Adminlevel]);
					cache_get_value_name_int(0, "Kills", PlayerInfo[playerid][Kills]);
					cache_get_value_name_int(0, "Deaths", PlayerInfo[playerid][Deaths]);
					cache_get_value_name_int(0, "OnlineTime", PlayerInfo[playerid][OnlineTime]);

					ResetPlayerMoney(playerid);
					GivePlayerMoney(playerid, PlayerInfo[playerid][Money]);
					SetPlayerScore(playerid, PlayerInfo[playerid][Score]);
				}
				else
				{
					SendClientMessage(playerid, COLOR_RED, "Wrong password");
					ShowPlayerDialogEx(playerid, DIALOG_LOGIN);
				}

				cache_delete(result);
			}
		}

		case DIALOG_REGISTER:
		{
			if(!response)
				ShowPlayerDialogEx(playerid,DIALOG_REGISTER);
			else
			{
				mysql_format(gCon, query, sizeof(query), "INSERT INTO Players (Playername, Password, rIP, lIP) VALUES ('%e', SHA2('%e', 512), '%s', '%s')",PlayerInfo[playerid][Name], inputtext, PlayerInfo[playerid][IP], PlayerInfo[playerid][IP]);
				mysql_query(gCon, query, false);

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
#include <MV_cPanel/cPanel_Commands>