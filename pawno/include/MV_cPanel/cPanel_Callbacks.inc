forward OnAccountCheck(playerid);
forward OnBanCheck(playerid);
forward OnVipLoaded(playerid);
forward OnPlayerLogin(playerid);
forward PlayerTimer(playerid);
forward UnmutePlayer(playerid);
forward JailTimer(playerid);
forward SpectateTimer(playerid, target);

public OnAccountCheck(playerid)
{
	if(cache_num_rows() == 1)
		ShowPlayerDialogEx(playerid,DIALOG_LOGIN);
	else
		ShowPlayerDialogEx(playerid,DIALOG_REGISTER);
}

public OnBanCheck(playerid)
{
	if(cache_num_rows() >= 1)
	{
		new banned[256*3], string[128], data[5][32], bID;

		cache_get_value_name_int(0, "bID", bID);
		cache_get_value_name(0, "Player", data[0]);
		cache_get_value_name(0, "Admin", data[1]);
		cache_get_value_name(0, "Reason", data[2]);
		cache_get_value_name(0, "IP", data[3]);
		cache_get_value_name(0, "Date", data[4]);

		strcat(banned, "You are banned from this server.\n\n");

		format(string, sizeof(string), "ID: %i\n", bID);
		strcat(banned, string);

		format(string, sizeof(string), "Name: %s\n", data[0]);
		strcat(banned, string);

		format(string, sizeof(string), "Banned by: %s\n", data[1]);
		strcat(banned, string);

		format(string, sizeof(string), "Reason: %s\n", data[2]);
		strcat(banned, string);

		format(string, sizeof(string), "IP: %s\n", data[3]);
		strcat(banned, string);

		format(string, sizeof(string), "Date: %s\n", data[4]);
		strcat(banned, string);

		ShowPlayerDialog(playerid, DIALOG_NORESPONSE, DIALOG_STYLE_MSGBOX, "Banned", banned, "OK", "");

		format(string, sizeof(string), COL_ADMIN_1"-[%s]- "COL_ADMIN_2"Denied connection to %s. Banned: %s - %s - %s (%i)", AC_NAME, GetPlayerNameEx(playerid), data[0], data[3], data[2], bID);
		SendClientMessageToAdmins(-1, string);

		Kick(playerid);
	}
	else
	{
		mysql_format(gCon, query, sizeof(query), "SELECT Playername FROM Players WHERE Playername = '%e'", GetPlayerNameEx(playerid));
		mysql_tquery(gCon, query, "OnAccountCheck", "i", playerid);
	}
}

public OnVipLoaded(playerid)
{
	if(cache_num_rows() == 1)
	{
		cache_get_value_name_int(0, "Duration", VipInfo[playerid][Duration]);
		cache_get_value_name_bool(0, "Toggle0", VipInfo[playerid][Toggle][0]);
		cache_get_value_name_bool(0, "Toggle1", VipInfo[playerid][Toggle][1]);
		cache_get_value_name_bool(0, "Toggle2", VipInfo[playerid][Toggle][2]);
		
		printf("VIP %s has connected",PlayerInfo[playerid][Name]);

		if(VipInfo[playerid][Duration] < gettime() && VipInfo[playerid][Duration] != 0)
		{
			VipInfo[playerid][Duration] = -1;
			SendClientMessage(playerid, -1,  COL_VIP_1"-[VIP]-"COL_VIP_2" Your VIP status has been expired.");

			mysql_format(gCon, query, sizeof(query), "DELETE FROM Vips WHERE Name = '%e'", PlayerInfo[playerid][Name]);
			mysql_tquery(gCon, query);
		}
		else
			SendClientMessage(playerid, -1, COL_VIP_1"-[VIP]-"COL_VIP_2" Your vip settings have been loaded.");
	}
	else
		VipInfo[playerid][Duration] = -1;
}

public OnPlayerLogin(playerid)
{
	if(cache_num_rows() == 1)
	{
		SendClientMessage(playerid, COLOR_GREEN, "Successfully logged in.");

		cache_get_value_name_int(0, "Score", PlayerInfo[playerid][Score]);
		cache_get_value_name_int(0, "Money", PlayerInfo[playerid][Money]);
		cache_get_value_name_int(0, "Adminlevel", PlayerInfo[playerid][Adminlevel]);
		cache_get_value_name_int(0, "OnlineTime", PlayerInfo[playerid][OnlineTime]);
		cache_get_value_name_int(0, "Warnings", PlayerInfo[playerid][Warns]);
		cache_get_value_name_int(0, "Muted", PlayerInfo[playerid][Muted]);

		ResetPlayerMoney(playerid);
		SetPlayerCash(playerid, PlayerInfo[playerid][Money]);
		SetPlayerScore(playerid, PlayerInfo[playerid][Score]);
		if(IsPlayerMuted(playerid))
				MutePlayer(playerid, INVALID_PLAYER_ID, "Still muted since last session.", PlayerInfo[playerid][Muted]);
		

		mysql_format(gCon, query, sizeof(query), "SELECT * FROM Vips WHERE Name = '%e'", PlayerInfo[playerid][Name]);
		mysql_tquery(gCon, query, "OnVipLoaded", "i", playerid);
	}
	else
	{
		SendClientMessage(playerid, COLOR_RED, "Wrong password");
		ShowPlayerDialogEx(playerid, DIALOG_LOGIN);
	}
}

public PlayerTimer(playerid)
{
	if(IsPlayerVIP(playerid) && IsPlayerInAnyVehicle(playerid) && VipInfo[playerid][Toggle][2])
	{
		new Float:vhealth, veh = GetPlayerVehicleID(playerid);

		GetVehicleHealth(veh, vhealth);
		if(vhealth <= 300) RepairVehicle(veh);
	}

	if(!IsPlayerAdminEx(playerid, LEVEL_MOD) && GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_USEJETPACK)
	{
		BanPlayer(playerid, INVALID_PLAYER_ID, "Jetpack");
	}

	#if MAX_PING > 0
	if(GetPlayerPing(playerid) > MAX_PING)
	{
		KickPlayer(playerid, INVALID_PLAYER_ID, "High ping");
	}
	#endif

	for(new i = 0; i < sizeof(forbiddenWeapons); i++)
	{
		if(GetPlayerWeapon(playerid) == forbiddenWeapons[i])
		{
			#if BAN_ON_FWEAPON
			BanPlayer(playerid, INVALID_PLAYER_ID, "Forbidden weapon");
			#else
			KickPlayer(playerid, INVALID_PLAYER_ID, "Forbidden weapon");
			#endif
			break;
		}
	}
}

public UnmutePlayer(playerid)
{
	KillTimer(PlayerInfo[playerid][pTimer][1]);

	new string[128];
	format(string, sizeof(string), "You have been unmuted after %i minutes", PlayerInfo[playerid][Muted]);
	SendClientMessage(playerid, COLOR_GREEN, string);

	PlayerInfo[playerid][Muted] = 0;
}

public JailTimer(playerid)
{
	KillTimer(PlayerInfo[playerid][pTimer][2]);
	SendClientMessage(playerid, COLOR_GREEN, "You have been unjailed.");

	SetPlayerInterior(playerid, 0);
	TogglePlayerControllable(playerid, true);
	SpawnPlayer(playerid);
}

public SpectateTimer(playerid, target)
{
	if(IsPlayerConnected(target))
	{
		if(IsPlayerInAnyVehicle(target))
			PlayerSpectateVehicle(playerid, GetPlayerVehicleID(target));
		else
			PlayerSpectatePlayer(playerid, target);	
	}
	else
		cmd_spectate(playerid, ""); //equals stop spectating
}
