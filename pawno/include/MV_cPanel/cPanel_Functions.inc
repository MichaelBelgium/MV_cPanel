#define cPanel_func:%0(%1)		forward %0(%1); public %0(%1)

ShowPlayerDialogEx(playerid,dialogid)
{
	new string[128];
	switch(dialogid)
	{
		case DIALOG_LOGIN:
		{
			format(string, sizeof(string), COL_WHITE"Your user is "COL_RED"registered"COL_WHITE"! Please "COL_BLUE"login"COL_WHITE" with your password below!", PlayerInfo[playerid][Name]);
			ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "Login", string, "Login", "");
		}

		case DIALOG_REGISTER:
		{
			format(string, sizeof(string), COL_WHITE"Your user (%s) is "COL_RED"not"COL_WHITE" registered! Please "COL_BLUE"register"COL_WHITE" with a password below!", PlayerInfo[playerid][Name]);
			ShowPlayerDialog(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, "Register", string, "Register", "");
		}

		case DIALOG_CPANEL: 
		{
			if(IsPlayerAdminEx(playerid, LEVEL_OWNER))
				format(string, sizeof(string), "Open player panel\nOpen server panel\nRestart server");
			else
				format(string, sizeof(string), "Open player panel");

			ShowPlayerDialog(playerid,DIALOG_CPANEL,DIALOG_STYLE_LIST,"cPanel by {FF0A00}Michael@Belgium",string,"Ok","Cancel");	
		}
		case DIALOG_CPANEL_PLAYERPANEL: ShowPlayerDialog(playerid,DIALOG_CPANEL_PLAYERPANEL,DIALOG_STYLE_INPUT,"Player ID","Please fill in a PlayerID.","OK","Cancel");
		case DIALOG_CPANEL_SERVERPANEL: ShowPlayerDialog(playerid,DIALOG_CPANEL_SERVERPANEL,DIALOG_STYLE_LIST,"cPanel by {FF0A00}Michael@Belgium","Change gamemode \r\nLoad filterscript \r\nUnload filterscript \r\nSet hostname \r\nSet server password \r\nSet gamemodetext","Select","Back");
		case DIALOG_CPANEL_PP_LIST: 
		{
			if(IsPlayerAdminEx(playerid, LEVEL_TRIAL_MOD)) strcat(string,"Spectate\nGet\nGoto\n(Un)Mute\n");
			if(IsPlayerAdminEx(playerid, LEVEL_MOD)) strcat(string,"Freeze\nUnfreeze\nJail\nUnjail\nKill\nKick\n");
			if(IsPlayerAdminEx(playerid, LEVEL_TRIAL_ADMIN)) strcat(string, "Ban\n");
			if(IsPlayerAdminEx(playerid, LEVEL_OWNER)) strcat(string, "Set level");

			ShowPlayerDialog(playerid, DIALOG_CPANEL_PP_LIST, DIALOG_STYLE_LIST, "Options", string, "Select", "Cancel");
		}
		case DIALOG_CPANEL_PP_LIST_LEVEL: ShowPlayerDialog(playerid, DIALOG_CPANEL_PP_LIST_LEVEL, DIALOG_STYLE_INPUT, "Edit adminlevel", "Which adminlevel would you promote/demote the player to?", "Change", "Cancel");
	}

	return 1;
}

cPanel_func:SendClientMessageToAdmins(color, message[])
{
    for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
    {
        if(!IsPlayerConnected(i) || PlayerInfo[i][Adminlevel] == 0) continue;
        SendClientMessage(i, color, message);
    }
}

stock SaveLog(logtype, player[] = "", command[] = "", target[] = "", params[] = "")
{
	new tmptarget[MAX_PLAYER_NAME], tmpparams[64], tmpcommand[16], tmpplayer[MAX_PLAYER_NAME];

	if(isnull(player))
		format(tmpplayer, sizeof(tmpplayer), "%s", "NULL");
	else
		mysql_format(gCon, tmpplayer, sizeof(tmpplayer), "'%e'", player);

	if(isnull(target))
		format(tmptarget, sizeof(tmptarget), "%s", "NULL");
	else
		mysql_format(gCon, tmptarget, sizeof(tmptarget), "'%e'", target);

	if(isnull(params))
		format(tmpparams, sizeof(tmpparams), "%s", "NULL");
	else
		mysql_format(gCon, tmpparams, sizeof(tmpparams), "'%e'", params);

	if(isnull(command))
		format(tmpcommand, sizeof(tmpcommand), "%s", "NULL");
	else
		mysql_format(gCon, tmpcommand, sizeof(tmpcommand), "'%e'", command);

	mysql_format(gCon, query, sizeof(query), "INSERT INTO Logs (Type, Player, Target, Command, Params) VALUES (%i, %s, %s, %s, %s)", logtype, tmpplayer, tmptarget, tmpcommand, tmpparams);
	mysql_tquery(gCon, query);
}

stock BanPlayer(playerid, admin, reason[])
{
	new string[128], tmpadmin[MAX_PLAYER_NAME];
	if(admin != INVALID_PLAYER_ID)
	{
		format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"has banned %s (%i). Reason: %s", GetPlayerLevelEx(GetPlayerLevel(admin)), GetPlayerNameEx(admin), GetPlayerNameEx(playerid), playerid, reason);
		mysql_format(gCon, tmpadmin, sizeof(tmpadmin), "'%e'", GetPlayerNameEx(admin));
	}
	else
	{
		format(string, sizeof(string), COL_ADMIN_1"-[%s]- "COL_ADMIN_2"has banned %s (%i). Reason: %s", AC_NAME, GetPlayerNameEx(playerid), playerid, reason);
		format(tmpadmin, sizeof(tmpadmin), "%s", "NULL");
	}

	SendClientMessageToAll(-1, string);

	mysql_format(gCon, query, sizeof(query), "INSERT INTO Bans (Player, Admin, Reason, IP) VALUE ('%e', %s, '%e', '%s')", GetPlayerNameEx(playerid), tmpadmin, reason, PlayerInfo[playerid][IP]);
	mysql_tquery(gCon, query);

	#if LOG_ANTICHEAT
	if(admin == INVALID_PLAYER_ID)
		SaveLog(LOG_TYPE_AC, "", "BAN", GetPlayerNameEx(playerid), reason);
	#endif

	Kick(playerid);
}

stock KickPlayer(playerid, admin, reason[])
{
	new string[128], tmpadmin[MAX_PLAYER_NAME];
	if(admin != INVALID_PLAYER_ID)
	{
		format(string, sizeof(string), COL_ADMIN_1"-[%s: %s]- "COL_ADMIN_2"kicked %s (%i). Reason: %s",  GetPlayerLevelEx(GetPlayerLevel(admin)), GetPlayerNameEx(admin), GetPlayerNameEx(playerid), playerid, reason);
		mysql_format(gCon, tmpadmin, sizeof(tmpadmin), "'%e'", GetPlayerNameEx(admin));
	}
	else
	{
		format(string, sizeof(string), COL_ADMIN_1"-[%s]- "COL_ADMIN_2"kicked %s (%i). Reason: %s", AC_NAME, GetPlayerNameEx(playerid), playerid, reason);
		format(tmpadmin, sizeof(tmpadmin), "%s", "NULL");
	}

	SendClientMessageToAll(-1, string);

	#if LOG_ANTICHEAT
	if(admin == INVALID_PLAYER_ID)
		SaveLog(LOG_TYPE_AC, "", "KICK", GetPlayerNameEx(playerid), reason);
	#endif
	Kick(playerid);
}

stock GetPlayerLevel(playerid) return PlayerInfo[playerid][Adminlevel];

stock GetPlayerLevelEx(level)
{
    new playerlevel[32];
    switch(level)
    {
        case LEVEL_TRIAL_MOD: playerlevel = "Trial moderator";
        case LEVEL_MOD: playerlevel = "Moderator";
        case LEVEL_TRIAL_ADMIN: playerlevel = "Trial administrator";
        case LEVEL_ADMIN: playerlevel = "Administrator";
        case LEVEL_OWNER: playerlevel = "Owner";
        default: playerlevel = "Normal player";
    }
    return playerlevel;
}

cPanel_func:IsPlayerVIP(playerid) return VipInfo[playerid][Duration] != -1;
cPanel_func:IsPlayerAdminEx(playerid, level) return PlayerInfo[playerid][Adminlevel] >= level;
cPanel_func:IsPlayerMuted(playerid) return PlayerInfo[playerid][Muted] > 0;

cPanel_func:GivePlayerScore(playerid, score)
{
	PlayerInfo[playerid][Score] += score;
	SetPlayerScore(playerid, PlayerInfo[playerid][Score]);
}

cPanel_func:SetPlayerScoreEx(playerid, score)
{
	PlayerInfo[playerid][Score] = score;
	SetPlayerScore(playerid, PlayerInfo[playerid][Score]);
}

cPanel_func:GetPlayerScoreEx(playerid) return PlayerInfo[playerid][Score];

cPanel_func:GivePlayerCash(playerid, money)
{
    PlayerInfo[playerid][Money] += money;

    ResetPlayerMoney(playerid);
    GivePlayerMoney(playerid, PlayerInfo[playerid][Money]);
}

cPanel_func:SetPlayerCash(playerid, money)
{
    PlayerInfo[playerid][Money] = money;

    ResetPlayerMoney(playerid);
    GivePlayerMoney(playerid, PlayerInfo[playerid][Money]);
}

cPanel_func:GetPlayerCash(playerid) return PlayerInfo[playerid][Money];
stock GetPlayerNameEx(playerid)
{
    new name[MAX_PLAYER_NAME];
    strins(name, PlayerInfo[playerid][Name], 0);
    return name;
}

cPanel_func:IsWeaponForbidden(weaponid)
{
	for(new i = 0; i < sizeof(forbiddenWeapons); i++)
	{
		if(weaponid == forbiddenWeapons[i]) return true;
	}
	return false;
}

stock UnlockVehicle(vehicleid) 
{
	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	SetVehicleParamsEx(vehicleid, engine, lights, alarm, VEHICLE_PARAMS_OFF, bonnet, boot, objective);
}

stock LockVehicle(vehicleid) 
{
	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	SetVehicleParamsEx(vehicleid, engine, lights, alarm, VEHICLE_PARAMS_ON, bonnet, boot, objective);
}

stock GetWeaponNameEx(weaponid)
{
	new weapon[32];
	GetWeaponName(weaponid, weapon, sizeof(weapon));
	return weapon;
}
stock IsPlayerCaged(playerid) return IsValidObject(PlayerInfo[playerid][Cage][0]);

stock CagePlayer(playerid)
{
	new Float:pos[3];
	GetPlayerPos(playerid, pos[0], pos[1], pos[2]);

	PlayerInfo[playerid][Cage][0] = CreateObject(985, pos[0], pos[1] + 4.0, pos[2], 0.0, 0.0, 0.0);
	PlayerInfo[playerid][Cage][1] = CreateObject(985, pos[0] + 4.0, pos[1], pos[2], 0.0, 0.0, 90.0);
	PlayerInfo[playerid][Cage][2] = CreateObject(985, pos[0] - 4.0, pos[1], pos[2], 0.0, 0.0, 270.0);
	PlayerInfo[playerid][Cage][3] = CreateObject(985, pos[0], pos[1] - 4.0, pos[2], 0.0, 0.0, 180.0);

	PlayerPlaySound(playerid, 1137, pos[0], pos[1], pos[2]);
	
}

stock UncagePlayer(playerid)
{
	for(new i = 0; i < 4; i++) 
	{
		DestroyObject(PlayerInfo[playerid][Cage][i]);
		PlayerInfo[playerid][Cage][i] = INVALID_OBJECT_ID;
	}
}

stock IsNumeric(const string[])
{
    for (new i = 0, j = strlen(string); i < j; i++)
    {
        if (string[i] > '9' || string[i] < '0') return 0;
    }
    return 1;
}

stock IsAdvertisement(string[])
{
	new iCount,	iPeriod, iPos, iChar, iColon;

	while((iChar = string[iPos++])) 
	{
		if('0' <= iChar <= '9') iCount++;
		else if(iChar == '.') iPeriod++;
		else if(iChar == ':') iColon++;
	}

	return (iCount >= 7 && iPeriod >= 3 && iColon >= 1);
}

stock MsToString(millisecond, format[])
{
    new
        tmp[4],
        result[64],
        hour,
        minute,
        second,
        format_char,
        result_lenght,
        bool:padding,
        len = strlen(format);
 
    hour            = (millisecond / (1000 * 60 * 60));
    minute          = (millisecond % (1000 * 60 * 60)) / (1000 * 60);
    second          = ((millisecond % (1000 * 60 * 60)) % (1000 * 60)) / 1000;
    millisecond     = millisecond - (hour * 60 * 60 * 1000) - (minute * 60 * 1000) - (second * 1000);
 
    while(format_char < len)
    {
        if(format[format_char] == '%')
        {
            format_char++;
 
            if(format[format_char] == '1')
            {
                padding = true;
                format_char++;
            }
            else
            {
                padding = false;
            }
 
            switch(format[format_char])
            {
                case 'h':
                {
                    valstr(tmp, hour);
 
                    if(padding)
                    {
                        if(hour < 10)
                            strcat(result, "0");
                    }
 
                    strcat(result, tmp);
                    result_lenght = strlen(result);
                }
 
                case 'm':
                {
                    valstr(tmp, minute);
 
                    if(padding)
                    {
                        if(minute < 10)
                            strcat(result, "0");
                    }
 
                    strcat(result, tmp);
                    result_lenght = strlen(result);
                }
 
                case 's':
                {
                    valstr(tmp, second);
 
                    if(padding)
                    {
                        if(second < 10)
                            strcat(result, "0");
                    }
 
                    strcat(result, tmp);
                    result_lenght = strlen(result);
                }
 
                case 'd':
                {
                    valstr(tmp, millisecond);
 
                    if(padding)
                    {
                        if(millisecond < 10)
                            strcat(result, "00");
 
                        else if(millisecond < 100)
                            strcat(result, "0");
                    }
 
                    strcat(result, tmp);
                    result_lenght = strlen(result);
                }
            }
        }
        else
        {
            result[result_lenght] = format[format_char];
            result_lenght++;
        }
 
        format_char++;
    }
 
    return result;
}